#pragma once

#include "esp_err.h"
#include "lwip/ip4_addr.h"
#include "tcpip_adapter.h"

/**
 * An interface class providing Networking functionality
 */
class NetworkInterface
{
public:
  virtual ~NetworkInterface() = default;
  /**
   * @brief connect to the network
   * @return whether the connection could be established successfully
   */
  virtual esp_err_t connect() = 0;
  /**
   * @brief disconnect from the network
   * @return whether the disconnect was successfully
   */
  virtual esp_err_t disconnect() = 0;
  /**
   * @brief checks whether the network is connected
   * @return whether an connection is established
   */
  virtual bool isConnected() const = 0;
  /**
   * @brief gives information about the current connection
   * @return ip information
   */
  virtual tcpip_adapter_ip_info_t getIpInfo() const = 0;

  /**
   * @brief returns the currently configured ipv4 address
   * @return configured ip address
   */
  ip4_addr_t ipAddress() const
  {
    return getIpInfo().ip;
  }

  /**
   * @brief returns the currently configured netmask
   * @return configured netmask
   */
  ip4_addr_t netmask() const
  {
    return getIpInfo().netmask;
  }

  /**
   * @brief returns the currently configured gateway
   * @return configured gateway
   */
  ip4_addr_t gateway() const
  {
    return getIpInfo().gw;
  }
};
