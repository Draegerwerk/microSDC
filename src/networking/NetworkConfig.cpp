#include "NetworkConfig.hpp"

NetworkConfig::NetworkConfig(bool useTLS, std::string ipAddress, std::uint16_t port)
  : useTLS_(useTLS)
  , ipAddress_(std::move(ipAddress))
  , port_(port)
{
}

NetworkConfig::NetworkConfig(bool useTLS, std::string ipAddress, std::uint16_t port,
                             DiscoveryProxyProtocol proxyProtocol,
                             const std::string& discoveryProxy)
  : useTLS_(useTLS)
  , ipAddress_(std::move(ipAddress))
  , port_(port)
  , discoveryProxy_(discoveryProxy)
  , discoveryProxyProtocol_(proxyProtocol)
{
}

bool NetworkConfig::isUsingTLS() const
{
  return useTLS_;
}

const std::string& NetworkConfig::ipAddress() const
{
  return ipAddress_;
}

std::uint16_t NetworkConfig::port() const
{
  return port_;
}

const std::optional<std::string>& NetworkConfig::discoveryProxy() const
{
  return discoveryProxy_;
}

NetworkConfig::DiscoveryProxyProtocol NetworkConfig::discoveryProxyProtocol() const
{
  return discoveryProxyProtocol_;
}
