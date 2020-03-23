#pragma once

#include "MessagingContext.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include <array>
#include <asio.hpp>
#include <atomic>
#include <thread>

class DPWSHost
{
public:
  /// @brief Constructs DPWS host
  DPWSHost(WS::ADDRESSING::EndpointReferenceType::AddressType epr,
           WS::DISCOVERY::QNameListType types, WS::DISCOVERY::UriListType xAddresses,
           WS::DISCOVERY::HelloType::MetadataVersionType metadataVersion = 1);


  /// @brief listens on udp multicast socket for DPWS messages
  void start();


  /// @brief stop the DPWS host
  void stop();


  /// @brief Returns whether this DPWS host is running
  /// @return whether this host runs
  bool running() const;

  void setLocation(const BICEPS::PM::LocationDetailType& locationDetail);

private:
  /// whether this dpws host runs
  std::atomic_bool running_{false};
  /// thread of this host
  std::thread thread_;
  /// asio IO context for DPWS
  asio::io_context ioContext_;
  /// sending and receiving socket for DPWS messages
  asio::ip::udp::socket socket_;
  /// multicast endpoint 239.255.255.250:3702
  asio::ip::udp::endpoint multicastEndpoint_;
  /// buffer for receving udp data
  std::unique_ptr<std::array<char, MDPWS::MAX_ENVELOPE_SIZE + 1>> receiveBuffer_;
  /// sending endpoint of a received packet
  asio::ip::udp::endpoint senderEndpoint_;

  /// messaging context of this DPWS host
  MessagingContext messagingContext_;
  /// endpoint reference of this host
  const WS::ADDRESSING::EndpointReferenceType::AddressType endpointReference_;
  WS::DISCOVERY::ScopesType scopes_;
  WS::DISCOVERY::QNameListType types_;
  WS::DISCOVERY::UriListType xAddresses_;
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

  void doReceive();

  void sendHello();

  void buildHelloMessage(MESSAGEMODEL::Envelope& envelope);
  void buildProbeMatchMessage(MESSAGEMODEL::Envelope& envelope,
                              const MESSAGEMODEL::Envelope& request);
  void buildResolveMatchMessage(MESSAGEMODEL::Envelope& envelope,
                                const MESSAGEMODEL::Envelope& request);
};
