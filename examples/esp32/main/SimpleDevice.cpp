#include "SimpleDevice.hpp"
#include "BME280.hpp"

SimpleDevice::NumericStateHandler::NumericStateHandler(const std::string& descriptor_handle)
  : StateHandler(descriptor_handle)
{
}

std::shared_ptr<BICEPS::PM::AbstractState>
SimpleDevice::NumericStateHandler::get_initial_state() const
{
  auto state = std::make_shared<BICEPS::PM::NumericMetricState>(get_descriptor_handle());
  state->metric_value = std::make_optional<BICEPS::PM::NumericMetricValue>(
      BICEPS::PM::MetricQuality{BICEPS::PM::MeasurementValidity::VLD});
  state->metric_value->value = 0;
  return state;
}


BICEPS::MM::InvocationState
SimpleDevice::NumericStateHandler::request_state_change(const BICEPS::MM::AbstractSet& set)
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

void SimpleDevice::NumericStateHandler::set_value(double value)
{
  auto state = dyn_cast<BICEPS::PM::NumericMetricState>(get_initial_state());
  state->metric_value->value = value;
  update_state(state);
}

SimpleDevice::SimpleDevice()
  : pressure_state_handler_{std::make_shared<NumericStateHandler>("pressureState_handle")}
  , temperature_state_handler_{std::make_shared<NumericStateHandler>("temperatureState_handle")}
  , humidity_state_handler_{std::make_shared<NumericStateHandler>("humidityState_handle")}
//, settable_state_handler_{std::make_shared<NumericStateHandler>("settableState_handle")}
{
  init();
}

void SimpleDevice::set_network_config(std::unique_ptr<NetworkConfig> network_config)
{
  sdc_.set_network_config(std::move(network_config));
}

void SimpleDevice::init()
{
  sdc_.set_endpoint_reference("urn:uuid:a1337d00-d306-4c2f-a981-7f15a4ec2022");

  DeviceCharacteristics device_characteristics;
  device_characteristics.set_friendly_name("MicroSDC on ESP32");
  device_characteristics.set_manufacturer("Draeger");
  device_characteristics.set_model_name("MicroSDC_Device01");
  sdc_.set_device_characteristics(device_characteristics);

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

  // Dummy settable state
  //  auto settable_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
  //      "settableState_handle", BICEPS::PM::CodedValue("262656"), BICEPS::PM::MetricCategory::Set,
  //      BICEPS::PM::MetricAvailability::CONT, 1);
  //  settable_state->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::ChannelDescriptor device_channel("device_channel");
  device_channel.type = BICEPS::PM::CodedValue{"128771"};
  device_channel.type->concept_description =
      BICEPS::PM::LocalizedText{"dynamic not settable metrics"};
  device_channel.metric.emplace_back(pressure_state);
  device_channel.metric.emplace_back(temperature_state);
  device_channel.metric.emplace_back(humidity_state);
  // device_channel.metric.emplace_back(settable_state);

  BICEPS::PM::ScoDescriptor device_sco("sco_handle");
  //  auto set_value_operation = std::make_shared<BICEPS::PM::SetValueOperationDescriptor>(
  //      "set_value_operation_handle", "settableState_handle");
  //  device_sco.operation.emplace_back(set_value_operation);

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
  sdc_.set_md_description(md_description);

  BICEPS::PM::LocationDetail location_detail;
  location_detail.poc = "SC8";
  location_detail.room = "Room-A";
  location_detail.bed = "tam";
  location_detail.facility = "DRAEGER";
  location_detail.building = "Building-A";
  location_detail.floor = "Floor-A";
  sdc_.set_location("location_context", location_detail);

  sdc_.add_md_state(pressure_state_handler_);
  sdc_.add_md_state(temperature_state_handler_);
  sdc_.add_md_state(humidity_state_handler_);
  // sdc_.add_md_state(settable_state_handler_);
}

void SimpleDevice::start_sdc()
{
  sdc_.start();
}

void SimpleDevice::run()
{
  const auto device_address = 0x76U;
  const auto sda_pin = GPIO_NUM_33;
  const auto scl_pin = GPIO_NUM_32;
  BME280 bme280(I2C_NUM_0, device_address, sda_pin, scl_pin);
  while (true)
  {
    const auto sensor_data = bme280.get_sensor_data();
    LOG(LogLevel::INFO, "pressure: " << sensor_data.pressure << " temp: " << sensor_data.temperature
                                     << " humidity: " << sensor_data.humidity);
    pressure_state_handler_->set_value(sensor_data.pressure);
    temperature_state_handler_->set_value(sensor_data.temperature);
    humidity_state_handler_->set_value(sensor_data.humidity);
    const auto delay = 2000;
    vTaskDelay(delay / portTICK_PERIOD_MS);
  }
}
