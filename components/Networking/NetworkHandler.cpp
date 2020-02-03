#include "NetworkHandler.hpp"
#include "lwip/sockets.h"

NetworkHandler::NetworkHandler()
{
#if CONFIG_CONNECT_ETHERNET
  networkInterface_ = std::unique_ptr<NetworkInterface>(new Ethernet());
#elif CONFIG_CONNECT_WIFI
  networkInterface_ = std::unique_ptr<NetworkInterface>(new WiFi());
#endif
}

NetworkHandler& NetworkHandler::getInstance()
{
  static NetworkHandler instance__;
  return instance__;
}

esp_err_t NetworkHandler::connect()
{
  return networkInterface_->connect();
}

std::string NetworkHandler::address() const
{
  // Get the sender's ip address as string
  std::array<char, 32> addressString;
  const ip4_addr_t address = networkInterface_->ipAddress();
  inet_ntoa_r(address, addressString.data(), addressString.size());
  return std::string(addressString.data(), strlen(addressString.data()));
}
