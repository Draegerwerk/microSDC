#include "Log.hpp"
#include "MicroSDC.hpp"
#include "NetworkInterface.hpp"
#include "StateHandler.hpp"
#include "networking/NetworkConfig.hpp"
#include <thread>

/// @brief Implements a MdStateHandler for NumericStates
class NumericStateHandler : public StateHandler
{
public:
  /// @brief constructs a new NumericStateHandler attached to a given descriptor state handle
  /// @param descriptorHandle the handle of the state's descriptor
  explicit NumericStateHandler(const std::string& descriptor_handle)
    : StateHandler(descriptor_handle)
  {
  }

  std::shared_ptr<BICEPS::PM::AbstractState> get_initial_state() const override
  {
    auto state = std::make_shared<BICEPS::PM::NumericMetricState>(get_descriptor_handle());
    state->metric_value = std::make_optional<BICEPS::PM::NumericMetricValue>(
        BICEPS::PM::MetricQuality{BICEPS::PM::MeasurementValidity::VLD});
    state->metric_value->value = 42;
    return state;
  }

  BICEPS::MM::InvocationState request_state_change(const BICEPS::MM::AbstractSet& set) override
  {
    const auto* const set_value = dyn_cast<const BICEPS::MM::SetValue>(&set);
    if (set_value == nullptr)
    {
      LOG(LogLevel::ERROR, "Cannot cast to SetValue!");
      return BICEPS::MM::InvocationState::Fail;
    }
    this->set_value(set_value->requestedNumericValue);
    return BICEPS::MM::InvocationState::Fin;
  }

  /// @param sets a new numeric value to the state handled by this handler and updates the mdib
  /// @param value the new value to set
  void set_value(double value)
  {
    auto state = dyn_cast<BICEPS::PM::NumericMetricState>(get_initial_state());
    state->metric_value->value = value;
    update_state(state);
  }
};

class StringStateHandler : public StateHandler
{
public:
  /// @brief constructs a new NumericStateHandler attached to a given descriptor state handle
  /// @param descriptorHandle the handle of the state's descriptor
  explicit StringStateHandler(const std::string& descriptor_handle)
    : StateHandler(descriptor_handle)
  {
  }

  std::shared_ptr<BICEPS::PM::AbstractState> get_initial_state() const override
  {
    auto state = std::make_shared<BICEPS::PM::StringMetricState>(get_descriptor_handle());
    state->metric_value = std::make_optional<BICEPS::PM::StringMetricValue>(
        BICEPS::PM::MetricQuality{BICEPS::PM::MeasurementValidity::VLD});
    state->metric_value->value = "Hello Esp32!";
    return state;
  }

  BICEPS::MM::InvocationState request_state_change(const BICEPS::MM::AbstractSet& set) override
  {
    const auto* const set_string = dyn_cast<const BICEPS::MM::SetString>(&set);
    if (set_string == nullptr)
    {
      LOG(LogLevel::ERROR, "Cannot cast to SetString!");
      return BICEPS::MM::InvocationState::Fail;
    }
    this->set_value(set_string->requestedStringValue);
    return BICEPS::MM::InvocationState::Fin;
  }

  /// @param sets a new numeric value to the state handled by this handler and updates the mdib
  /// @param value the new value to set
  void set_value(std::string value)
  {
    auto state = dyn_cast<BICEPS::PM::StringMetricState>(get_initial_state());
    state->metric_value->value = value;
    update_state(state);
  }
};

class EnumStringStateHandler : public StateHandler
{
public:
  /// @brief constructs a new NumericStateHandler attached to a given descriptor state handle
  /// @param descriptorHandle the handle of the state's descriptor
  explicit EnumStringStateHandler(const std::string& descriptor_handle)
    : StateHandler(descriptor_handle)
  {
  }

