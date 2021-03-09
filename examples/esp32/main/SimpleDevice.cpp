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
  state->metricValue = std::make_optional<BICEPS::PM::NumericMetricValue>(
      BICEPS::PM::MetricQuality{BICEPS::PM::MeasurementValidity::Vld});
  state->metricValue->value = 0;
  return state;
}


BICEPS::MM::InvocationState
SimpleDevice::NumericStateHandler::request_state_change(const BICEPS::MM::AbstractSet& /*set*/)
{
  return BICEPS::MM::InvocationState::Fail;
}

void SimpleDevice::NumericStateHandler::set_value(double value)
{
  auto state = dyn_cast<BICEPS::PM::NumericMetricState>(get_initial_state());
  state->metricValue->value = value;
  update_state(state);
}

SimpleDevice::SimpleDevice()
  : pressure_state_handler_{std::make_shared<NumericStateHandler>("pressureState_handle")}
  , temperature_state_handler_{std::make_shared<NumericStateHandler>("temperatureState_handle")}
  , humidity_state_handler_{std::make_shared<NumericStateHandler>("humidityState_handle")}
  , settable_state_handler_{std::make_shared<NumericStateHandler>("settableState_handle")}
{
  init();
}

void SimpleDevice::set_network_config(std::unique_ptr<NetworkConfig> network_config)
{
  sdc_.set_network_config(std::move(network_config));
}

void SimpleDevice::init()
{
  sdc_.set_endpoint_reference("urn:uuid:MicroSDC-provider-on-esp32");

  DeviceCharacteristics device_characteristics;
  device_characteristics.set_friendly_name("MicroSDC on ESP32");
  device_characteristics.set_manufacturer("Draeger");
  device_characteristics.set_model_name("MicroSDC_Device01");
  sdc_.set_device_characteristics(device_characteristics);

  BICEPS::PM::Metadata metadata;
  metadata.manufacturer.emplace_back("Draeger");
  metadata.modelName.emplace_back("MicroSDC_Device01");
  metadata.modelNumber.emplace("1");
  metadata.serialNumber.emplace_back("2345-6789");

  BICEPS::PM::SystemContextDescriptor system_context("system_context");
  system_context.patientContext = BICEPS::PM::PatientContextDescriptor("patient_context");
  system_context.locationContext = BICEPS::PM::LocationContextDescriptor("location_context");

  // States for measured values
  auto pressure_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "pressureState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  pressure_state->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  auto temperature_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "temperatureState_handle", BICEPS::PM::CodedValue("6048"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  temperature_state->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  auto humidity_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "humidityState_handle", BICEPS::PM::CodedValue("262688"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  humidity_state->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  // Dummy settable state
  auto settable_state = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "settableState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  settable_state->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::ChannelDescriptor device_channel("device_channel");
  device_channel.metric.emplace_back(pressure_state);
  device_channel.metric.emplace_back(temperature_state);
  device_channel.metric.emplace_back(humidity_state);
  device_channel.metric.emplace_back(settable_state);

  BICEPS::PM::ScoDescriptor device_sco("sco_handle");
  auto set_value_operation = std::make_shared<BICEPS::PM::SetValueOperationDescriptor>(
      "setValueOperation_handle", "settableState_handle");
  device_sco.operation.emplace_back(set_value_operation);

  device_channel.safetyClassification = BICEPS::PM::SafetyClassification::MedA;
  BICEPS::PM::VmdDescriptor device_module("device_vmd");
  device_module.channel.emplace_back(device_channel);
  device_module.sco = device_sco;

  BICEPS::PM::MdsDescriptor device_descriptor("MedicalDevices");
  device_descriptor.metaData = metadata;
  device_descriptor.systemContext = system_context;
  device_descriptor.vmd.emplace_back(device_module);

  BICEPS::PM::MdDescription md_description;
  md_description.mds.emplace_back(device_descriptor);
  sdc_.set_md_description(md_description);

  BICEPS::PM::LocationDetail location_detail;
  location_detail.poC = "PoC-A";
  location_detail.room = "Room-A";
  location_detail.bed = "Bed-A";
  location_detail.facility = "Facility-A";
  location_detail.building = "Building-A";
  location_detail.floor = "Floor-A";
  sdc_.set_location("location_context", location_detail);

  sdc_.add_md_state(pressure_state_handler_);
  sdc_.add_md_state(temperature_state_handler_);
  sdc_.add_md_state(humidity_state_handler_);
  sdc_.add_md_state(settable_state_handler_);
}

void SimpleDevice::start_sdc()
{
  sdc_.start();
}

void SimpleDevice::run()
{
  const auto device_address = 0x76U;
  const auto sda_pin = GPIO_NUM_13;
  const auto scl_pin = GPIO_NUM_16;
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
