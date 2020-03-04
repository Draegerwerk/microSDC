#include "BME280.hpp"
#include "DeviceCharacteristics.hpp"
#include "MicroSDC.hpp"
#include "NetworkHandler.hpp"
#include "StateHandler.hpp"
#include "esp_eth.h"
#include "esp_log.h"
#include "esp_pthread.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <thread>

static constexpr const char* TAG = "main_component";

static void ipEventHandler(void* arg, esp_event_base_t /*event_base*/, int32_t eventId,
                           void* /*event_data*/)
{
  switch (eventId)
  {
    case IP_EVENT_ETH_GOT_IP:
      [[fallthrough]];
    case IP_EVENT_STA_GOT_IP: {
      auto sdc = reinterpret_cast<MicroSDC*>(arg);
      sdc->start();
      break;
    }
    default:
      break;
  }
}

static void wifiEventHandler(void* arg, esp_event_base_t /*event_base*/, int32_t eventId,
                             void* /*event_data*/)
{
  switch (eventId)
  {
    case WIFI_EVENT_WIFI_READY:
      ESP_LOGI(TAG, "WiFi ready");
      break;
    case WIFI_EVENT_SCAN_DONE:
      ESP_LOGI(TAG, "WiFi scan done");
      break;
    case WIFI_EVENT_STA_START:
      ESP_LOGI(TAG, "WiFi start");
      break;
    case WIFI_EVENT_STA_STOP:
      ESP_LOGI(TAG, "WiFi stop");
      break;
    case WIFI_EVENT_STA_CONNECTED:
      ESP_LOGI(TAG, "WiFi connected");
      break;
    case WIFI_EVENT_STA_DISCONNECTED: {
      ESP_LOGI(TAG, "WiFi disconnected");
      auto sdc = reinterpret_cast<MicroSDC*>(arg);
      sdc->stop();
      break;
    }
    default:
      break;
  }
}

