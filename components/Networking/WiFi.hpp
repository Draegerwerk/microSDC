#include "NetworkInterface.hpp"
#include "esp_event.h"
#include <mutex>

class WiFi : public NetworkInterface
{
  friend class NetworkHandler;

private:
  WiFi() = default;
  esp_err_t connect() override;
  esp_err_t disconnect() override;
  bool isConnected() const override;
  tcpip_adapter_ip_info_t getIpInfo() const override;

  enum class WiFiState
  {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    UNKNOWN
  } state_{WiFiState::DISCONNECTED};
  mutable std::mutex stateMutex_;

  static void onWifiDisconnect(void* arg, esp_event_base_t eventBase, int32_t eventId,
                               void* eventData);
  static void onGotIP(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);
};
