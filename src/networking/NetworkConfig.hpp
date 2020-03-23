#pragma once

#include <string>

// TODO: add doxygen
class NetworkConfig
{
public:
  NetworkConfig(bool useTLS, std::string ipAddress);

  void setUseTLS(bool useTLS);

  bool useTLS() const;

  const std::string& ipAddress() const;

  std::string& ipAddress();

private:
  /// whether to use TLS encrypted communication
  bool useTLS_{true};
  /// the ip address to use
  std::string ipAddress_;
};
