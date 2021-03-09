#include "SimpleDevice.hpp"
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

static void ip_event_handler(void* arg, esp_event_base_t /*event_base*/, int32_t event_id,
                             void* event_data)
{
  switch (event_id)
  {
    case IP_EVENT_ETH_GOT_IP:
      [[fallthrough]];
    case IP_EVENT_STA_GOT_IP: {
      // collect ip address information
      auto* event = static_cast<ip_event_got_ip_t*>(event_data);
      std::string ip_address;
      ip_address += std::to_string(esp_ip4_addr1_16(&event->ip_info.ip));
      ip_address += ".";
      ip_address += std::to_string(esp_ip4_addr2_16(&event->ip_info.ip));
      ip_address += ".";
      ip_address += std::to_string(esp_ip4_addr3_16(&event->ip_info.ip));
      ip_address += ".";
      ip_address += std::to_string(esp_ip4_addr4_16(&event->ip_info.ip));
      // startup MicroSDC
      auto* device = static_cast<SimpleDevice*>(arg);
      constexpr auto sdc_port = 443;
      device->set_network_config(std::make_unique<NetworkConfig>(true, ip_address, sdc_port));
      device->start_sdc();
      break;
    }
    default:
      break;
  }
}

static void wifi_event_handler(void* arg, esp_event_base_t /*event_base*/, int32_t event_id,
                               void* /*event_data*/)
{
  switch (event_id)
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

static void eth_event_handler(void* arg, esp_event_base_t /*event_base*/, int32_t event_id,
                              void* event_data)
{
  // we can get the ethernet driver handle from event data
  esp_eth_handle_t eth_handle = *static_cast<esp_eth_handle_t*>(event_data);

  switch (event_id)
  {
    case ETHERNET_EVENT_CONNECTED: {
      // std::uint8_t mac_address[6] = {0};
      const auto mac_addr_num_bytes = 6;
      std::array<std::uint8_t, mac_addr_num_bytes> mac_address{0};
      esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_address.data());
      LOG(LogLevel::INFO, "Ethernet Link Up");
      LOG(LogLevel::INFO, "Ethernet HW Addr " << std::hex
                                              << static_cast<unsigned int>(mac_address[0]) << ":"
                                              << static_cast<unsigned int>(mac_address[1]) << ":"
                                              << static_cast<unsigned int>(mac_address[2]) << ":"
                                              << static_cast<unsigned int>(mac_address[3]) << ":"
                                              << static_cast<unsigned int>(mac_address[4]) << ":"
                                              << static_cast<unsigned int>(mac_address[5]));
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

void init_wifi()
{
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  wifi_config_t wifi_config = {.sta = {
                                   CONFIG_WIFI_SSID,     // ssid
                                   CONFIG_WIFI_PASSWORD, // password
                                   {},                   // scan_method
                                   {},                   // bssid_set
                                   {},                   // bssid
                                   {},                   // channel
                                   {},                   // listen_interval
                                   {},                   // sort_method
                                   {},                   // threshold
                                   {},                   // pmf_cfg
                                   {},                   // rm_enabled
                                   {},                   // btm_enabled
                                   {},                   // reserved
                               }};
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}

void init_ethernet()
{
  esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
  esp_netif_t* eth_netif = esp_netif_new(&cfg);

  // Set default handlers to process TCP/IP stuffs
  ESP_ERROR_CHECK(esp_eth_set_default_handlers(eth_netif));

  eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
  eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();

  esp_eth_mac_t* mac = esp_eth_mac_new_esp32(&mac_config);
  // esp_eth_phy_t* phy = esp_eth_phy_new_ip101(&phyConfig);
  esp_eth_phy_t* phy = esp_eth_phy_new_lan8720(&phy_config);

  esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, phy);
  esp_eth_handle_t eth_handle = nullptr;
  ESP_ERROR_CHECK(esp_eth_driver_install(&config, &eth_handle));
  /* attach Ethernet driver to TCP/IP stack */
  ESP_ERROR_CHECK(esp_netif_attach(eth_netif, esp_eth_new_netif_glue(eth_handle)));
  /* start Ethernet driver state machine */
  ESP_ERROR_CHECK(esp_eth_start(eth_handle));

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
  Log::set_log_level(LogLevel::INFO);
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
  auto* device = new SimpleDevice();

  ESP_ERROR_CHECK(
      esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, device));
  ESP_ERROR_CHECK(
      esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, device));
  ESP_ERROR_CHECK(
      esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, device));
  LOG(LogLevel::INFO, "Connecting...");
#if CONFIG_CONNECT_ETHERNET
  init_ethernet();
#elif CONFIG_CONNECT_WIFI
  init_wifi();
#endif

  device->run();

  vTaskDelete(nullptr);
}
