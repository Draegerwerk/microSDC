#include "MicroSDC.hpp"
#include "NetworkHandler.hpp"
#include "SDCConstants.hpp"
#include "StateHandler.hpp"
#include "UUIDGenerator.hpp"
#include "asio/system_error.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "dpws/MetadataProvider.hpp"
#include "esp_log.h"
#include "esp_tls.h"
#include "services/DeviceService.hpp"
#include "services/GetService.hpp"
#include "services/SetService.hpp"
#include "services/StateEventService.hpp"
#include "services/StaticService.hpp"
#include "wsdl/GetServiceWSDL.hpp"
#include "wsdl/SetServiceWSDL.hpp"
#include "wsdl/StateEventServiceWSDL.hpp"

static constexpr const char* TAG = "MicroSDC";

MicroSDC::MicroSDC()
  : mdib_(std::make_unique<BICEPS::PM::Mdib>(std::string("0")))
{
  mdib_->MdState() = BICEPS::PM::MdState();
}

void MicroSDC::start()
{
  sdcThread_ = std::thread([this]() { startup(); });
}

void MicroSDC::startup()
{
  {
    std::lock_guard<std::mutex> lock(runningMutex_);
    if (running_)
    {
      ESP_LOGW(TAG, "called MicroSDC connect but already running!");
      return;
    }
    running_ = true;
  } // free lock of running bool
  ESP_LOGI(TAG, "Initialize...");

  MetadataProvider metadata(getDeviceCharacteristics(), useTLS_);

  // construct xAddresses containing reference to the service
  WS::DISCOVERY::UriListType xAddresses;
  std::string protocol = useTLS_ ? "https" : "http";
  std::string xaddress = protocol + "://" + NetworkHandler::getInstance().address() +
                         (useTLS_ ? ":443" : ":80") + metadata.getDeviceServicePath();
  xAddresses.emplace_back(xaddress);

  // fill discovery types
  WS::DISCOVERY::QNameListType types;
  types.emplace_back(MDPWS::WS_NS_DPWS_PREFIX, "Device");
  types.emplace_back(MDPWS::NS_MDPWS_PREFIX, "MedicalDevice");

  initializeMdStates();

  try
  {
    dpws_ = std::make_unique<DPWSHost>(
        WS::ADDRESSING::EndpointReferenceType::AddressType(endpointReference_), types, xAddresses);
    if (locationContextState_ != nullptr && locationContextState_->LocationDetail().has_value())
    {
      dpws_->setLocation(locationContextState_->LocationDetail().value());
    }
  }
  catch (const asio::system_error& e)
  {
    ESP_LOGE(TAG, "asio system_error: %s", e.what());
    return;
  }

  // initialize global ca store for client communication
  ESP_ERROR_CHECK(esp_tls_init_global_ca_store());
  extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
  extern const unsigned char cacert_pem_end[] asm("_binary_cacert_pem_end");
  const std::size_t cacert_len = cacert_pem_end - cacert_pem_start;
  ESP_ERROR_CHECK(esp_tls_set_global_ca_store(cacert_pem_start, cacert_len));

  // construct subscription manager
  subscriptionManager_ = std::make_shared<SubscriptionManager>();

  // construct web services
  auto deviceService = std::make_shared<DeviceService>(metadata);
  auto getService = std::make_shared<GetService>(*this, metadata);
  auto getWSDLService =
      std::make_shared<StaticService>(getService->getURI() + "/?wsdl", WSDL::GET_SERVICE_WSDL);
  auto setService = std::make_shared<SetService>(*this, metadata, subscriptionManager_);
  auto setWSDLService =
      std::make_shared<StaticService>(setService->getURI() + "/?wsdl", WSDL::SET_SERVICE_WSDL);
  auto stateEventService =
      std::make_shared<StateEventService>(*this, metadata, subscriptionManager_);
  auto stateEventWSDLService = std::make_shared<StaticService>(
      stateEventService->getURI() + "/?wsdl", WSDL::STATE_EVENT_SERVICE_SERVICE_WSDL);

  webserver_ = std::make_unique<WebServer>(useTLS_);

  // register webservices
  webserver_->addService(deviceService);
  webserver_->addService(getService);
  webserver_->addService(getWSDLService);
  webserver_->addService(setService);
  webserver_->addService(setWSDLService);
  webserver_->addService(stateEventService);
  webserver_->addService(stateEventWSDLService);

  webserver_->start();
  dpws_->start();
}

void MicroSDC::stop()
{
  std::lock_guard<std::mutex> lock(runningMutex_);
  if (running_)
  {
    dpws_->stop();
    webserver_->stop();
    running_ = false;
    sdcThread_.join();
    ESP_LOGI(TAG, "stopped");
  }
}

void MicroSDC::initializeMdStates()
{
  for (const auto& handler : stateHandlers_)
  {
    if (handler->getStateType() == BICEPS::PM::StateType::NUMERIC_METRIC)
    {
      auto& numericHandler =
          static_cast<const MdStateHandler<BICEPS::PM::NumericMetricState>&>(*handler);
      std::lock_guard<std::mutex> lock(mdibMutex_);
      mdib_->MdState()->State().emplace_back(numericHandler.getInitialState());
    }
  }
}

