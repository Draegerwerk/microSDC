#include "DiscoveryService.hpp"
#include "ClientSession/ClientSession.hpp"
#include "Log.hpp"
#include "MicroSDC.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include <array>
#include <memory>
#include <utility>

static constexpr const char* TAG = "DPWS";

DiscoveryService::DiscoveryService(WS::ADDRESSING::EndpointReferenceType::AddressType epr,
                                   WS::DISCOVERY::QNameListType types,
                                   WS::DISCOVERY::UriListType xAddresses,
                                   WS::DISCOVERY::HelloType::MetadataVersionType metadataVersion)
  : socket_(ioContext_,
            asio::ip::udp::endpoint(asio::ip::udp::v4(), MDPWS::UDP_MULTICAST_DISCOVERY_PORT))
  , multicastEndpoint_(addressFromString(MDPWS::UDP_MULTICAST_DISCOVERY_IP_V4),
                       MDPWS::UDP_MULTICAST_DISCOVERY_PORT)
  , receiveBuffer_(std::make_unique<std::array<char, MDPWS::MAX_ENVELOPE_SIZE + 1>>())
  , endpointReference_(std::move(epr))
  , types_(std::move(types))
  , xAddresses_(std::move(xAddresses))
  , metadataVersion_(metadataVersion)
{
  socket_.set_option(asio::ip::udp::socket::reuse_address(true));
  socket_.set_option(asio::ip::multicast::join_group(multicastEndpoint_.address()));
}

DiscoveryService::~DiscoveryService() noexcept
{
  stop();
}

void DiscoveryService::stop()
{
  LOG(LogLevel::INFO, "Stopping...");
  sendBye();
  running_.store(false);
  socket_.close();
  ioContext_.stop();
  thread_.join();
}

void DiscoveryService::start()
{
  thread_ = std::thread([&]() {
    running_.store(true);
    sendHello();
    LOG(LogLevel::INFO, "Start listening for discovery messages...");
    doReceive();
    ioContext_.run();
    LOG(LogLevel::INFO, "Shutting down discovery service thread...");
  });
}

bool DiscoveryService::running() const
{
  return running_.load();
}

void DiscoveryService::configureProxy(const NetworkConfig::DiscoveryProxyProtocol proxyProtocol,
                                      const std::string& proxyAddress)
{
  discoveryProxyProtocol_ = proxyProtocol;
  if (discoveryProxyProtocol_ == NetworkConfig::DiscoveryProxyProtocol::UDP)
  {
    discoveryProxyUdpEndpoint_ = {addressFromString(proxyAddress.c_str()),
                                  MDPWS::UDP_MULTICAST_DISCOVERY_PORT};
  }
  else if (discoveryProxyProtocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTP)
  {
    discoveryProxyHttpEndpoint_ = proxyAddress;
  }
  else
  {
    throw std::runtime_error("Configured DiscoveryProxyProtocol not implemented!");
  }
  if (running())
  {
    sendHello();
  }
}

void DiscoveryService::setLocation(const BICEPS::PM::LocationDetailType& locationDetail)
{
  std::string ctxt = "sdc.ctxt.loc:/sdc.ctxt.loc.detail/?";
  ctxt += "fac=" + locationDetail.Facility.value_or("");
  ctxt += "&bldng=" + locationDetail.Building.value_or("");
  ctxt += "&poc=" + locationDetail.PoC.value_or("");
  ctxt += "&flr=" + locationDetail.Floor.value_or("");
  ctxt += "&rm=" + locationDetail.Room.value_or("");
  ctxt += "&bed=" + locationDetail.Bed.value_or("");
  scopes_.emplace_back(WS::ADDRESSING::URIType(ctxt));
}

void DiscoveryService::doReceive()
{
  if (running_.load())
  {
    socket_.async_receive_from(
        asio::buffer(receiveBuffer_->data(), receiveBuffer_->size()), senderEndpoint_,
        [this](const std::error_code& error, std::size_t bytesRecvd) {
          LOG(LogLevel::DEBUG, "Received " << bytesRecvd << " bytes, ec: " << error.message());
          // null terminate whatever received
          receiveBuffer_->at(bytesRecvd) = '\0';
          if (!error)
          {
            handleUDPMessage(bytesRecvd);
          }
          doReceive();
        });
  }
}

