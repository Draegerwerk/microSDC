#pragma once

#include <boost/optional.hpp>
#include <string>

/// @brief NetworkConfig holds configuration of Network settings relevant to configure MicroSDC
class NetworkConfig
{
public:
  enum class DiscoveryProxyProtocol
  {
    UDP,
    HTTP,
    HTTPS
  };
  /// @brief constructs a new NetworkConfig from given settings
  /// @param useTLS whether to use TLS in any communication
  /// @param ipAddress the ip address of this device
  /// @param port the port the microSDC service is operating on
  NetworkConfig(bool use_tls, std::string ip_address, std::uint16_t port);

  /// @brief constructs a new NetworkConfig from given settings
  /// @param useTLS whether to use TLS in any communication
  /// @param ipAddress the ip address of this device
  /// @param port the port the microSDC service is operating on
  /// @param discoveryProxy the ip address of an discovery proxy to use in WS-Discovery managed mode
  NetworkConfig(bool use_tls, std::string ip_address, std::uint16_t port,
                DiscoveryProxyProtocol proxy_protocol, const std::string& discovery_proxy);

  /// @brief returns whether to use TLS for communication
  /// @return wether TLS is enabled
  bool is_using_tls() const;

  /// @brief gets a reference to the ip address of this configuration
  /// @return the ip address string
  const std::string& ip_address() const;

  /// @brief gets the configured port of the sdc service
  /// @return the configured port
  std::uint16_t port() const;

  /// @brief gets the optional discoveryProxy for ws-discovery
  /// @return the configured discovery proxy ip address
  const boost::optional<std::string>& discovery_proxy() const;

  /// @brief gets the configured discoveryProxyProtocol
  /// @return the protocol the discovery proxy is communicating
  DiscoveryProxyProtocol discovery_proxy_protocol() const;


private:
  /// whether to use TLS encrypted communication
  bool use_tls_{true};
  /// the ip address to use
  std::string ip_address_;
  /// the configured port
  std::uint16_t port_;
  /// the ip address of the discovery proxy to use in ws-discovery
  boost::optional<std::string> discovery_proxy_;
  /// the communication protocol of the discovery proxy
  DiscoveryProxyProtocol discovery_proxy_protocol_{DiscoveryProxyProtocol::UDP};
};
