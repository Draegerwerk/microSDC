#include "MicroSDC.hpp"

#include "Casting.hpp"
#include "Log.hpp"
#include "MetadataProvider.hpp"
#include "SDCConstants.hpp"
#include "StateHandler.hpp"
#include "SubscriptionManager.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "networking/NetworkConfig.hpp"
#include "services/DeviceService.hpp"
#include "services/GetService.hpp"
#include "services/SetService.hpp"
#include "services/StateEventService.hpp"
#include "services/StaticService.hpp"
#include "uuid/UUIDGenerator.hpp"
#include "wsdl/GetServiceWSDL.hpp"
#include "wsdl/SetServiceWSDL.hpp"
#include "wsdl/StateEventServiceWSDL.hpp"

#include "asio/system_error.hpp"

MicroSDC::MicroSDC()
  : mdib_(std::make_unique<BICEPS::PM::Mdib>(WS::ADDRESSING::URIType("0")))
{
  mdib_->MdState = BICEPS::PM::MdState();
}

void MicroSDC::start()
{
  if (networkConfig_ == nullptr)
  {
    LOG(LogLevel::ERROR, "Failed to start MicroSDC. Set NetworkConfig first!");
    return;
  }

  std::lock_guard<std::mutex> lock(runningMutex_);
  if (running_)
  {
    LOG(LogLevel::WARNING, "called MicroSDC start but already running!");
    return;
  }
  webserver_ = WebServerFactory::produce(networkConfig_);
  startup();
  // MicroSDC is now ready und is running
  running_ = true;
}

void MicroSDC::startup()
{
  LOG(LogLevel::INFO, "Initialize...");

  const auto metadata =
      std::make_shared<const MetadataProvider>(networkConfig_, deviceCharacteristics_);

  // construct xAddresses containing reference to the service
  WS::DISCOVERY::UriListType xAddresses;
  std::string protocol = networkConfig_->useTLS() ? "https" : "http";
  std::string xaddress = protocol + "://" + networkConfig_->ipAddress() + ":" +
                         std::to_string(networkConfig_->port()) +
                         MetadataProvider::getDeviceServicePath();
  xAddresses.emplace_back(xaddress);

  // fill discovery types
  WS::DISCOVERY::QNameListType types;
  types.emplace_back(MDPWS::WS_NS_DPWS_PREFIX, "Device");
  types.emplace_back(MDPWS::NS_MDPWS_PREFIX, "MedicalDevice");

  initializeMdStates();

  discoveryService_ = std::make_unique<DiscoveryService>(
      WS::ADDRESSING::EndpointReferenceType::AddressType(endpointReference_), types, xAddresses);
  if (locationContextState_ != nullptr && locationContextState_->LocationDetail.has_value())
  {
    discoveryService_->setLocation(locationContextState_->LocationDetail.value());
  }

  // construct subscription manager
  subscriptionManager_ = std::make_shared<SubscriptionManager>();

  // construct web services
  auto deviceService = std::make_shared<DeviceService>(metadata);
  auto getService = std::make_shared<GetService>(*this, metadata);
  auto getWSDLService =
      std::make_shared<StaticService>(getService->getURI() + "/wsdl", WSDL::GET_SERVICE_WSDL);
  auto setService = std::make_shared<SetService>(*this, metadata, subscriptionManager_);
  auto setWSDLService =
      std::make_shared<StaticService>(setService->getURI() + "/wsdl", WSDL::SET_SERVICE_WSDL);
  auto stateEventService =
      std::make_shared<StateEventService>(*this, metadata, subscriptionManager_);
  auto stateEventWSDLService = std::make_shared<StaticService>(
      stateEventService->getURI() + "/wsdl", WSDL::STATE_EVENT_SERVICE_SERVICE_WSDL);

  // register webservices
  webserver_->addService(deviceService);
  webserver_->addService(getService);
  webserver_->addService(getWSDLService);
  webserver_->addService(setService);
  webserver_->addService(setWSDLService);
  webserver_->addService(stateEventService);
  webserver_->addService(stateEventWSDLService);

  webserver_->start();
  discoveryService_->start();
}

void MicroSDC::stop()
{
  std::lock_guard<std::mutex> lock(runningMutex_);
  if (running_)
  {
    discoveryService_->stop();
    webserver_->stop();
    sdcThread_.join();
    running_ = false;
    LOG(LogLevel::INFO, "stopped");
  }
}

bool MicroSDC::isRunning() const
{
  std::lock_guard<std::mutex> lock(runningMutex_);
  return running_;
}

