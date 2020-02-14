#include "MicroSDC.hpp"
#include "NetworkHandler.hpp"
#include "SDCConstants.hpp"
#include "StateHandler.hpp"
#include "UUIDGenerator.hpp"
#include "asio/system_error.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "dpws/MetadataProvider.hpp"
#include "esp_log.h"
#include "services/DeviceService.hpp"
#include "services/GetService.hpp"
#include "services/SetService.hpp"
#include "services/StaticService.hpp"
#include "wsdl/GetServiceWSDL.hpp"
#include "wsdl/SetServiceWSDL.hpp"

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
  }
  catch (const asio::system_error& e)
  {
    ESP_LOGE(TAG, "asio system_error: %s", e.what());
    return;
  }

  // construct web services
  auto deviceService = std::make_shared<DeviceService>(metadata);
  auto getService = std::make_shared<GetService>(*this, metadata);
  auto getWSDLService =
      std::make_shared<StaticService>(getService->getURI() + "/?wsdl", WSDL::GET_SERVICE_WSDL);
  auto setService = std::make_shared<SetService>(*this, metadata);
  auto setWSDLService =
      std::make_shared<StaticService>(setService->getURI() + "/?wsdl", WSDL::SET_SERVICE_WSDL);

  webserver_ = std::make_unique<WebServer>(useTLS_);

  // register webservices
  webserver_->addService(deviceService);
  webserver_->addService(getService);
  webserver_->addService(getWSDLService);
  webserver_->addService(setService);
  webserver_->addService(setWSDLService);

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
  for (const auto& [descriptorHandle, handler] : stateHandlers_)
  {
    if (handler->getMetricType() == BICEPS::PM::MetricType::NUMERIC)
    {
      auto& numericHandler =
          static_cast<const MdStateHandler<BICEPS::PM::NumericMetricState>&>(*handler);
      std::lock_guard<std::mutex> lock(mdibMutex_);
      mdib_->MdState()->State().emplace_back(numericHandler.getInitialState());
    }
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
  stateHandlers_[stateHandler->getDescriptorHandle()] = std::move(stateHandler);
}

void MicroSDC::updateState(std::shared_ptr<BICEPS::PM::NumericMetricState> state)
{
  updateMdib(std::move(state));
}

template <class T>
void MicroSDC::updateMdib(std::shared_ptr<T> newState)
{
  incrementMdibVersion();
  std::lock_guard<std::mutex> lock(mdibMutex_);
  for (auto& state : mdib_->MdState()->State())
  {
    if (newState->DescriptorHandle() == state->DescriptorHandle())
    {
      state = newState;
    }
  }
}

void MicroSDC::incrementMdibVersion()
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  mdib_->MdibVersion() = mdib_->MdibVersion().value_or(0) + 1;
}