static void ethEventHandler(void* arg, esp_event_base_t /*event_base*/, int32_t eventId,
                            void* eventData)
{
  uint8_t macAddress[6] = {0};
  // we can get the ethernet driver handle from event data
  esp_eth_handle_t ethHandle = *static_cast<esp_eth_handle_t*>(eventData);

  switch (eventId)
  {
    case ETHERNET_EVENT_CONNECTED: {
      esp_eth_ioctl(ethHandle, ETH_CMD_G_MAC_ADDR, macAddress);
      ESP_LOGI(TAG, "Ethernet Link Up");
      ESP_LOGI(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x", macAddress[0], macAddress[1],
               macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
      break;
    }
    case ETHERNET_EVENT_DISCONNECTED: {
      ESP_LOGI(TAG, "Ethernet Link Down");
      auto sdc = reinterpret_cast<MicroSDC*>(arg);
      sdc->stop();
      break;
    }
    case ETHERNET_EVENT_START:
      ESP_LOGI(TAG, "Ethernet Started");
      break;
    case ETHERNET_EVENT_STOP:
      ESP_LOGI(TAG, "Ethernet Stopped");
      break;
    default:
      break;
  }
}

class NumericStateHandler : public MdStateHandler<BICEPS::PM::NumericMetricState>
{
public:
  explicit NumericStateHandler(const std::string& descriptorHandle)
    : MdStateHandler(descriptorHandle)
  {
  }

  BICEPS::PM::MetricType getMetricType() const override
  {
    return BICEPS::PM::MetricType::NUMERIC;
  }

  std::shared_ptr<BICEPS::PM::NumericMetricState> getInitialState() const override
  {
    auto state = std::make_shared<BICEPS::PM::NumericMetricState>(getDescriptorHandle());
    BICEPS::PM::NumericMetricValue value;
    value.Value() = 0;
    state->MetricValue() = value;
    return state;
  }

  void setValue(int value)
  {
    auto state = getInitialState();
    state->MetricValue()->Value() = value;
    updateState(state);
  }
};

// force c linkage for app_main()
extern "C" void app_main()
{
  ESP_LOGI(TAG, "Starting up....");

  ESP_LOGI(TAG, "NVS Flash init...");
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_LOGI(TAG, "TCP adapter init...");
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  auto sdc = new MicroSDC();
  sdc->setEndpointReference("urn:uuid:MicroSDC-provider-on-esp32");
  sdc->setUseTLS(true);

  DeviceCharacteristics deviceCharacteristics;
  deviceCharacteristics.setFriendlyName("MicroSDC on ESP32");
  deviceCharacteristics.setManufacturer("Draeger");
  deviceCharacteristics.setModelName("MicroSDC_Device01");
  sdc->setDeviceCharacteristics(deviceCharacteristics);

  BICEPS::PM::Metadata metadata;
  metadata.Manufacturer().emplace_back("Draeger");
  metadata.ModelName().emplace_back("MicroSDC_Device01");
  metadata.ModelNumber().emplace("1");
  metadata.SerialNumber().emplace_back("1234-5678");

  BICEPS::PM::SystemContextDescriptor systemContext("system_context");
  systemContext.PatientContext() = BICEPS::PM::PatientContextDescriptor("patient_context");
  systemContext.LocationContext() = BICEPS::PM::LocationContextDescriptor("location_context");

  auto pressureState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "pressureState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  pressureState->SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;

  auto temperatureState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "temperatureState_handle", BICEPS::PM::CodedValue("6048"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  temperatureState->SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;

  auto humidityState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "humidityState_handle", BICEPS::PM::CodedValue("262688"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  humidityState->SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::ChannelDescriptor deviceChannel("device_channel");
  deviceChannel.Metric().emplace_back(pressureState);
  deviceChannel.Metric().emplace_back(temperatureState);
  deviceChannel.Metric().emplace_back(humidityState);

  deviceChannel.SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;
  BICEPS::PM::VmdDescriptor deviceModule("device_vmd");
  deviceModule.Channel().emplace_back(deviceChannel);

  BICEPS::PM::MdsDescriptor deviceDescriptor("MedicalDevices");
  deviceDescriptor.MetaData() = metadata;
  deviceDescriptor.SystemContext() = systemContext;
  deviceDescriptor.Vmd().emplace_back(deviceModule);

  BICEPS::PM::MdDescription mdDescription;
  mdDescription.Mds().emplace_back(deviceDescriptor);
  sdc->setMdDescription(mdDescription);

  auto pressureStateHandler = std::make_shared<NumericStateHandler>("pressureState_handle");
  auto temperatureStateHandler = std::make_shared<NumericStateHandler>("temperatureState_handle");
  auto humidityStateHandler = std::make_shared<NumericStateHandler>("humidityState_handle");
  sdc->addMdState(pressureStateHandler);
  sdc->addMdState(temperatureStateHandler);
  sdc->addMdState(humidityStateHandler);

  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ipEventHandler, sdc));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, sdc));
  ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &ethEventHandler, sdc));

  ESP_LOGI(TAG, "Connecting...");
  ESP_ERROR_CHECK(NetworkHandler::getInstance().connect());

  esp_pthread_cfg_t pthreadConf = esp_pthread_get_default_config();
  pthreadConf.stack_size = 8192;
  esp_pthread_set_cfg(&pthreadConf);
  std::thread updateThread([=]() {
    BME280 bme280(i2c_port_t::I2C_NUM_0, 0x76u, static_cast<gpio_num_t>(13),
                  static_cast<gpio_num_t>(16));
    while (true)
    {
      const auto sensorData = bme280.getSensorData();
      ESP_LOGI(TAG, "pressure: %0.2f, temp: %0.2f, humidity: %0.2f", sensorData.pressure,
               sensorData.temperature, sensorData.humidity);
      pressureStateHandler->setValue(static_cast<int>(sensorData.pressure));
      temperatureStateHandler->setValue(static_cast<int>(sensorData.temperature));
      humidityStateHandler->setValue(static_cast<int>(sensorData.humidity));
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  });
  updateThread.join();
  vTaskDelete(nullptr);
}
