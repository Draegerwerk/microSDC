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
  mdib_->mdState = BICEPS::PM::MdState();
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
  std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
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
  if (locationContextState_ != nullptr && locationContextState_->locationDetail.has_value())
  {
    discoveryService_->setLocation(locationContextState_->locationDetail.value());
  }
  if (networkConfig_->discoveryProxy().has_value())
  {
    LOG(LogLevel::INFO,
        "Configuring discovery Proxy: " << networkConfig_->discoveryProxy().value());
    discoveryService_->configureProxy(networkConfig_->discoveryProxyProtocol(),
                                      networkConfig_->discoveryProxy().value());
  }

  // construct subscription manager
  subscriptionManager_ = std::make_shared<SubscriptionManager>(networkConfig_->isUsingTLS());

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
    std::lock_guard<std::mutex> lock(mdibMutex_);
    mdib_->mdState->state.emplace_back(handler->getInitialState());
  }
}

void MicroSDC::setLocation(const std::string& descriptorHandle,
                           const BICEPS::PM::LocationDetail& locationDetail)
{
  auto mdibVersion = getMdibVersion();
  std::lock_guard<std::mutex> lock(mdibMutex_);
  if (locationContextState_ == nullptr)
  {
    locationContextState_ =
        std::make_shared<BICEPS::PM::LocationContextState>(descriptorHandle, descriptorHandle);
    mdib_->mdState->state.emplace_back(locationContextState_);
  }
  locationContextState_->locationDetail = locationDetail;

  BICEPS::PM::InstanceIdentifier identification;
  identification.root = WS::ADDRESSING::URIType("sdc.ctxt.loc.detail");
  identification.extension = locationDetail.facility.value() + "///" + locationDetail.poC.value() +
                             "//" + locationDetail.bed.value();
  locationContextState_->identification.emplace_back(identification);

  BICEPS::PM::InstanceIdentifier validator;
  identification.root = WS::ADDRESSING::URIType("Validator");
  identification.extension = "System";
  locationContextState_->validator.emplace_back(validator);

  locationContextState_->contextAssociation = BICEPS::PM::ContextAssociation::Assoc;
  locationContextState_->bindingMdibVersion = mdibVersion;

  if (discoveryService_ != nullptr && locationContextState_->locationDetail.has_value())
  {
    discoveryService_->setLocation(locationContextState_->locationDetail.value());
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
  mdib_->mdDescription = mdDescription;
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

void MicroSDC::updateState(const std::shared_ptr<BICEPS::PM::AbstractState>& state)
{
  std::lock_guard<std::mutex> lock(runningMutex_);
  if (!running_)
  {
    return;
  }
  auto newState = updateMdib(state);
  if (const auto metricState = dyn_cast<const BICEPS::PM::AbstractMetricState>(newState);
      metricState != nullptr)
  {
    notifyEpisodicMetricReport(metricState);
  }
}

std::shared_ptr<const BICEPS::PM::AbstractState>
MicroSDC::updateMdib(std::shared_ptr<BICEPS::PM::AbstractState> newState)
{
  incrementMdibVersion();
  std::lock_guard<std::mutex> lock(mdibMutex_);
  const auto state = std::find_if(
      mdib_->mdState->state.begin(), mdib_->mdState->state.end(),
      [&](const auto& state) { return newState->descriptorHandle == state->descriptorHandle; });
  if (state == mdib_->mdState->state.end())
  {
    throw std::runtime_error("Cannot find descriptor handle '" + newState->descriptorHandle +
                             "'in mdib");
    return nullptr;
  }
  newState->stateVersion = (*state)->stateVersion.value_or(0) + 1;
  *state = newState;
  return *state;
}

void MicroSDC::incrementMdibVersion()
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  mdib_->mdibVersion = mdib_->mdibVersion.value_or(0) + 1;
}

unsigned int MicroSDC::getMdibVersion() const
{
  std::lock_guard<std::mutex> lock(mdibMutex_);
  return mdib_->mdibVersion.value_or(0);
}

void MicroSDC::notifyEpisodicMetricReport(
    std::shared_ptr<const BICEPS::PM::AbstractMetricState> state)
{
  BICEPS::MM::MetricReportPart reportPart;
  reportPart.metricState.emplace_back(std::move(state));
  BICEPS::MM::EpisodicMetricReport report(WS::ADDRESSING::URIType("0"));
  report.reportPart.emplace_back(std::move(reportPart));
  report.mdibVersion = getMdibVersion();
  subscriptionManager_->fireEvent(report);
}