void MicroSDC::setLocation(const std::string& descriptorHandle,
                           const BICEPS::PM::LocationDetailType& locationDetail)
{
  auto mdibVersion = getMdibVersion();
  std::lock_guard<std::mutex> lock(mdibMutex_);
  if (locationContextState_ == nullptr)
  {
    locationContextState_ =
        std::make_shared<BICEPS::PM::LocationContextState>(descriptorHandle, descriptorHandle);
    mdib_->MdState()->State().emplace_back(locationContextState_);
  }
  locationContextState_->LocationDetail() = locationDetail;

  BICEPS::PM::InstanceIdentifier identification;
  identification.Root() = WS::ADDRESSING::URIType("sdc.ctxt.loc.detail");
  identification.Extension() = locationDetail.Facility().value() + "///" +
                               locationDetail.PoC().value() + "//" + locationDetail.Bed().value();
  locationContextState_->Identification().emplace_back(identification);

  BICEPS::PM::InstanceIdentifier validator;
  identification.Root() = WS::ADDRESSING::URIType("Validator");
  identification.Extension() = "System";
  locationContextState_->Validator().emplace_back(validator);

  locationContextState_->ContextAssociation() = BICEPS::PM::ContextAssociation::Assoc;
  locationContextState_->BindingMdibVersion() = mdibVersion;

  if (dpws_ != nullptr && locationContextState_->LocationDetail().has_value())
  {
    dpws_->setLocation(locationContextState_->LocationDetail().value());
  }
}

const BICEPS::PM::Mdib& MicroSDC::getMdib() const
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  return *mdib_;
}

void MicroSDC::setMdDescription(const BICEPS::PM::MdDescription& mdDescription)
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  mdib_->MdDescription() = mdDescription;
}

void MicroSDC::setDeviceCharacteristics(DeviceCharacteristics devChar)
{
  std::lock_guard<std::mutex> lock(deviceCharacteristicsMutex_);
  deviceCharacteristics_ = std::move(devChar);
  deviceCharacteristics_.setEndpointReference(endpointReference_);
}

const DeviceCharacteristics& MicroSDC::getDeviceCharacteristics() const
{
  std::lock_guard<std::mutex> lock(deviceCharacteristicsMutex_);
  return deviceCharacteristics_;
}

void MicroSDC::setEndpointReference(const std::string& epr)
{
  std::lock_guard<std::mutex> eprLock{eprMutex_};
  endpointReference_ = epr;
}

std::string MicroSDC::getEndpointReference() const
{
  std::lock_guard<std::mutex> eprLock{eprMutex_};
  return std::string(endpointReference_);
}

void MicroSDC::setUseTLS(const bool useTLS)
{
  this->useTLS_ = useTLS;
}

std::string MicroSDC::calculateUUID()
{
  auto uuid = UUIDGenerator().create();
  return uuid.toString();
}

std::string MicroSDC::calculateMessageID()
{
  return std::string(SDC::UUID_SDC_PREFIX + MicroSDC::calculateUUID());
}

void MicroSDC::addMdState(std::shared_ptr<StateHandler> stateHandler)
{
  stateHandler->setMicroSDC(this);
  stateHandlers_.emplace_back(std::move(stateHandler));
}

void MicroSDC::updateState(const std::shared_ptr<BICEPS::PM::NumericMetricState>& state)
{
  if (!running_)
  {
    return;
  }
  auto newState = updateMdib(state);
  notifyEpisodicMetricReport(newState);
}

template <class T>
std::shared_ptr<const T> MicroSDC::updateMdib(std::shared_ptr<T> newState)
{
  incrementMdibVersion();
  std::lock_guard<std::mutex> lock(mdibMutex_);
  for (auto& state : mdib_->MdState()->State())
  {
    if (newState->DescriptorHandle() == state->DescriptorHandle())
    {
      newState->StateVersion() = state->StateVersion().value_or(0) + 1;
      state = newState;
      return newState;
    }
  }
  throw std::runtime_error("Cannot find descriptor handle '" + newState->DescriptorHandle() +
                           "'in mdib");
  return nullptr;
}

void MicroSDC::incrementMdibVersion()
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  mdib_->MdibVersion() = mdib_->MdibVersion().value_or(0) + 1;
}

unsigned int MicroSDC::getMdibVersion() const
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  return mdib_->MdibVersion().value_or(0);
}

void MicroSDC::notifyEpisodicMetricReport(
    std::shared_ptr<const BICEPS::PM::NumericMetricState> state)
{
  if (!running_)
  {
    return;
  }
  BICEPS::MM::MetricReportPart reportPart;
  reportPart.MetricState().emplace_back(std::move(state));
  BICEPS::MM::EpisodicMetricReport report(WS::ADDRESSING::URIType("0"));
  report.ReportPart().emplace_back(std::move(reportPart));
  report.MdibVersion() = getMdibVersion();
  subscriptionManager_->fireEvent(report);
}
