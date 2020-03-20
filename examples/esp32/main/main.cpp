#include "BME280.hpp"
#include "DeviceCharacteristics.hpp"
#include "MicroSDC.hpp"
#include "SessionManager.hpp"
#include "StateHandler.hpp"
#include "WebServer.hpp"
#include "networking/NetworkConfig.hpp"
#include <chrono>
#include <pthread.h>
#include <sstream>
#include <thread>

#include "esp_eth.h"
#include "esp_system.h"
#include "esp_tls.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

static constexpr const char* TAG = "main_component";

static void ipEventHandler(void* arg, esp_event_base_t /*event_base*/, int32_t eventId,
                           void* eventData)
{
  switch (eventId)
  {
    case IP_EVENT_ETH_GOT_IP:
      [[fallthrough]];
    case IP_EVENT_STA_GOT_IP: {
      // collect ip address information
      auto* event = static_cast<ip_event_got_ip_t*>(eventData);
      std::string ipAddress;
      ipAddress += std::to_string(esp_ip4_addr1_16(&event->ip_info.ip));
      ipAddress += ".";
      ipAddress += std::to_string(esp_ip4_addr2_16(&event->ip_info.ip));
      ipAddress += ".";
      ipAddress += std::to_string(esp_ip4_addr3_16(&event->ip_info.ip));
      ipAddress += ".";
      ipAddress += std::to_string(esp_ip4_addr4_16(&event->ip_info.ip));
      auto networkConfig = std::make_shared<NetworkConfig>(true, ipAddress);
      // startup MicroSDC
      auto* sdc = static_cast<MicroSDC*>(arg);
      sdc->setNetworkConfig(networkConfig);
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
      LOG(LogLevel::INFO, "WiFi ready");
      break;
    case WIFI_EVENT_SCAN_DONE:
      LOG(LogLevel::INFO, "WiFi scan done");
      break;
    case WIFI_EVENT_STA_START:
      LOG(LogLevel::INFO, "WiFi started. Connecting...");
      esp_wifi_connect();
      break;
    case WIFI_EVENT_STA_STOP:
      LOG(LogLevel::INFO, "WiFi stop");
      break;
    case WIFI_EVENT_STA_CONNECTED:
      LOG(LogLevel::INFO, "WiFi connected");
      break;
    case WIFI_EVENT_STA_DISCONNECTED: {
      LOG(LogLevel::INFO, "WiFi disconnected");
      auto* sdc = static_cast<MicroSDC*>(arg);
      sdc->stop();
      LOG(LogLevel::INFO, "Trying to reconnect...");
      esp_wifi_connect();
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
      LOG(LogLevel::INFO, "Ethernet Link Up");
      LOG(LogLevel::INFO, "Ethernet HW Addr " << std::hex << macAddress[0] << ":" << macAddress[1]
                                              << ":" << macAddress[2] << ":" << macAddress[3] << ":"
                                              << macAddress[4] << ":" << macAddress[5]);
      break;
    }
    case ETHERNET_EVENT_DISCONNECTED: {
      LOG(LogLevel::INFO, "Ethernet Link Down");
      auto* sdc = static_cast<MicroSDC*>(arg);
      sdc->stop();
      break;
    }
    case ETHERNET_EVENT_START:
      LOG(LogLevel::INFO, "Ethernet Started");
      break;
    case ETHERNET_EVENT_STOP:
      LOG(LogLevel::INFO, "Ethernet Stopped");
      break;
    default:
      break;
  }
}

void initWifi()
{
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  wifi_config_t wifiConfig = {.sta = {
                                  CONFIG_WIFI_SSID,          // ssid
                                  CONFIG_WIFI_PASSWORD,      // password
                                  WIFI_FAST_SCAN,            // scan_method
                                  false,                     // bssid_set
                                  {},                        // bssid
                                  0,                         // channel
                                  0,                         // listen_interval
                                  WIFI_CONNECT_AP_BY_SIGNAL, // sort_method
                                  {}                         // threshold
                              }};
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig));
  ESP_ERROR_CHECK(esp_wifi_start());
}

void initEthernet()
{
  esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
  esp_netif_t* ethNetif = esp_netif_new(&cfg);
  // Set default handlers to process TCP/IP stuffs
  ESP_ERROR_CHECK(esp_eth_set_default_handlers(ethNetif));

  eth_mac_config_t macConfig = ETH_MAC_DEFAULT_CONFIG();
  eth_phy_config_t phyConfig = ETH_PHY_DEFAULT_CONFIG();

  // ESP OLIMEX EVB specific phy address
  phyConfig.phy_addr = 0;

  esp_eth_mac_t* mac = esp_eth_mac_new_esp32(&macConfig);
  esp_eth_phy_t* phy = esp_eth_phy_new_lan8720(&phyConfig);

  esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, phy);
  esp_eth_handle_t ethHandle = nullptr;
  ESP_ERROR_CHECK(esp_eth_driver_install(&config, &ethHandle));
  /* attach Ethernet driver to TCP/IP stack */
  ESP_ERROR_CHECK(esp_netif_attach(ethNetif, esp_eth_new_netif_glue(ethHandle)));
  /* start Ethernet driver state machine */
  ESP_ERROR_CHECK(esp_eth_start(ethHandle));
}