void MicroSDC::initializeMdStates()
{
  for (const auto& handler : stateHandlers_)
  {
    if (const auto numericHandler = dyn_cast<NumericStateHandler>(handler);
        numericHandler != nullptr)
    {
      std::lock_guard<std::mutex> lock(mdibMutex_);
      mdib_->MdState->State.emplace_back(numericHandler->getInitialState());
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
    mdib_->MdState->State.emplace_back(locationContextState_);
  }
  locationContextState_->LocationDetail = locationDetail;

  BICEPS::PM::InstanceIdentifier identification;
  identification.Root = WS::ADDRESSING::URIType("sdc.ctxt.loc.detail");
  identification.Extension = locationDetail.Facility.value() + "///" + locationDetail.PoC.value() +
                             "//" + locationDetail.Bed.value();
  locationContextState_->Identification.emplace_back(identification);

  BICEPS::PM::InstanceIdentifier validator;
  identification.Root = WS::ADDRESSING::URIType("Validator");
  identification.Extension = "System";
  locationContextState_->Validator.emplace_back(validator);

  locationContextState_->ContextAssociation = BICEPS::PM::ContextAssociation::Assoc;
  locationContextState_->BindingMdibVersion = mdibVersion;

  if (discoveryService_ != nullptr && locationContextState_->LocationDetail.has_value())
  {
    discoveryService_->setLocation(locationContextState_->LocationDetail.value());
  }
}

const BICEPS::PM::Mdib& MicroSDC::getMdib() const
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  return *mdib_;
}

void MicroSDC::setMdDescription(const BICEPS::PM::MdDescription& mdDescription)
{
  std::lock_guard<std::mutex> runningLock(runningMutex_);
  if (running_)
  {
    throw std::runtime_error("MicroSDC has to be stopped to set MdDescription!");
  }
  std::lock_guard<std::mutex> lock(mdibMutex_);
  mdib_->MdDescription = mdDescription;
}

void MicroSDC::setDeviceCharacteristics(DeviceCharacteristics devChar)
{
  std::lock_guard<std::mutex> lock(runningMutex_);
  if (running_)
  {
    throw std::runtime_error("MicroSDC has to be stopped to set DeviceCharacteristics!");
  }
  deviceCharacteristics_ = std::move(devChar);
  std::lock_guard<std::mutex> eprLock(eprMutex_);
  deviceCharacteristics_.setEndpointReference(endpointReference_);
}

void MicroSDC::setEndpointReference(const std::string& epr)
{
  std::lock_guard<std::mutex> eprLock{eprMutex_};
  endpointReference_ = epr;
}

std::string MicroSDC::getEndpointReference() const
{
  std::lock_guard<std::mutex> eprLock{eprMutex_};
  return endpointReference_;
}

void MicroSDC::setNetworkConfig(std::unique_ptr<NetworkConfig> networkConfig)
{
  std::lock_guard<std::mutex> lock(runningMutex_);
  if (running_)
  {
    throw std::runtime_error("MicroSDC has to be stopped to set NetworkConfig!");
  }
  networkConfig_ = std::move(networkConfig);
}

std::string MicroSDC::calculateUUID()
{
  auto uuid = UUIDGenerator{}();
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
  std::lock_guard<std::mutex> lock(runningMutex_);
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
  for (auto& state : mdib_->MdState->State)
  {
    if (newState->DescriptorHandle == state->DescriptorHandle)
    {
      newState->StateVersion = state->StateVersion.value_or(0) + 1;
      state = newState;
      return newState;
    }
  }
  throw std::runtime_error("Cannot find descriptor handle '" + newState->DescriptorHandle +
                           "'in mdib");
  return nullptr;
}

void MicroSDC::incrementMdibVersion()
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  mdib_->MdibVersion = mdib_->MdibVersion.value_or(0) + 1;
}

unsigned int MicroSDC::getMdibVersion() const
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  return mdib_->MdibVersion.value_or(0);
}

void MicroSDC::notifyEpisodicMetricReport(
    std::shared_ptr<const BICEPS::PM::NumericMetricState> state)
{
  BICEPS::MM::MetricReportPart reportPart;
  reportPart.MetricState.emplace_back(std::move(state));
  BICEPS::MM::EpisodicMetricReport report(WS::ADDRESSING::URIType("0"));
  report.ReportPart.emplace_back(std::move(reportPart));
  report.MdibVersion = getMdibVersion();
  subscriptionManager_->fireEvent(report);
}