asio::ip::address_v4 DiscoveryService::addressFromString(const char* addressString)
{
  asio::ip::address_v4::bytes_type addressBytes;
  if (inet_pton(AF_INET, addressString, &addressBytes) <= 0)
  {
    throw std::runtime_error("Cannot create ip address from string!");
  }
  return asio::ip::address_v4(addressBytes);
}

void DiscoveryService::handleUDPMessage(std::size_t bytesRecvd)
{
  const auto senderAddress = senderEndpoint_.address().to_string();
  LOG(LogLevel::DEBUG, "Received " << bytesRecvd << " bytes from " << senderAddress << "\n"
                                   << receiveBuffer_->data());

  rapidxml::xml_document<> doc;
  try
  {
    doc.parse<rapidxml::parse_fastest>(receiveBuffer_->data());
  }
  catch (const rapidxml::parse_error& e)
  {
    LOG(LogLevel::ERROR, "ParseError at " << *e.where<char>() << " ("
                                          << e.where<char>() - receiveBuffer_->data()
                                          << "): " << e.what());
  }
  auto* envelopeNode = doc.first_node("Envelope", MDPWS::WS_NS_SOAP_ENVELOPE);
  if (envelopeNode == nullptr)
  {
    LOG(LogLevel::ERROR, "Cannot find soap envelope node in received message!");
    return;
  }
  std::unique_ptr<MESSAGEMODEL::Envelope> envelope;

  try
  {
    envelope = std::make_unique<MESSAGEMODEL::Envelope>(*envelopeNode);
  }
  catch (ExpectedElement& e)
  {
    LOG(LogLevel::ERROR, "ExpectedElement " << e.ns() << ":" << e.name() << " not encountered");
    return;
  }

  if (envelope->Body.Probe.has_value())
  {
    LOG(LogLevel::INFO, "Received Probe from " << senderAddress);
    handleProbe(*envelope);
  }
  else if (envelope->Body.Bye.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery Bye message from " << senderAddress);
  }
  else if (envelope->Body.Hello.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery Hello message from " << senderAddress);
  }
  else if (envelope->Body.ProbeMatches.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery ProbeMatches message from " << senderAddress);
  }
  else if (envelope->Body.Resolve.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery Resolve message from "
                            << senderAddress << " asking for EndpointReference "
                            << envelope->Body.Resolve->EndpointReference.Address);
    handleResolve(*envelope);
  }
  else if (envelope->Body.ResolveMatches.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery ResolveMatches message from " << senderAddress);
  }
  else
  {
    LOG(LogLevel::WARNING, "Received unhandled UDP message");
  }
  doc.clear();
}

void DiscoveryService::sendHello()
{
  messagingContext_.resetInstanceId();
  // Construct Hello Message
  auto message = std::make_unique<MESSAGEMODEL::Envelope>();
  buildHelloMessage(*message);
  MESSAGEMODEL::Envelope::HeaderType::AppSequenceType appSequence(
      messagingContext_.getInstanceId(), messagingContext_.getNextMessageCounter());
  message->Header.AppSequence = appSequence;
  // Serialize and send
  MessageSerializer serializer;
  serializer.serialize(*message);
  auto msg = std::make_shared<std::string>(serializer.str());
  LOG(LogLevel::INFO, "Sending hello message...");
  if (discoveryProxyProtocol_ == NetworkConfig::DiscoveryProxyProtocol::UDP)
  {
    socket_.async_send_to(
        asio::buffer(*msg), discoveryProxyUdpEndpoint_.value_or(multicastEndpoint_),
        [msg](const std::error_code& ec, const std::size_t bytesTransferred) {
          if (ec)
          {
            LOG(LogLevel::ERROR,
                "Error while sending Hello: ec " << ec.value() << ": " << ec.message());
            return;
          }
          LOG(LogLevel::DEBUG, "Sent hello msg (" << bytesTransferred << " bytes): \n" << *msg);
        });
  }
  else if (discoveryProxyProtocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTP)
  {
    auto session = ClientSessionFactory::produce(discoveryProxyHttpEndpoint_, false);
    session->send(*msg);
  }
  else
  {
    throw std::runtime_error("Configured DiscoveryProxyProtocol not implemented!");
  }
}