// force c linkage for app_main()
extern "C" void app_main()
{
  Log::setLogLevel(LogLevel::INFO);
  LOG(LogLevel::INFO, "Starting up....");

  // Initialize NVS
  LOG(LogLevel::INFO, "NVS Flash init...");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  // Initialize esp_netif component
  ESP_ERROR_CHECK(esp_netif_init());
  // create default loop for eventing
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // initialize global ca store for client communication
  ESP_ERROR_CHECK(esp_tls_init_global_ca_store());
  extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
  extern const unsigned char cacert_pem_end[] asm("_binary_cacert_pem_end");
  const std::size_t cacert_len = cacert_pem_end - cacert_pem_start;
  ESP_ERROR_CHECK(esp_tls_set_global_ca_store(cacert_pem_start, cacert_len));

  // create MicroSDC instance
  auto* sdc = new MicroSDC(std::make_shared<WebServer>(true), std::make_shared<SessionManager>());
  sdc->setEndpointReference("urn:uuid:MicroSDC-provider-on-esp32");

  DeviceCharacteristics deviceCharacteristics;
  deviceCharacteristics.setFriendlyName("MicroSDC on ESP32");
  deviceCharacteristics.setManufacturer("Draeger");
  deviceCharacteristics.setModelName("MicroSDC_Device01");
  sdc->setDeviceCharacteristics(deviceCharacteristics);

  BICEPS::PM::Metadata metadata;
  metadata.Manufacturer().emplace_back("Draeger");
  metadata.ModelName().emplace_back("MicroSDC_Device01");
  metadata.ModelNumber().emplace("1");
  metadata.SerialNumber().emplace_back("2345-6789");

  BICEPS::PM::SystemContextDescriptor systemContext("system_context");
  systemContext.PatientContext() = BICEPS::PM::PatientContextDescriptor("patient_context");
  systemContext.LocationContext() = BICEPS::PM::LocationContextDescriptor("location_context");

  // States for measured values
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

  // Dummy settable state
  auto settableState = std::make_shared<BICEPS::PM::NumericMetricDescriptor>(
      "settableState_handle", BICEPS::PM::CodedValue("3840"), BICEPS::PM::MetricCategory::Msrmt,
      BICEPS::PM::MetricAvailability::Cont, 1);
  settableState->SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;

  BICEPS::PM::ChannelDescriptor deviceChannel("device_channel");
  deviceChannel.Metric().emplace_back(pressureState);
  deviceChannel.Metric().emplace_back(temperatureState);
  deviceChannel.Metric().emplace_back(humidityState);
  deviceChannel.Metric().emplace_back(settableState);

  BICEPS::PM::ScoDescriptor deviceSco("sco_handle");
  auto setValueOperation = std::make_shared<BICEPS::PM::SetValueOperationDescriptor>(
      "setValueOperation_handle", "settableState_handle");
  deviceSco.Operation().emplace_back(setValueOperation);

  deviceChannel.SafetyClassification() = BICEPS::PM::SafetyClassification::MedA;
  BICEPS::PM::VmdDescriptor deviceModule("device_vmd");
  deviceModule.Channel().emplace_back(deviceChannel);
  deviceModule.Sco() = deviceSco;

  BICEPS::PM::MdsDescriptor deviceDescriptor("MedicalDevices");
  deviceDescriptor.MetaData() = metadata;
  deviceDescriptor.SystemContext() = systemContext;
  deviceDescriptor.Vmd().emplace_back(deviceModule);

  BICEPS::PM::MdDescription mdDescription;
  mdDescription.Mds().emplace_back(deviceDescriptor);
  sdc->setMdDescription(mdDescription);

  BICEPS::PM::LocationDetailType locationDetail;
  locationDetail.PoC() = "PoC-A";
  locationDetail.Room() = "Room-A";
  locationDetail.Bed() = "Bed-A";
  locationDetail.Facility() = "Facility-A";
  locationDetail.Building() = "Building-A";
  locationDetail.Floor() = "Floor-A";
  sdc->setLocation("location_context", locationDetail);

  auto pressureStateHandler = std::make_shared<NumericStateHandler>("pressureState_handle");
  auto temperatureStateHandler = std::make_shared<NumericStateHandler>("temperatureState_handle");
  auto humidityStateHandler = std::make_shared<NumericStateHandler>("humidityState_handle");
  auto settableStateHandler = std::make_shared<NumericStateHandler>("settableState_handle");
  sdc->addMdState(pressureStateHandler);
  sdc->addMdState(temperatureStateHandler);
  sdc->addMdState(humidityStateHandler);
  sdc->addMdState(settableStateHandler);

  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ipEventHandler, sdc));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, sdc));
  ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &ethEventHandler, sdc));
  LOG(LogLevel::INFO, "Connecting...");
#if CONFIG_CONNECT_ETHERNET
  initEthernet();
#elif CONFIG_CONNECT_WIFI
  initWifi();
#endif

  std::thread updateThread([=]() {
    BME280 bme280(I2C_NUM_0, 0x76u, static_cast<gpio_num_t>(13), static_cast<gpio_num_t>(16));
    while (true)
    {
      const auto sensorData = bme280.getSensorData();
      LOG(LogLevel::INFO, "pressure: " << sensorData.pressure << " temp: " << sensorData.temperature
                                       << " humidity: " << sensorData.humidity);
      pressureStateHandler->setValue(sensorData.pressure);
      temperatureStateHandler->setValue(sensorData.temperature);
      humidityStateHandler->setValue(sensorData.humidity);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  });
  updateThread.join();
  vTaskDelete(nullptr);
}
