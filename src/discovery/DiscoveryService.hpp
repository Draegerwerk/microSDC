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
                   WS::DISCOVERY::QNameListType types, WS::DISCOVERY::UriListType xAddresses,
                   WS::DISCOVERY::HelloType::MetadataVersionType metadataVersion = 1);
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
  void configureProxy(NetworkConfig::DiscoveryProxyProtocol proxyProtocol,
                      const std::string& proxyAddress);

  /// @brief sets a new location of this instance
  /// @param locationDetail the location state information
  void setLocation(const BICEPS::PM::LocationDetail& locationDetail);

private:
  /// whether this discovery service runs
  std::atomic_bool running_{false};
  /// thread of this host
  std::thread thread_;
  /// asio IO context for discovery service
  asio::io_context ioContext_;
  /// sending and receiving socket for discovery messages
  asio::ip::udp::socket socket_;
  /// multicast endpoint 239.255.255.250:3702
  asio::ip::udp::endpoint multicastEndpoint_;
  /// endpoint of the discovery proxy for udp, is empty, if no proxy is configured
  std::optional<asio::ip::udp::endpoint> discoveryProxyUdpEndpoint_;
  /// endpoint of the discovery proxy for protocol types HTTP and HTTPS
  std::string discoveryProxyHttpEndpoint_;
  /// the protocol type of the discoveryProxy
  NetworkConfig::DiscoveryProxyProtocol discoveryProxyProtocol_{
      NetworkConfig::DiscoveryProxyProtocol::UDP};
  /// buffer for receving udp data
  std::unique_ptr<std::array<char, MDPWS::MAX_ENVELOPE_SIZE + 1>> receiveBuffer_;
  /// sending endpoint of a received packet
  asio::ip::udp::endpoint senderEndpoint_;

  /// messaging context of this discovery host
  MessagingContext messagingContext_;
  /// endpoint reference of this host
  const WS::ADDRESSING::EndpointReferenceType::AddressType endpointReference_;
  /// scopes provided by this instance of MicroSDC
  WS::DISCOVERY::ScopesType scopes_;
  /// types represented by the services implementing this device
  WS::DISCOVERY::QNameListType types_;
  /// addresses of the services exposed by this device
  WS::DISCOVERY::UriListType xAddresses_;
  /// the version of the metadata
  const WS::DISCOVERY::HelloType::MetadataVersionType metadataVersion_;


  /// @brief creates an endpoint v4 address from string
  /// @return the ipv4 address
  static asio::ip::address_v4 addressFromString(const char* addressString);

  /// @brief handle incoming udp message packet by determine its type.
  void handleUDPMessage(std::size_t bytesRecvd);

  /// @brief handle a WS-Discovery message of type PROBE
  /// @param doc a pointer to the parsed xml document message
  void handleProbe(const MESSAGEMODEL::Envelope& envelope);

  /// @brief handle a WS-Discovery message of type RESOLVE
  /// @param doc a pointer to the parsed xml document message
  void handleResolve(const MESSAGEMODEL::Envelope& envelope);

  /// @brief registers for socket receive at any discovery multicast address and the configured
  /// address of this device
  void doReceive();

  /// @breif sends a hello message to the multicast endpoint
  void sendHello();

  /// @brief constructs a hello message into a given envelope
  /// @param[out] envelope the envelope to fill the hello message into
  void buildHelloMessage(MESSAGEMODEL::Envelope& envelope);

  /// @breif sends a bye message to the multicast endpoint
  void sendBye();

  /// @brief constructs a bye message into a given envelope
  /// @param[out] envelope the envelope to fill the bye message into
  void buildByeMessage(MESSAGEMODEL::Envelope& envelope);

  /// @brief constructs a probe match into a given envelope
  /// @param[out] envelope the envelope to fill the probe match into
  /// @param request the probe request to construct the response from
  void buildProbeMatchMessage(MESSAGEMODEL::Envelope& envelope,
                              const MESSAGEMODEL::Envelope& request);

  /// @brief constructs a resolve match into a given envelope
  /// @param[out] envelope the envelope to fill the resolve match into
  /// @param request the resolve request to construct the response from
  void buildResolveMatchMessage(MESSAGEMODEL::Envelope& envelope,
                                const MESSAGEMODEL::Envelope& request);
};
