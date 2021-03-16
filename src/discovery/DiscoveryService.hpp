#pragma once

#include "MessagingContext.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "networking/NetworkConfig.hpp"
#include <array>
#include <asio.hpp>
#include <atomic>
#include <thread>

/// @brief DiscoveryService manages all discovery related communication like sending hello messages
/// for discovery and replying to probes/resolves.
class DiscoveryService
{
public:
  /// @brief Constructs DiscoveryService
  DiscoveryService(WS::ADDRESSING::EndpointReferenceType::AddressType epr,
                   WS::DISCOVERY::QNameListType types, WS::DISCOVERY::UriListType x_addresses);
  DiscoveryService(const DiscoveryService&) = delete;
  DiscoveryService(DiscoveryService&&) = delete;
  DiscoveryService& operator=(const DiscoveryService&) = delete;
  DiscoveryService& operator=(DiscoveryService&&) = delete;
  ~DiscoveryService() noexcept;


  /// @brief listens on udp multicast socket for discovery messages
  void start();

  /// @brief stop the discovery service
  void stop();

  /// @brief Returns whether this discovery service is running
  /// @return whether this host runs
  bool running() const;

  /// @brief Configure a discovery proxy to operate in MANAGED mode and reduce multicast traffic
  /// @param proxyProtocol the protocol the discovery proxy should operate on
  /// @param proxyAddress the ip address of the discovery proxy to report to
  void configure_proxy(NetworkConfig::DiscoveryProxyProtocol proxy_protocol,
                       const std::string& proxy_address);

  /// @brief sets a new location of this instance
  /// @param locationDetail the location state information
  void set_location(const BICEPS::PM::LocationDetail& location_detail);

private:
  /// whether this discovery service runs
  std::atomic_bool running_{false};
  /// thread of this host
  std::thread thread_;
  /// asio IO context for discovery service
  asio::io_context io_context_;
  /// sending and receiving socket for discovery messages
  asio::ip::udp::socket socket_;
  /// multicast endpoint 239.255.255.250:3702
  asio::ip::udp::endpoint multicast_endpoint_;
  /// endpoint of the discovery proxy for udp, is empty, if no proxy is configured
  std::optional<asio::ip::udp::endpoint> discovery_proxy_udp_endpoint_;
  /// endpoint of the discovery proxy for protocol types HTTP and HTTPS
  std::string discovery_proxy_http_endpoint_;
  /// the protocol type of the discoveryProxy
  NetworkConfig::DiscoveryProxyProtocol discovery_proxy_protocol_{
      NetworkConfig::DiscoveryProxyProtocol::UDP};
  /// buffer for receving udp data
  std::unique_ptr<std::array<char, MDPWS::MAX_ENVELOPE_SIZE + 1>> receive_buffer_;
  /// sending endpoint of a received packet
  asio::ip::udp::endpoint sender_endpoint_;

  /// messaging context of this discovery host
  MessagingContext messaging_context_;
  /// endpoint reference of this host
  const WS::ADDRESSING::EndpointReferenceType::AddressType endpoint_reference_;
  /// scopes provided by this instance of MicroSDC
  WS::DISCOVERY::ScopesType scopes_;
  /// types represented by the services implementing this device
  WS::DISCOVERY::QNameListType types_;
  /// addresses of the services exposed by this device
  WS::DISCOVERY::UriListType x_addresses_;
  /// the version of the metadata
  const WS::DISCOVERY::HelloType::MetadataVersionType metadata_version_{1};


  /// @brief creates an endpoint v4 address from string
  /// @return the ipv4 address
  static asio::ip::address_v4 address_from_string(const char* address_string);

  /// @brief handle incoming udp message packet by determine its type.
  void handle_udp_message(std::size_t bytes_recvd);

  /// @brief handle a WS-Discovery message of type PROBE
  /// @param doc a pointer to the parsed xml document message
  void handle_probe(const MESSAGEMODEL::Envelope& envelope);

  /// @brief handle a WS-Discovery message of type RESOLVE
  /// @param doc a pointer to the parsed xml document message
  void handle_resolve(const MESSAGEMODEL::Envelope& envelope);

  /// @brief registers for socket receive at any discovery multicast address and the configured
  /// address of this device
  void do_receive();

  /// @breif sends a hello message to the multicast endpoint
  void send_hello();

  /// @brief constructs a hello message into a given envelope
  /// @param[out] envelope the envelope to fill the hello message into
  void build_hello_message(MESSAGEMODEL::Envelope& envelope);

  /// @breif sends a bye message to the multicast endpoint
  void send_bye();

  /// @brief constructs a bye message into a given envelope
  /// @param[out] envelope the envelope to fill the bye message into
  void build_bye_message(MESSAGEMODEL::Envelope& envelope);

  /// @brief constructs a probe match into a given envelope
  /// @param[out] envelope the envelope to fill the probe match into
  /// @param request the probe request to construct the response from
  void build_probe_match_message(MESSAGEMODEL::Envelope& envelope,
                                 const MESSAGEMODEL::Envelope& request);

  /// @brief constructs a resolve match into a given envelope
  /// @param[out] envelope the envelope to fill the resolve match into
  /// @param request the resolve request to construct the response from
  void build_resolve_match_message(MESSAGEMODEL::Envelope& envelope,
                                   const MESSAGEMODEL::Envelope& request);
};
