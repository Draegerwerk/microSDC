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
  if (network_config_ == nullptr)
  {
    LOG(LogLevel::ERROR, "Failed to start MicroSDC. Set NetworkConfig first!");
    return;
  }

  std::lock_guard<std::mutex> lock(running_mutex_);
  if (running_)
  {
    LOG(LogLevel::WARNING, "called MicroSDC start but already running!");
    return;
  }
  webserver_ = WebServerFactory::produce(network_config_);
  startup();
  // MicroSDC is now ready und is running
  running_ = true;
}

void MicroSDC::startup()
{
  LOG(LogLevel::INFO, "Initialize...");

  const auto metadata =
      std::make_shared<const MetadataProvider>(network_config_, device_characteristics_);

  // construct xAddresses containing reference to the service
  WS::DISCOVERY::UriListType x_addresses;
  std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  std::string xaddress = protocol + "://" + network_config_->ip_address() + ":" +
                         std::to_string(network_config_->port()) +
                         MetadataProvider::get_device_service_path();
  x_addresses.emplace_back(xaddress);

  // fill discovery types
  WS::DISCOVERY::QNameListType types;
  types.emplace_back(MDPWS::WS_NS_DPWS_PREFIX, "Device");
  types.emplace_back(MDPWS::NS_MDPWS_PREFIX, "MedicalDevice");

  initialize_md_states();

  discovery_service_ = std::make_unique<DiscoveryService>(
      WS::ADDRESSING::EndpointReferenceType::AddressType(endpoint_reference_), types, x_addresses);
  if (location_context_state_ != nullptr && location_context_state_->locationDetail.has_value())
  {
    discovery_service_->set_location(location_context_state_->locationDetail.value());
  }
  if (network_config_->discovery_proxy().has_value())
  {
    LOG(LogLevel::INFO,
        "Configuring discovery Proxy: " << network_config_->discovery_proxy().value());
    discovery_service_->configure_proxy(network_config_->discovery_proxy_protocol(),
                                        network_config_->discovery_proxy().value());
  }

  // construct subscription manager
  subscription_manager_ = std::make_shared<SubscriptionManager>(network_config_->is_using_tls());

  // construct web services
  auto device_service = std::make_shared<DeviceService>(metadata);
  auto get_service = std::make_shared<GetService>(*this, metadata);
  auto get_wsdl_service =
      std::make_shared<StaticService>(get_service->get_uri() + "/wsdl", WSDL::GET_SERVICE_WSDL);
  auto set_service = std::make_shared<SetService>(this, metadata, subscription_manager_);
  auto set_wsdl_service =
      std::make_shared<StaticService>(set_service->get_uri() + "/wsdl", WSDL::SET_SERVICE_WSDL);
  auto state_event_service =
      std::make_shared<StateEventService>(*this, metadata, subscription_manager_);
  auto state_event_wsdl_service = std::make_shared<StaticService>(
      state_event_service->get_uri() + "/wsdl", WSDL::STATE_EVENT_SERVICE_SERVICE_WSDL);

  // register webservices
  webserver_->add_service(device_service);
  webserver_->add_service(get_service);
  webserver_->add_service(get_wsdl_service);
  webserver_->add_service(set_service);
  webserver_->add_service(set_wsdl_service);
  webserver_->add_service(state_event_service);
  webserver_->add_service(state_event_wsdl_service);

  webserver_->start();
  discovery_service_->start();
}

void MicroSDC::stop()
{
  std::lock_guard<std::mutex> lock(running_mutex_);
  if (running_)
  {
    discovery_service_->stop();
    webserver_->stop();
    sdc_thread_.join();
    running_ = false;
    LOG(LogLevel::INFO, "stopped");
  }
}

bool MicroSDC::is_running() const
{
  std::lock_guard<std::mutex> lock(running_mutex_);
  return running_;
}