  std::shared_ptr<BICEPS::PM::AbstractState> get_initial_state() const override
  {
    auto state = std::make_shared<BICEPS::PM::EnumStringMetricState>(get_descriptor_handle());
    state->metric_value = std::make_optional<BICEPS::PM::StringMetricValue>(
        BICEPS::PM::MetricQuality{BICEPS::PM::MeasurementValidity::VLD});
    state->metric_value->value = "OFF";
    return state;
  }

  BICEPS::MM::InvocationState request_state_change(const BICEPS::MM::AbstractSet& set) override
  {
    const auto* const set_string = dyn_cast<const BICEPS::MM::SetString>(&set);
    if (set_string == nullptr)
    {
      LOG(LogLevel::ERROR, "Cannot cast to SetString!");
      return BICEPS::MM::InvocationState::Fail;
    }
    this->set_value(set_string->requestedStringValue);
    return BICEPS::MM::InvocationState::Fin;
  }

  /// @param sets a new numeric value to the state handled by this handler and updates the mdib
  /// @param value the new value to set
  void set_value(std::string value)
  {
    auto state = dyn_cast<BICEPS::PM::EnumStringMetricState>(get_initial_state());
    state->metric_value->value = value;
    update_state(state);
  }
};

static volatile std::atomic_bool keep_running = true;
static std::condition_variable cv_running;
static std::mutex running_mutex;

void int_handler(int /*unused*/)
{
  keep_running = false;
  std::unique_lock<std::mutex> lock(running_mutex);
  cv_running.notify_all();
}

