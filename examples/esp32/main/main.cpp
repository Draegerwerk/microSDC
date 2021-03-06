#include "BME280.hpp"
#include "DeviceCharacteristics.hpp"
#include "MicroSDC.hpp"
#include "StateHandler.hpp"
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

#ifndef CONFIG_WIFI_SSID
#define CONFIG_WIFI_SSID                                                                           \
  {                                                                                                \
  }
#endif
#ifndef CONFIG_WIFI_PASSWORD
#define CONFIG_WIFI_PASSWORD                                                                       \
  {                                                                                                \
  }
#endif

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
      // startup MicroSDC
      auto* sdc = static_cast<MicroSDC*>(arg);
      constexpr auto sdcPort = 443;
      sdc->setNetworkConfig(std::make_unique<NetworkConfig>(true, ipAddress, sdcPort));
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
  std::uint8_t macAddress[6] = {0};
  // we can get the ethernet driver handle from event data
  esp_eth_handle_t ethHandle = *static_cast<esp_eth_handle_t*>(eventData);

  switch (eventId)
  {
    case ETHERNET_EVENT_CONNECTED: {
      esp_eth_ioctl(ethHandle, ETH_CMD_G_MAC_ADDR, macAddress);
      LOG(LogLevel::INFO, "Ethernet Link Up");
      LOG(LogLevel::INFO, "Ethernet HW Addr " << std::hex
                                              << static_cast<unsigned int>(macAddress[0]) << ":"
                                              << static_cast<unsigned int>(macAddress[1]) << ":"
                                              << static_cast<unsigned int>(macAddress[2]) << ":"
                                              << static_cast<unsigned int>(macAddress[3]) << ":"
                                              << static_cast<unsigned int>(macAddress[4]) << ":"
                                              << static_cast<unsigned int>(macAddress[5]));
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
                                  CONFIG_WIFI_SSID,     // ssid
                                  CONFIG_WIFI_PASSWORD, // password
                                  {},                   // scan_method
                                  {},                   // bssid_set
                                  {},                   // bssid
                                  {},                   // channel
                                  {},                   // listen_interval
                                  {},                   // sort_method
                                  {},                   // threshold
                                  {}                    // pmf_cfg
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

  esp_eth_mac_t* mac = esp_eth_mac_new_esp32(&macConfig);
  esp_eth_phy_t* phy = esp_eth_phy_new_ip101(&phyConfig);

  esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, phy);
  esp_eth_handle_t ethHandle = nullptr;
  ESP_ERROR_CHECK(esp_eth_driver_install(&config, &ethHandle));
  /* attach Ethernet driver to TCP/IP stack */
  ESP_ERROR_CHECK(esp_netif_attach(ethNetif, esp_eth_new_netif_glue(ethHandle)));
  /* start Ethernet driver state machine */
  ESP_ERROR_CHECK(esp_eth_start(ethHandle));

  /*
  // Static IP address
  ESP_ERROR_CHECK(esp_netif_dhcpc_stop(ethNetif));
  esp_netif_ip_info_t ipInfo{};
  esp_netif_set_ip4_addr(&ipInfo.ip, 10, 0, 0, 2);
  esp_netif_set_ip4_addr(&ipInfo.netmask, 255, 0, 0, 0);
  ESP_ERROR_CHECK(esp_netif_set_ip_info(ethNetif, &ipInfo));
  */
}

// force c linkage for app_main()
extern "C" void app_main()
{
  Log::setLogLevel(LogLevel::DEBUG);
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
  extern const unsigned char ca_crt_start[] asm("_binary_ca_crt_start");
  extern const unsigned char ca_crt_end[] asm("_binary_ca_crt_end");
  const std::size_t ca_cert_len = ca_crt_end - ca_crt_start;
  ESP_ERROR_CHECK(esp_tls_set_global_ca_store(ca_crt_start, ca_cert_len));

  // create MicroSDC instance
  auto* sdc = new MicroSDC();
  sdc->setEndpointReference("urn:uuid:MicroSDC-provider-on-esp32");

  DeviceCharacteristics deviceCharacteristics;
  deviceCharacteristics.setFriendlyName("MicroSDC on ESP32");
  deviceCharacteristics.setManufacturer("Draeger");
  deviceCharacteristics.setModelName("MicroSDC_Device01");
  sdc->setDeviceCharacteristics(deviceCharacteristics);

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
  sdc->setMdDescription(mdDescription);

  BICEPS::PM::LocationDetail locationDetail;
  locationDetail.poC = "PoC-A";
  locationDetail.room = "Room-A";
  locationDetail.bed = "Bed-A";
  locationDetail.facility = "Facility-A";
  locationDetail.building = "Building-A";
  locationDetail.floor = "Floor-A";
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