void DiscoveryService::buildHelloMessage(MESSAGEMODEL::Envelope& envelope)
{
  envelope.Header.Action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_HELLO);
  envelope.Header.To = WS::ADDRESSING::URIType(MDPWS::WS_DISCOVERY_URN);
  envelope.Header.MessageID = WS::ADDRESSING::URIType{MicroSDC::calculateMessageID()};
  auto& hello = envelope.Body.Hello = WS::DISCOVERY::HelloType(
      WS::ADDRESSING::EndpointReferenceType(endpointReference_), metadataVersion_);
  if (!scopes_.empty())
  {
    hello->Scopes = scopes_;
  }
  if (!types_.empty())
  {
    hello->Types = types_;
  }
  if (!xAddresses_.empty())
  {
    hello->XAddrs = xAddresses_;
  }
}

void DiscoveryService::sendBye()
{
  // Construct Bye Message
  auto message = std::make_unique<MESSAGEMODEL::Envelope>();
  buildByeMessage(*message);
  MESSAGEMODEL::Envelope::HeaderType::AppSequenceType appSequence(
      messagingContext_.getInstanceId(), messagingContext_.getNextMessageCounter());
  message->Header.AppSequence = appSequence;
  // Serialize and send
  MessageSerializer serializer;
  serializer.serialize(*message);
  auto msg = std::make_shared<std::string>(serializer.str());
  LOG(LogLevel::INFO, "Sending bye message...");
  socket_.async_send_to(asio::buffer(*msg), discoveryProxyUdpEndpoint_.value_or(multicastEndpoint_),
                        [msg](const std::error_code& ec, const std::size_t bytesTransferred) {
                          if (ec)
                          {
                            LOG(LogLevel::ERROR, "Error while sending Bye: ec "
                                                     << ec.value() << ": " << ec.message());
                            return;
                          }
                          LOG(LogLevel::DEBUG, "Sent bye msg (" << bytesTransferred << " bytes): \n"
                                                                << *msg);
                        });
}

void DiscoveryService::buildByeMessage(MESSAGEMODEL::Envelope& envelope)
{
  envelope.Header.Action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_BYE);
  envelope.Header.To = WS::ADDRESSING::URIType(MDPWS::WS_DISCOVERY_URN);
  envelope.Header.MessageID = WS::ADDRESSING::URIType{MicroSDC::calculateMessageID()};
  auto& bye = envelope.Body.Bye =
      WS::DISCOVERY::ByeType(WS::ADDRESSING::EndpointReferenceType(endpointReference_));
  if (!scopes_.empty())
  {
    bye->Scopes = scopes_;
  }
  if (!types_.empty())
  {
    bye->Types = types_;
  }
  if (!xAddresses_.empty())
  {
    bye->XAddrs = xAddresses_;
  }
}

void DiscoveryService::handleProbe(const MESSAGEMODEL::Envelope& envelope)
{
  auto responseMessage = std::make_unique<MESSAGEMODEL::Envelope>();
  buildProbeMatchMessage(*responseMessage, envelope);
  MessageSerializer serializer;
  serializer.serialize(*responseMessage);
  LOG(LogLevel::INFO, "Sending ProbeMatch");
  auto msg = std::make_shared<std::string>(serializer.str());
  socket_.async_send_to(
      asio::buffer(*msg), senderEndpoint_,
      [msg](const std::error_code& ec, const std::size_t bytesTransferred) {
        if (ec)
        {
          LOG(LogLevel::ERROR,
              "Error while sending ProbeMatch: ec " << ec.value() << ": " << ec.message());
          return;
        }
        LOG(LogLevel::DEBUG, "Sent ProbeMatch msg (" << bytesTransferred << " bytes): \n" << *msg);
      });
}