int main()
{
  Log::set_log_level(LogLevel::INFO);
  LOG(LogLevel::INFO, "Starting up....");

  auto micro_sdc = std::make_unique<MicroSDC>();

  const auto sdc_port = 8080;
  const auto default_address = NetworkInterface::find_default_interface().address();
  LOG(LogLevel::INFO, "Setting local ip address " << default_address);
  micro_sdc->set_network_config(std::make_unique<NetworkConfig>(
      true, default_address, sdc_port, NetworkConfig::DiscoveryProxyProtocol::HTTPS,
      "https://10.52.219.176:3703"));

  DeviceCharacteristics device_characteristics;
  device_characteristics.set_friendly_name("MicroSDC on Linux");
  device_characteristics.set_manufacturer("Draeger");
  device_characteristics.set_model_name("MicroSDC_Device01");
  micro_sdc->set_device_characteristics(device_characteristics);

  micro_sdc->set_endpoint_reference("urn:uuid:MicroSDC-provider-on-linux");

  // Construct MdDescription
  BICEPS::PM::SystemContextDescriptor system_context("system_context");
  system_context.patient_context = BICEPS::PM::PatientContextDescriptor("patient_context");
  system_context.location_context = BICEPS::PM::LocationContextDescriptor("location_context");

  BICEPS::PM::MdsDescriptor device_descriptor("MedicalDevices");
  device_descriptor.system_context = system_context;

  // Dummy numeric state
  auto numeric_descriptor = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "numeric_state_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::MSRMT,
      BICEPS::PM::MetricAvailability::CONT, 1);
  numeric_descriptor->type = BICEPS::PM::CodedValue{"196074"};
  numeric_descriptor->type->concept_description =
      BICEPS::PM::LocalizedText{"dummy dynamic numeric metric"};
  // settable numeric
  auto settable_descriptor = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "settable_state_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::SET,
      BICEPS::PM::MetricAvailability::CONT, 1);
  // dummy string tate
  auto string_descriptor = std::make_shared<BICEPS::PM::StringMetricDescriptor>(
      "string_state_handle", BICEPS::PM::CodedValue("262656"), BICEPS::PM::MetricCategory::SET,
      BICEPS::PM::MetricAvailability::CONT);
  // dummy enum string tate
  BICEPS::PM::EnumStringMetricDescriptor::AllowedValueSequence allowed_value;
  auto& on = allowed_value.emplace_back("ON");
  on.type = BICEPS::PM::AllowedValue::TypeType("192834");
  on.type->concept_description = BICEPS::PM::LocalizedText{"On"};
  auto& off = allowed_value.emplace_back("OFF");
  off.type = BICEPS::PM::AllowedValue::TypeType("192835");
  off.type->concept_description = BICEPS::PM::LocalizedText{"Off"};
  auto enum_string_descriptor = std::make_shared<BICEPS::PM::EnumStringMetricDescriptor>(
      "enum_string_state_handle", BICEPS::PM::CodedValue("262656"), BICEPS::PM::MetricCategory::SET,
      BICEPS::PM::MetricAvailability::CONT, std::move(allowed_value));

  BICEPS::PM::ChannelDescriptor device_channel("device_channel");
  device_channel.metric.emplace_back(numeric_descriptor);
  device_channel.metric.emplace_back(settable_descriptor);
  device_channel.metric.emplace_back(string_descriptor);
  device_channel.metric.emplace_back(enum_string_descriptor);
  device_channel.safety_classification = BICEPS::PM::SafetyClassification::MED_A;
  device_channel.type = BICEPS::PM::CodedValue{"130537"};
  device_channel.type->concept_description = BICEPS::PM::LocalizedText{"dynamic not settable metrics"};

  BICEPS::PM::ScoDescriptor device_sco("sco_handle");
  auto set_value_operation = std::make_shared<BICEPS::PM::SetValueOperationDescriptor>(
      "set_value_operation_handle", "settable_state_handle");
  auto set_string_operation = std::make_shared<BICEPS::PM::SetStringOperationDescriptor>(
      "set_string_operation_handle", "string_state_handle");
  device_sco.operation.emplace_back(set_value_operation);
  device_sco.operation.emplace_back(set_string_operation);

  BICEPS::PM::VmdDescriptor device_module("device_vmd");
  device_module.type = BICEPS::PM::CodedValue{"130536"};
  device_module.type->concept_description = BICEPS::PM::LocalizedText{"not settable metrics"};
  device_module.channel.emplace_back(device_channel);
  device_module.sco = device_sco;

  device_descriptor.vmd.emplace_back(device_module);

  BICEPS::PM::MdDescription md_description;
  md_description.mds.emplace_back(device_descriptor);

  micro_sdc->set_md_description(md_description);

  // set location detail
  BICEPS::PM::LocationDetail location_detail;
  location_detail.poc = "PoC-A";
  location_detail.room = "Room-A";
  location_detail.bed = "Bed-A";
  location_detail.facility = "Facility-A";
  location_detail.building = "Building-A";
  location_detail.floor = "Floor-A";
  micro_sdc->set_location("location_context", location_detail);

  auto numeric_state_handler = std::make_shared<NumericStateHandler>("numeric_state_handle");
  auto settable_state_handler = std::make_shared<NumericStateHandler>("settable_state_handle");
  auto string_state_handler = std::make_shared<StringStateHandler>("string_state_handle");
  auto enum_string_state_handler =
      std::make_shared<EnumStringStateHandler>("enum_string_state_handle");
  micro_sdc->add_md_state(numeric_state_handler);
  micro_sdc->add_md_state(settable_state_handler);
  micro_sdc->add_md_state(string_state_handler);
  micro_sdc->add_md_state(enum_string_state_handler);

  micro_sdc->start();

  auto value_thread = std::thread([numeric_state_handler]() {
    double i = 0.0;
    while (keep_running)
    {
      // numeric_state_handler->set_value(i++);
      std::unique_lock<std::mutex> lock(running_mutex);
      cv_running.wait_for(lock, std::chrono::seconds(1));
    }
  });

  struct sigaction sa
  {
  };
  sa.sa_handler = &int_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);

  sigset_t mask;
  sigemptyset(&mask);
  while (keep_running)
  {
    sigsuspend(&mask);
  }
  value_thread.join();
  return 0;
}
