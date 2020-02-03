#pragma once

#include "NetworkInterface.hpp"
#include <memory>

class Ethernet : public NetworkInterface
{
  friend class NetworkHandler;

private:
  /**
   * @brief constructs a new Ethernet interface
   */
  Ethernet() = default;
  esp_err_t connect() override;
  esp_err_t disconnect() override;
  bool isConnected() const override;
  tcpip_adapter_ip_info_t getIpInfo() const override;
};
