#pragma once

#include <string>

/// @brief NetworkConfig holds configuration of Network settings relevant to configure MicroSDC
class NetworkConfig
{
public:
  /// @brief constructs a new NetworkConfig from given settings
  /// @param useTLS whether to use TLS in any communication
  /// @param ipAddress the ip address of this device
  NetworkConfig(bool useTLS, std::string ipAddress, std::uint16_t port);

  /// @brief returns whether to use TLS for communication
  /// @return wether TLS is enabled
  bool useTLS() const;

  /// @brief gets a reference to the ip address of this configuration
  /// @return the ip address string
  const std::string& ipAddress() const;

  /// @brief gets the configured port of the sdc service
  /// @return the configured port
  std::uint16_t port() const;

private:
  /// whether to use TLS encrypted communication
  bool useTLS_{true};
  /// the ip address to use
  std::string ipAddress_;
  /// the configured port
  std::uint16_t port_;
};