void DiscoveryService::handleResolve(const MESSAGEMODEL::Envelope& envelope)
{
  if (envelope.Body.Resolve->EndpointReference.Address != endpointReference_)
  {
    return;
  }
  auto responseMessage = std::make_unique<MESSAGEMODEL::Envelope>();
  buildResolveMatchMessage(*responseMessage, envelope);
  MessageSerializer serializer;
  serializer.serialize(*responseMessage);
  LOG(LogLevel::INFO, "Sending ResolveMatch");
  auto msg = std::make_shared<std::string>(serializer.str());
  socket_.async_send_to(asio::buffer(*msg), senderEndpoint_,
                        [msg](const std::error_code& ec, const std::size_t bytesTransferred) {
                          if (ec)
                          {
                            LOG(LogLevel::ERROR, "Error while sending ResolveMatch: ec "
                                                     << ec.value() << ": " << ec.message());
                            return;
                          }
                          LOG(LogLevel::DEBUG, "Sent ResolveMatch msg (" << bytesTransferred
                                                                         << " bytes): \n"
                                                                         << *msg);
                        });
}

void DiscoveryService::buildProbeMatchMessage(MESSAGEMODEL::Envelope& envelope,
                                              const MESSAGEMODEL::Envelope& request)
{
  auto& probeMatches = envelope.Body.ProbeMatches = WS::DISCOVERY::ProbeMatchesType({});
  // TODO: check for match
  auto& match = probeMatches->ProbeMatch.emplace_back(
      WS::ADDRESSING::EndpointReferenceType(endpointReference_), metadataVersion_);
  if (!scopes_.empty())
  {
    match.Scopes = scopes_;
  }
  if (!types_.empty())
  {
    match.Types = types_;
  }
  if (!xAddresses_.empty())
  {
    match.XAddrs = xAddresses_;
  }

  envelope.Header.Action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_PROBE_MATCHES);
  if (request.Header.ReplyTo.has_value())
  {
    envelope.Header.To = request.Header.ReplyTo->Address;
  }
  else
  {
    envelope.Header.To = WS::ADDRESSING::URIType(MDPWS::WS_ADDRESSING_ANONYMOUS);
  }
  if (request.Header.MessageID.has_value())
  {
    envelope.Header.RelatesTo = WS::ADDRESSING::RelatesToType(request.Header.MessageID.value());
  }
  envelope.Header.MessageID = WS::ADDRESSING::URIType{MicroSDC::calculateMessageID()};
}

void DiscoveryService::buildResolveMatchMessage(MESSAGEMODEL::Envelope& envelope,
                                                const MESSAGEMODEL::Envelope& request)
{
  auto& resolveMatches = envelope.Body.ResolveMatches = WS::DISCOVERY::ResolveMatchesType({});
  auto& match = resolveMatches->ResolveMatch.emplace_back(
      WS::ADDRESSING::EndpointReferenceType(endpointReference_), metadataVersion_);
  if (!scopes_.empty())
  {
    match.Scopes = scopes_;
  }
  if (!types_.empty())
  {
    match.Types = types_;
  }
  if (!xAddresses_.empty())
  {
    match.XAddrs = xAddresses_;
  }

  envelope.Header.Action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_RESOLVE_MATCHES);
  if (request.Header.ReplyTo.has_value())
  {
    envelope.Header.To = request.Header.ReplyTo->Address;
  }
  else
  {
    envelope.Header.To = WS::ADDRESSING::URIType(MDPWS::WS_ADDRESSING_ANONYMOUS);
  }
  if (request.Header.MessageID.has_value())
  {
    envelope.Header.RelatesTo = WS::ADDRESSING::RelatesToType(request.Header.MessageID.value());
  }
  envelope.Header.MessageID = WS::ADDRESSING::URIType{MicroSDC::calculateMessageID()};
}
