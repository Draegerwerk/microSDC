#include "NetworkConfig.hpp"

NetworkConfig::NetworkConfig(bool use_tls, std::string ip_address, std::uint16_t port)
  : use_tls_(use_tls)
  , ip_address_(std::move(ip_address))
  , port_(port)
{
}

NetworkConfig::NetworkConfig(bool use_tls, std::string ip_address, std::uint16_t port,
                             DiscoveryProxyProtocol proxy_protocol,
                             const std::string& discovery_proxy)
  : use_tls_(use_tls)
  , ip_address_(std::move(ip_address))
  , port_(port)
  , discovery_proxy_(discovery_proxy)
  , discovery_proxy_protocol_(proxy_protocol)
{
}

bool NetworkConfig::is_using_tls() const
{
  return use_tls_;
}

const std::string& NetworkConfig::ip_address() const
{
  return ip_address_;
}

std::uint16_t NetworkConfig::port() const
{
  return port_;
}

const std::optional<std::string>& NetworkConfig::discovery_proxy() const
{
  return discovery_proxy_;
}

NetworkConfig::DiscoveryProxyProtocol NetworkConfig::discovery_proxy_protocol() const
{
  return discovery_proxy_protocol_;
}
