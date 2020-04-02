#include "NetworkConfig.hpp"

NetworkConfig::NetworkConfig(bool useTLS, std::string ipAddress, std::uint16_t port)
  : useTLS_(useTLS)
  , ipAddress_(std::move(ipAddress))
  , port_(port)
{
}

bool NetworkConfig::useTLS() const
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
