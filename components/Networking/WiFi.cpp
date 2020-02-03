#include "WiFi.hpp"
#include "esp_log.h"
#include "esp_wifi.h"

#ifndef CONFIG_WIFI_SSID
#define CONFIG_WIFI_SSID ""
#endif
#ifndef CONFIG_WIFI_PASSWORD
#define CONFIG_WIFI_PASSWORD ""
#endif

static constexpr const char* TAG = "WiFi";

bool WiFi::isConnected() const
{
  std::lock_guard<std::mutex> lock(stateMutex_);
  return state_ == WiFiState::CONNECTED;
}

tcpip_adapter_ip_info_t WiFi::getIpInfo() const
{
  tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
  return ipInfo;
}

void WiFi::onWifiDisconnect(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
  WiFi* wifi = reinterpret_cast<WiFi*>(arg);
  std::lock_guard<std::mutex> lock(wifi->stateMutex_);
  if (wifi->state_ == WiFiState::CONNECTING || wifi->state_ == WiFiState::CONNECTED)
  {
    ESP_LOGI(TAG, "Wi-Fi disconnected, trying to reconnect...");
    ESP_ERROR_CHECK(esp_wifi_connect());
  }
  else
  {
    ESP_LOGI(TAG, "Wi-Fi disconnected");
  }
}

void WiFi::onGotIP(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
  WiFi* wifi = reinterpret_cast<WiFi*>(arg);
  std::lock_guard<std::mutex> lock(wifi->stateMutex_);
  wifi->state_ = WiFiState::CONNECTED;
}

esp_err_t WiFi::disconnect()
{
  std::lock_guard<std::mutex> lock(stateMutex_);
  if (state_ != WiFiState::CONNECTING && state_ != WiFiState::CONNECTED)
  {
    ESP_LOGE(TAG, "Invalid wifi state cannot disconnect!");
    return ESP_ERR_INVALID_STATE;
  }
  state_ = WiFiState::DISCONNECTED;
  ESP_ERROR_CHECK(esp_wifi_disconnect());
  ESP_ERROR_CHECK(esp_wifi_stop());
  return ESP_OK;
}

esp_err_t WiFi::connect()
{
  {
    std::lock_guard<std::mutex> lock(stateMutex_);
    if (state_ != WiFiState::DISCONNECTED)
    {
      ESP_LOGE(TAG, "Invalid wifi state cannot connect!");
      return ESP_ERR_INVALID_STATE;
    }
    state_ = WiFiState::CONNECTING;
  }
  // initialize default wifi configuration
  wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
  // initialize wifi adapter with configuration
  ESP_ERROR_CHECK(esp_wifi_init(&wifiInitConfig));
  // register callbacks for disconnection Event
  ESP_ERROR_CHECK(
      esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, onWifiDisconnect, this));
  // register callbacks for connection event
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, onGotIP, this));

  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
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
  ESP_LOGI(TAG, "Connecting to %s...", wifiConfig.sta.ssid);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_connect());
  return ESP_OK;
}
