#include "Ethernet.hpp"
#include "esp_eth.h"

esp_err_t Ethernet::connect()
{
#if !CONFIG_ETHERNET_DHCP
  ESP_ERROR_CHECK(tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_ETH));
  tcpip_adapter_ip_info_t tcp_ip_info;
  IP4_ADDR(&tcp_ip_info.ip, 10, 42, 0, 2);
  IP4_ADDR(&tcp_ip_info.gw, 10, 42, 0, 2);
  IP4_ADDR(&tcp_ip_info.netmask, 255, 255, 255, 0);
  ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_ETH, &tcp_ip_info));
#endif
  ESP_ERROR_CHECK(tcpip_adapter_set_default_eth_handlers());

  eth_mac_config_t macConfig = ETH_MAC_DEFAULT_CONFIG();
  eth_phy_config_t phyConfig = ETH_PHY_DEFAULT_CONFIG();
  // ESP OLIMEX EVB specific phy address
  phyConfig.phy_addr = 0;

  esp_eth_mac_t* mac = esp_eth_mac_new_esp32(&macConfig);
  esp_eth_phy_t* phy = esp_eth_phy_new_lan8720(&phyConfig);

  esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, phy);
  esp_eth_handle_t ethHandle = nullptr;
  ESP_ERROR_CHECK(esp_eth_driver_install(&config, &ethHandle));
  ESP_ERROR_CHECK(esp_eth_start(ethHandle));

  return ESP_OK;
}

esp_err_t Ethernet::disconnect()
{
  return ESP_OK;
}

bool Ethernet::isConnected() const
{
  return false;
}

tcpip_adapter_ip_info_t Ethernet::getIpInfo() const
{
  tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ipInfo);
  return ipInfo;
}
