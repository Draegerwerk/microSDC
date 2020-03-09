#include "NetworkConfig.hpp"

NetworkConfig::NetworkConfig(bool useTLS, std::string ipAddress)
  : useTLS_(useTLS)
  , ipAddress_(std::move(ipAddress))
{
}

void NetworkConfig::setUseTLS(bool useTLS)
{
  useTLS_ = useTLS;
}

bool NetworkConfig::useTLS() const
{
  return useTLS_;
}

const std::string& NetworkConfig::ipAddress() const
{
  return ipAddress_;
}

std::string& NetworkConfig::ipAddress()
{
  return ipAddress_;
}
