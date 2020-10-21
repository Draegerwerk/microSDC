#pragma once

#include <optional>
#include <string>

/// @brief NetworkConfig holds configuration of Network settings relevant to configure MicroSDC
class NetworkConfig
{
public:
  /// @brief constructs a new NetworkConfig from given settings
  /// @param useTLS whether to use TLS in any communication
  /// @param ipAddress the ip address of this device
  /// @param port the port the microSDC service is operating on
  NetworkConfig(bool useTLS, std::string ipAddress, std::uint16_t port);

  /// @brief constructs a new NetworkConfig from given settings
  /// @param useTLS whether to use TLS in any communication
  /// @param ipAddress the ip address of this device
  /// @param port the port the microSDC service is operating on
  /// @param discoveryProxy the ip address of an discovery proxy to use in WS-Discovery managed mode
  NetworkConfig(bool useTLS, std::string ipAddress, std::uint16_t port, const std::string& discoveryProxy);

  /// @brief returns whether to use TLS for communication
  /// @return wether TLS is enabled
  bool isUsingTLS() const;

  /// @brief gets a reference to the ip address of this configuration
  /// @return the ip address string
  const std::string& ipAddress() const;

  /// @brief gets the configured port of the sdc service
  /// @return the configured port
  std::uint16_t port() const;

  /// @brief gets the optional discoveryProxy for ws-discovery
  /// @return the configured discovery proxy ip address
  const std::optional<std::string>& discoveryProxy() const;

private:
  /// whether to use TLS encrypted communication
  bool useTLS_{true};
  /// the ip address to use
  std::string ipAddress_;
  /// the configured port
  std::uint16_t port_;
  /// the ip address of the discovery proxy to use in ws-discovery
  std::optional<std::string> discoveryProxy_;
};