void MicroSDC::initialize_md_states()
{
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  for (const auto& handler : state_handlers_)
  {
    mdib_->mdState->state.emplace_back(handler->get_initial_state());
  }
  for (const auto& md : mdib_->mdDescription->mds)
  {
    for (const auto& vmd : md.vmd)
    {
      if (!vmd.sco.has_value())
      {
        continue;
      }
      for (const auto& operation : vmd.sco.value().operation)
      {
        if (const auto descriptor = dyn_cast<BICEPS::PM::SetValueOperationDescriptor>(operation);
            descriptor != nullptr)
        {
          mdib_->mdState->state.emplace_back(std::make_shared<BICEPS::PM::SetValueOperationState>(
              descriptor->handle, BICEPS::PM::OperatingMode::NA));
        }
        else if (const auto descriptor =
                     dyn_cast<BICEPS::PM::SetStringOperationDescriptor>(operation);
                 descriptor != nullptr)
        {
          mdib_->mdState->state.emplace_back(std::make_shared<BICEPS::PM::SetStringOperationState>(
              descriptor->handle, BICEPS::PM::OperatingMode::NA));
        }
      }
    }
  }
}

void MicroSDC::set_location(const std::string& descriptor_handle,
                            const BICEPS::PM::LocationDetail& location_detail)
{
  auto mdib_version = get_mdib_version();
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  if (location_context_state_ == nullptr)
  {
    location_context_state_ =
        std::make_shared<BICEPS::PM::LocationContextState>(descriptor_handle, descriptor_handle);
    mdib_->mdState->state.emplace_back(location_context_state_);
  }
  location_context_state_->locationDetail = location_detail;

  BICEPS::PM::InstanceIdentifier identification;
  identification.root = WS::ADDRESSING::URIType("sdc.ctxt.loc.detail");
  identification.extension = location_detail.facility.value() + "///" +
                             location_detail.poC.value() + "//" + location_detail.bed.value();
  location_context_state_->identification.emplace_back(identification);

  BICEPS::PM::InstanceIdentifier validator;
  identification.root = WS::ADDRESSING::URIType("Validator");
  identification.extension = "System";
  location_context_state_->validator.emplace_back(validator);

  location_context_state_->contextAssociation = BICEPS::PM::ContextAssociation::Assoc;
  location_context_state_->bindingMdibVersion = mdib_version;

  if (discovery_service_ != nullptr && location_context_state_->locationDetail.has_value())
  {
    discovery_service_->set_location(location_context_state_->locationDetail.value());
  }
}

BICEPS::MM::InvocationState MicroSDC::request_state_change(const BICEPS::MM::AbstractSet& set)
{
  const auto target_handle = find_operation_target_for_operation_handle(set.operationHandleRef);
  if (!target_handle.has_value())
  {
    LOG(LogLevel::ERROR, "No operation target for " << set.operationHandleRef << " found!");
    return BICEPS::MM::InvocationState::Fail;
  }
  const auto handler =
      std::find_if(state_handlers_.begin(), state_handlers_.end(), [&](const auto& handler) {
        return handler->get_descriptor_handle() == target_handle.value();
      });
  if (handler == state_handlers_.end())
  {
    LOG(LogLevel::ERROR, "No state handler for " << target_handle.value() << " found!");
    return BICEPS::MM::InvocationState::Fail;
  }
  return (*handler)->request_state_change(set);
}

const BICEPS::PM::Mdib& MicroSDC::get_mdib() const
{
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  return *mdib_;
}

void MicroSDC::set_md_description(const BICEPS::PM::MdDescription& md_description)
{
  std::lock_guard<std::mutex> running_lock(running_mutex_);
  if (running_)
  {
    throw std::runtime_error("MicroSDC has to be stopped to set MdDescription!");
  }
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  mdib_->mdDescription = md_description;
}

void MicroSDC::set_device_characteristics(DeviceCharacteristics dev_char)
{
  std::lock_guard<std::mutex> lock(running_mutex_);
  if (running_)
  {
    throw std::runtime_error("MicroSDC has to be stopped to set DeviceCharacteristics!");
  }
  device_characteristics_ = std::move(dev_char);
  std::lock_guard<std::mutex> epr_lock(epr_mutex_);
  device_characteristics_.set_endpoint_reference(endpoint_reference_);
}

