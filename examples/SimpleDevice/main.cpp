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
      return BICEPS::MM::InvocationState::FAIL;
    }
    this->set_value(set_value->requested_numeric_value);
    return BICEPS::MM::InvocationState::FIN;
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
      return BICEPS::MM::InvocationState::FAIL;
    }
    this->set_value(set_string->requested_string_value);
    return BICEPS::MM::InvocationState::FIN;
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
      return BICEPS::MM::InvocationState::FAIL;
    }
    this->set_value(set_string->requested_string_value);
    return BICEPS::MM::InvocationState::FIN;
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
  Log::set_log_level(LogLevel::DEBUG);
  LOG(LogLevel::INFO, "Starting up....");

  auto sdc = std::make_shared<MicroSDC>();

  const auto sdc_port = 8080;
  const auto default_address = NetworkInterface::find_default_interface().address();
  LOG(LogLevel::INFO, "Setting local ip address " << default_address);
  sdc->set_network_config(std::make_unique<NetworkConfig>(
      true, default_address, sdc_port, NetworkConfig::DiscoveryProxyProtocol::HTTPS,
      "https://10.52.219.176:3703"));

  sdc->set_endpoint_reference("urn:uuid:a1337d00-d306-4c2f-a981-7f15a4ec2022");

  DeviceCharacteristics device_characteristics;
  device_characteristics.set_friendly_name("MicroSDC on Linux");
  device_characteristics.set_manufacturer("Draeger");
  device_characteristics.set_model_name("MicroSDC_Device02");
  sdc->set_device_characteristics(device_characteristics);

  BICEPS::PM::Metadata metadata;
  metadata.manufacturer.emplace_back("Draeger");
  metadata.model_name.emplace_back("MicroSDC_Device01");
  metadata.model_number.emplace("1");
  metadata.serial_number.emplace_back("2345-6789");

  BICEPS::PM::SystemContextDescriptor system_context("system_context");
  system_context.patient_context = BICEPS::PM::PatientContextDescriptor("patient_context");
  system_context.location_context = BICEPS::PM::LocationContextDescriptor("location_context");

  // States for measured values
  auto pressure_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "pressureState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::MSRMT,
      BICEPS::PM::MetricAvailability::CONT, 1);
  pressure_state->unit.concept_description = BICEPS::PM::LocalizedText{"Pa"};
  pressure_state->safety_classification = BICEPS::PM::SafetyClassification::MED_A;
  pressure_state->type = BICEPS::PM::CodedValue{"152836"};
  pressure_state->type->concept_description = BICEPS::PM::LocalizedText{"Air pressure"};

  auto temperature_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "temperatureState_handle", BICEPS::PM::CodedValue("6048"), BICEPS::PM::MetricCategory::MSRMT,
      BICEPS::PM::MetricAvailability::CONT, 1);
  temperature_state->unit.concept_description = BICEPS::PM::LocalizedText{"°C"};
  temperature_state->safety_classification = BICEPS::PM::SafetyClassification::MED_A;
  temperature_state->type = BICEPS::PM::CodedValue{"184296"};
  temperature_state->type->concept_description = BICEPS::PM::LocalizedText{"Temperature"};

  auto humidity_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "humidityState_handle", BICEPS::PM::CodedValue("262688"), BICEPS::PM::MetricCategory::MSRMT,
      BICEPS::PM::MetricAvailability::CONT, 1);
  humidity_state->unit.concept_description = BICEPS::PM::LocalizedText{"%"};
  humidity_state->safety_classification = BICEPS::PM::SafetyClassification::MED_A;
  humidity_state->type = BICEPS::PM::CodedValue{"184292"};
  humidity_state->type->concept_description = BICEPS::PM::LocalizedText{"Humidity"};

  BICEPS::PM::ChannelDescriptor device_channel("device_channel");
  device_channel.type = BICEPS::PM::CodedValue{"128771"};
  device_channel.type->concept_description =
      BICEPS::PM::LocalizedText{"dynamic not settable metrics"};
  device_channel.metric.emplace_back(pressure_state);
  device_channel.metric.emplace_back(temperature_state);
  device_channel.metric.emplace_back(humidity_state);

  BICEPS::PM::ScoDescriptor device_sco("sco_handle");

  device_channel.safety_classification = BICEPS::PM::SafetyClassification::MED_A;
  BICEPS::PM::VmdDescriptor device_module("device_vmd");
  device_module.type = BICEPS::PM::CodedValue{"128770"};
  device_module.type->concept_description = BICEPS::PM::LocalizedText{"not settable metrics"};
  device_module.channel.emplace_back(device_channel);
  device_module.sco = device_sco;

  BICEPS::PM::MdsDescriptor device_descriptor("MedicalDevices");
  device_descriptor.meta_data = metadata;
  device_descriptor.system_context = system_context;
  device_descriptor.vmd.emplace_back(device_module);
  device_descriptor.type = BICEPS::PM::CodedValue{"70825"};
  device_descriptor.type->concept_description = BICEPS::PM::LocalizedText{"Mds"};

  BICEPS::PM::MdDescription md_description;
  md_description.mds.emplace_back(device_descriptor);
  sdc->set_md_description(md_description);

  BICEPS::PM::LocationDetail location_detail;
  location_detail.poc = "SC8";
  location_detail.bed = "tam";
  location_detail.facility = "DRAEGER";
  sdc->set_location("location_context", "location_context_state01", location_detail);

  auto pressure_state_handler{std::make_shared<NumericStateHandler>("pressureState_handle")};
  auto temperature_state_handler{std::make_shared<NumericStateHandler>("temperatureState_handle")};
  auto humidity_state_handler{std::make_shared<NumericStateHandler>("humidityState_handle")};

  sdc->add_md_state(pressure_state_handler);
  sdc->add_md_state(temperature_state_handler);
  sdc->add_md_state(humidity_state_handler);

  sdc->start();

  auto value_thread = std::thread([sdc, device_descriptor, pressure_state_handler,
                                   temperature_state_handler, humidity_state_handler]() {
    double i = 0.0;
    while (keep_running)
    {
      pressure_state_handler->set_value(i++);
      // temperature_state_handler->set_value(2 * i);
      // humidity_state_handler->set_value(3 * i);
      // sdc->update_state(std::make_shared<BICEPS::PM::MdsState>(device_descriptor.handle));
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
