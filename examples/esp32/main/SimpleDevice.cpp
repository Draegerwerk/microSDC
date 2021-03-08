#include "SimpleDevice.hpp"
#include "BME280.hpp"

SimpleDevice::NumericStateHandler::NumericStateHandler(const std::string& descriptorHandle)
  : StateHandler(descriptorHandle)
{
}

std::shared_ptr<BICEPS::PM::AbstractState>
SimpleDevice::NumericStateHandler::getInitialState() const
{
  auto state = std::make_shared<BICEPS::PM::NumericMetricState>(getDescriptorHandle());
  state->metricValue = std::make_optional<BICEPS::PM::NumericMetricValue>(
      BICEPS::PM::MetricQuality{BICEPS::PM::MeasurementValidity::Vld});
  state->metricValue->value = 0;
  return state;
}

void SimpleDevice::NumericStateHandler::setValue(double value)
{
  auto state = dyn_cast<BICEPS::PM::NumericMetricState>(getInitialState());
  state->metricValue->value = value;
  updateState(state);
}

SimpleDevice::SimpleDevice()
  : pressureStateHandler_{std::make_shared<NumericStateHandler>("pressureState_handle")}
  , temperatureStateHandler_{std::make_shared<NumericStateHandler>("temperatureState_handle")}
  , humidityStateHandler_{std::make_shared<NumericStateHandler>("humidityState_handle")}
  , settableStateHandler_{std::make_shared<NumericStateHandler>("settableState_handle")}
{
  init();
}

void SimpleDevice::setNetworkConfig(std::unique_ptr<NetworkConfig> networkConfig)
{
  sdc_.setNetworkConfig(std::move(networkConfig));
}

void SimpleDevice::init()
{
  sdc_.setEndpointReference("urn:uuid:MicroSDC-provider-on-esp32");

  DeviceCharacteristics deviceCharacteristics;
  deviceCharacteristics.setFriendlyName("MicroSDC on ESP32");
  deviceCharacteristics.setManufacturer("Draeger");
  deviceCharacteristics.setModelName("MicroSDC_Device01");
  sdc_.setDeviceCharacteristics(deviceCharacteristics);

  BICEPS::PM::Metadata metadata;
  metadata.manufacturer.emplace_back("Draeger");
  metadata.modelName.emplace_back("MicroSDC_Device01");
  metadata.modelNumber.emplace("1");
  metadata.serialNumber.emplace_back("2345-6789");

  BICEPS::PM::SystemContextDescriptor systemContext("system_context");
  systemContext.patientContext = BICEPS::PM::PatientContextDescriptor("patient_context");
  systemContext.locationContext = BICEPS::PM::LocationContextDescriptor("location_context");

  // States for measured values
  auto pressureState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "pressureState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  pressureState->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  auto temperatureState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "temperatureState_handle", BICEPS::PM::CodedValue("6048"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  temperatureState->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  auto humidityState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "humidityState_handle", BICEPS::PM::CodedValue("262688"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  humidityState->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  // Dummy settable state
  auto settableState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "settableState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  settableState->safetyClassification = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::ChannelDescriptor deviceChannel("device_channel");
  deviceChannel.metric.emplace_back(pressureState);
  deviceChannel.metric.emplace_back(temperatureState);
  deviceChannel.metric.emplace_back(humidityState);
  deviceChannel.metric.emplace_back(settableState);

  BICEPS::PM::ScoDescriptor deviceSco("sco_handle");
  auto setValueOperation = std::make_shared<BICEPS::PM::SetValueOperationDescriptor>(
      "setValueOperation_handle", "settableState_handle");
  deviceSco.operation.emplace_back(setValueOperation);

  deviceChannel.safetyClassification = BICEPS::PM::SafetyClassification::MedA;
  BICEPS::PM::VmdDescriptor deviceModule("device_vmd");
  deviceModule.channel.emplace_back(deviceChannel);
  deviceModule.sco = deviceSco;

  BICEPS::PM::MdsDescriptor deviceDescriptor("MedicalDevices");
  deviceDescriptor.metaData = metadata;
  deviceDescriptor.systemContext = systemContext;
  deviceDescriptor.vmd.emplace_back(deviceModule);

  BICEPS::PM::MdDescription mdDescription;
  mdDescription.mds.emplace_back(deviceDescriptor);
  sdc_.setMdDescription(mdDescription);

  BICEPS::PM::LocationDetail locationDetail;
  locationDetail.poC = "PoC-A";
  locationDetail.room = "Room-A";
  locationDetail.bed = "Bed-A";
  locationDetail.facility = "Facility-A";
  locationDetail.building = "Building-A";
  locationDetail.floor = "Floor-A";
  sdc_.setLocation("location_context", locationDetail);

  sdc_.addMdState(pressureStateHandler_);
  sdc_.addMdState(temperatureStateHandler_);
  sdc_.addMdState(humidityStateHandler_);
  sdc_.addMdState(settableStateHandler_);
}

void SimpleDevice::startSDC()
{
  sdc_.start();
}

void SimpleDevice::run()
{
  const auto deviceAddress = 0x76U;
  const auto sdaPin = GPIO_NUM_13;
  const auto sclPin = GPIO_NUM_16;
  BME280 bme280(I2C_NUM_0, deviceAddress, sdaPin, sclPin);
  while (true)
  {
    const auto sensorData = bme280.getSensorData();
    LOG(LogLevel::INFO, "pressure: " << sensorData.pressure << " temp: " << sensorData.temperature
                                     << " humidity: " << sensorData.humidity);
    pressureStateHandler_->setValue(sensorData.pressure);
    temperatureStateHandler_->setValue(sensorData.temperature);
    humidityStateHandler_->setValue(sensorData.humidity);
    const auto delay = 2000;
    vTaskDelay(delay / portTICK_PERIOD_MS);
  }
}