void MicroSDC::set_endpoint_reference(const std::string& epr)
{
  std::lock_guard<std::mutex> epr_lock{epr_mutex_};
  endpoint_reference_ = epr;
}

std::string MicroSDC::get_endpoint_reference() const
{
  std::lock_guard<std::mutex> epr_lock{epr_mutex_};
  return endpoint_reference_;
}

void MicroSDC::set_network_config(std::unique_ptr<NetworkConfig> network_config)
{
  std::lock_guard<std::mutex> lock(running_mutex_);
  if (running_)
  {
    throw std::runtime_error("MicroSDC has to be stopped to set NetworkConfig!");
  }
  network_config_ = std::move(network_config);
}

std::string MicroSDC::calculate_uuid()
{
  auto uuid = UUIDGenerator{}();
  return uuid.to_string();
}

std::string MicroSDC::calculate_message_id()
{
  return std::string(SDC::UUID_SDC_PREFIX + MicroSDC::calculate_uuid());
}

void MicroSDC::add_md_state(std::shared_ptr<StateHandler> state_handler)
{
  state_handler->set_micro_sdc(this);
  state_handlers_.emplace_back(std::move(state_handler));
}

void MicroSDC::update_state(const std::shared_ptr<BICEPS::PM::AbstractState>& state)
{
  std::lock_guard<std::mutex> lock(running_mutex_);
  if (!running_)
  {
    return;
  }
  auto new_state = update_mdib(state);
  if (const auto metric_state = dyn_cast<const BICEPS::PM::AbstractMetricState>(new_state);
      metric_state != nullptr)
  {
    notify_episodic_metric_report(metric_state);
  }
}

std::shared_ptr<const BICEPS::PM::AbstractState>
MicroSDC::update_mdib(std::shared_ptr<BICEPS::PM::AbstractState> new_state)
{
  increment_mdib_version();
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  const auto state = std::find_if(mdib_->mdState->state.begin(), mdib_->mdState->state.end(),
                                  [new_state](const auto& state) {
                                    return new_state->descriptorHandle == state->descriptorHandle;
                                  });
  if (state == mdib_->mdState->state.end())
  {
    throw std::runtime_error("Cannot find descriptor handle '" + new_state->descriptorHandle +
                             "'in mdib");
    return nullptr;
  }
  new_state->stateVersion = (*state)->stateVersion.value_or(0) + 1;
  *state = new_state;
  return *state;
}

void MicroSDC::increment_mdib_version()
{
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  mdib_->mdibVersion = mdib_->mdibVersion.value_or(0) + 1;
}

unsigned int MicroSDC::get_mdib_version() const
{
  std::lock_guard<std::mutex> lock(mdib_mutex_);
  return mdib_->mdibVersion.value_or(0);
}

void MicroSDC::notify_episodic_metric_report(
    std::shared_ptr<const BICEPS::PM::AbstractMetricState> state)
{
  BICEPS::MM::MetricReportPart report_part;
  report_part.metricState.emplace_back(std::move(state));
  BICEPS::MM::EpisodicMetricReport report(WS::ADDRESSING::URIType("0"));
  report.reportPart.emplace_back(std::move(report_part));
  report.mdibVersion = get_mdib_version();
  subscription_manager_->fire_event(report);
}


std::optional<BICEPS::PM::AbstractOperationDescriptor::OperationTargetType>
MicroSDC::find_operation_target_for_operation_handle(
    const BICEPS::PM::AbstractDescriptor::HandleType& handle) const
{
  if (!mdib_->mdDescription.has_value())
  {
    return {};
  }
  for (const auto& md : mdib_->mdDescription->mds)
  {
    for (const auto& vmd : md.vmd)
    {
      if (!vmd.sco.has_value())
      {
        continue;
      }
      for (const auto& operation : vmd.sco->operation)
      {
        if (operation->handle == handle)
        {
          return operation->operationTarget;
        }
      }
    }
  }
  return {};
}
