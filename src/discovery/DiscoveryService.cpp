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
                                   WS::DISCOVERY::UriListType x_addresses,
                                   WS::DISCOVERY::HelloType::MetadataVersionType metadata_version)
  : socket_(io_context_,
            asio::ip::udp::endpoint(asio::ip::udp::v4(), MDPWS::UDP_MULTICAST_DISCOVERY_PORT))
  , multicast_endpoint_(address_from_string(MDPWS::UDP_MULTICAST_DISCOVERY_IP_V4),
                        MDPWS::UDP_MULTICAST_DISCOVERY_PORT)
  , receive_buffer_(std::make_unique<std::array<char, MDPWS::MAX_ENVELOPE_SIZE + 1>>())
  , endpoint_reference_(std::move(epr))
  , types_(std::move(types))
  , x_addresses_(std::move(x_addresses))
  , metadata_version_(metadata_version)
{
  socket_.set_option(asio::ip::udp::socket::reuse_address(true));
  socket_.set_option(asio::ip::multicast::join_group(multicast_endpoint_.address()));
}

DiscoveryService::~DiscoveryService() noexcept
{
  stop();
}

void DiscoveryService::stop()
{
  LOG(LogLevel::INFO, "Stopping...");
  send_bye();
  running_.store(false);
  socket_.close();
  io_context_.stop();
  thread_.join();
}

void DiscoveryService::start()
{
  thread_ = std::thread([&]() {
    running_.store(true);
    send_hello();
    LOG(LogLevel::INFO, "Start listening for discovery messages...");
    do_receive();
    io_context_.run();
    LOG(LogLevel::INFO, "Shutting down discovery service thread...");
  });
}

bool DiscoveryService::running() const
{
  return running_.load();
}

void DiscoveryService::configure_proxy(const NetworkConfig::DiscoveryProxyProtocol proxy_protocol,
                                       const std::string& proxy_address)
{
  discovery_proxy_protocol_ = proxy_protocol;
  if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::UDP)
  {
    discovery_proxy_udp_endpoint_ = {address_from_string(proxy_address.c_str()),
                                     MDPWS::UDP_MULTICAST_DISCOVERY_PORT};
  }
  else if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTP ||
           discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTPS)
  {
    discovery_proxy_http_endpoint_ = proxy_address;
  }
  if (running())
  {
    send_hello();
  }
}

void DiscoveryService::set_location(const BICEPS::PM::LocationDetail& location_detail)
{
  std::string ctxt = "sdc.ctxt.loc:/sdc.ctxt.loc.detail/?";
  ctxt += "fac=" + location_detail.facility.value_or("");
  ctxt += "&bldng=" + location_detail.building.value_or("");
  ctxt += "&poc=" + location_detail.poC.value_or("");
  ctxt += "&flr=" + location_detail.floor.value_or("");
  ctxt += "&rm=" + location_detail.room.value_or("");
  ctxt += "&bed=" + location_detail.bed.value_or("");
  scopes_.emplace_back(WS::ADDRESSING::URIType(ctxt));
}

void DiscoveryService::do_receive()
{
  if (running_.load())
  {
    socket_.async_receive_from(
        asio::buffer(receive_buffer_->data(), receive_buffer_->size()), sender_endpoint_,
        [this](const std::error_code& error, std::size_t bytes_recvd) {
          LOG(LogLevel::DEBUG, "Received " << bytes_recvd << " bytes, ec: " << error.message());
          // null terminate whatever received
          receive_buffer_->at(bytes_recvd) = '\0';
          if (!error)
          {
            handle_udp_message(bytes_recvd);
          }
          do_receive();
        });
  }
}

asio::ip::address_v4 DiscoveryService::address_from_string(const char* address_string)
{
  asio::ip::address_v4::bytes_type address_bytes;
  if (inet_pton(AF_INET, address_string, &address_bytes) <= 0)
  {
    throw std::runtime_error("Cannot create ip address from string!");
  }
  return asio::ip::address_v4(address_bytes);
}

void DiscoveryService::handle_udp_message(std::size_t bytes_recvd)
{
  const auto sender_address = sender_endpoint_.address().to_string();
  LOG(LogLevel::DEBUG, "Received " << bytes_recvd << " bytes from " << sender_address << "\n"
                                   << receive_buffer_->data());

  rapidxml::xml_document<> doc;
  try
  {
    doc.parse<rapidxml::parse_fastest>(receive_buffer_->data());
  }
  catch (const rapidxml::parse_error& e)
  {
    LOG(LogLevel::ERROR, "ParseError at " << *e.where<char>() << " ("
                                          << e.where<char>() - receive_buffer_->data()
                                          << "): " << e.what());
  }
  auto* envelope_node = doc.first_node("Envelope", MDPWS::WS_NS_SOAP_ENVELOPE);
  if (envelope_node == nullptr)
  {
    LOG(LogLevel::ERROR, "Cannot find soap envelope node in received message!");
    return;
  }
  std::unique_ptr<MESSAGEMODEL::Envelope> envelope;

  try
  {
    envelope = std::make_unique<MESSAGEMODEL::Envelope>(*envelope_node);
  }
  catch (ExpectedElement& e)
  {
    LOG(LogLevel::WARNING, "In Message from " << sender_address << ": ExpectedElement " << e.ns()
                                              << ":" << e.name() << " not encountered: \n"
                                              << receive_buffer_->data());
    return;
  }

  if (envelope->body.probe.has_value())
  {
    LOG(LogLevel::INFO, "Received Probe from " << sender_address);
    handle_probe(*envelope);
  }
  else if (envelope->body.bye.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery Bye message from " << sender_address);
  }
  else if (envelope->body.hello.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery Hello message from " << sender_address);
  }
  else if (envelope->body.probeMatches.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery ProbeMatches message from " << sender_address);
  }
  else if (envelope->body.resolve.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery Resolve message from "
                            << sender_address << " asking for EndpointReference "
                            << envelope->body.resolve->endpointReference.address);
    handle_resolve(*envelope);
  }
  else if (envelope->body.resolveMatches.has_value())
  {
    LOG(LogLevel::INFO, "Received WS-Discovery ResolveMatches message from " << sender_address);
  }
  else
  {
    LOG(LogLevel::WARNING, "Received unhandled UDP message");
  }
  doc.clear();
}

void DiscoveryService::send_hello()
{
  messaging_context_.reset_instance_id();
  // Construct Hello Message
  auto message = std::make_unique<MESSAGEMODEL::Envelope>();
  build_hello_message(*message);
  MESSAGEMODEL::Envelope::HeaderType::AppSequenceType app_sequence(
      messaging_context_.get_instance_id(), messaging_context_.get_next_message_counter());
  message->header.appSequence = app_sequence;
  // Serialize and send
  MessageSerializer serializer;
  serializer.serialize(*message);
  auto msg = std::make_shared<std::string>(serializer.str());
  LOG(LogLevel::INFO, "Sending hello message...");
  const auto async_callback = [msg](const std::error_code& ec,
                                    const std::size_t bytes_transferred) {
    if (ec)
    {
      LOG(LogLevel::ERROR, "Error while sending Hello: ec " << ec.value() << ": " << ec.message());
      return;
    }
    LOG(LogLevel::DEBUG, "Sent hello msg (" << bytes_transferred << " bytes): \n" << *msg);
  };

  // alwayas send hello to multicast group
  socket_.async_send_to(asio::buffer(*msg), multicast_endpoint_, async_callback);

  // handle configured discovery proxy
  if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::UDP &&
      discovery_proxy_udp_endpoint_.has_value())
  {
    socket_.async_send_to(asio::buffer(*msg), discovery_proxy_udp_endpoint_.value(),
                          async_callback);
  }
  else if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTP)
  {
    auto session = ClientSessionFactory::produce(discovery_proxy_http_endpoint_, false);
    session->send(*msg);
  }
  else if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTPS)
  {
    auto session = ClientSessionFactory::produce(discovery_proxy_http_endpoint_, true);
    session->send(*msg);
  }
}

void DiscoveryService::build_hello_message(MESSAGEMODEL::Envelope& envelope)
{
  envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_HELLO);
  envelope.header.to = WS::ADDRESSING::URIType(MDPWS::WS_DISCOVERY_URN);
  envelope.header.messageID = WS::ADDRESSING::URIType{MicroSDC::calculate_message_id()};
  auto& hello = envelope.body.hello = WS::DISCOVERY::HelloType(
      WS::ADDRESSING::EndpointReferenceType(endpoint_reference_), metadata_version_);
  if (!scopes_.empty())
  {
    hello->scopes = scopes_;
  }
  if (!types_.empty())
  {
    hello->types = types_;
  }
  if (!x_addresses_.empty())
  {
    hello->xAddrs = x_addresses_;
  }
}

void DiscoveryService::send_bye()
{
  // Construct Bye Message
  auto message = std::make_unique<MESSAGEMODEL::Envelope>();
  build_bye_message(*message);
  MESSAGEMODEL::Envelope::HeaderType::AppSequenceType app_sequence(
      messaging_context_.get_instance_id(), messaging_context_.get_next_message_counter());
  message->header.appSequence = app_sequence;
  // Serialize and send
  MessageSerializer serializer;
  serializer.serialize(*message);
  auto msg = std::make_shared<std::string>(serializer.str());
  LOG(LogLevel::INFO, "Sending bye message...");
  const auto async_callback = [msg](const std::error_code& ec,
                                    const std::size_t bytes_transferred) {
    if (ec)
    {
      LOG(LogLevel::ERROR, "Error while sending Bye: ec " << ec.value() << ": " << ec.message());
      return;
    }
    LOG(LogLevel::DEBUG, "Sent bye msg (" << bytes_transferred << " bytes): \n" << *msg);
  };

  // alwayas send bye to multicast group
  socket_.async_send_to(asio::buffer(*msg), multicast_endpoint_, async_callback);

  // handle configured discovery proxy
  if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::UDP &&
      discovery_proxy_udp_endpoint_.has_value())
  {
    socket_.async_send_to(asio::buffer(*msg), discovery_proxy_udp_endpoint_.value(),
                          async_callback);
  }
  else if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTP)
  {
    auto session = ClientSessionFactory::produce(discovery_proxy_http_endpoint_, false);
    session->send(*msg);
  }
  else if (discovery_proxy_protocol_ == NetworkConfig::DiscoveryProxyProtocol::HTTPS)
  {
    auto session = ClientSessionFactory::produce(discovery_proxy_http_endpoint_, true);
    session->send(*msg);
  }
}

void DiscoveryService::build_bye_message(MESSAGEMODEL::Envelope& envelope)
{
  envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_BYE);
  envelope.header.to = WS::ADDRESSING::URIType(MDPWS::WS_DISCOVERY_URN);
  envelope.header.messageID = WS::ADDRESSING::URIType{MicroSDC::calculate_message_id()};
  auto& bye = envelope.body.bye =
      WS::DISCOVERY::ByeType(WS::ADDRESSING::EndpointReferenceType(endpoint_reference_));
  if (!scopes_.empty())
  {
    bye->scopes = scopes_;
  }
  if (!types_.empty())
  {
    bye->types = types_;
  }
  if (!x_addresses_.empty())
  {
    bye->xAddrs = x_addresses_;
  }
}

void DiscoveryService::handle_probe(const MESSAGEMODEL::Envelope& envelope)
{
  auto response_message = std::make_unique<MESSAGEMODEL::Envelope>();
  build_probe_match_message(*response_message, envelope);
  MessageSerializer serializer;
  serializer.serialize(*response_message);
  LOG(LogLevel::INFO, "Sending ProbeMatch");
  auto msg = std::make_shared<std::string>(serializer.str());
  socket_.async_send_to(
      asio::buffer(*msg), sender_endpoint_,
      [msg](const std::error_code& ec, const std::size_t bytes_transferred) {
        if (ec)
        {
          LOG(LogLevel::ERROR,
              "Error while sending ProbeMatch: ec " << ec.value() << ": " << ec.message());
          return;
        }
        LOG(LogLevel::DEBUG, "Sent ProbeMatch msg (" << bytes_transferred << " bytes): \n" << *msg);
      });
}

void DiscoveryService::handle_resolve(const MESSAGEMODEL::Envelope& envelope)
{
  if (envelope.body.resolve->endpointReference.address != endpoint_reference_)
  {
    return;
  }
  auto response_message = std::make_unique<MESSAGEMODEL::Envelope>();
  build_resolve_match_message(*response_message, envelope);
  MessageSerializer serializer;
  serializer.serialize(*response_message);
  LOG(LogLevel::INFO, "Sending ResolveMatch");
  auto msg = std::make_shared<std::string>(serializer.str());
  socket_.async_send_to(asio::buffer(*msg), sender_endpoint_,
                        [msg](const std::error_code& ec, const std::size_t bytes_transferred) {
                          if (ec)
                          {
                            LOG(LogLevel::ERROR, "Error while sending ResolveMatch: ec "
                                                     << ec.value() << ": " << ec.message());
                            return;
                          }
                          LOG(LogLevel::DEBUG, "Sent ResolveMatch msg (" << bytes_transferred
                                                                         << " bytes): \n"
                                                                         << *msg);
                        });
}

void DiscoveryService::build_probe_match_message(MESSAGEMODEL::Envelope& envelope,
                                                 const MESSAGEMODEL::Envelope& request)
{
  auto& probe_matches = envelope.body.probeMatches = WS::DISCOVERY::ProbeMatchesType({});
  // TODO: check for match
  auto& match = probe_matches->probeMatch.emplace_back(
      WS::ADDRESSING::EndpointReferenceType(endpoint_reference_), metadata_version_);
  if (!scopes_.empty())
  {
    match.scopes = scopes_;
  }
  if (!types_.empty())
  {
    match.types = types_;
  }
  if (!x_addresses_.empty())
  {
    match.xAddrs = x_addresses_;
  }

  envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_PROBE_MATCHES);
  if (request.header.replyTo.has_value())
  {
    envelope.header.to = request.header.replyTo->address;
  }
  else
  {
    envelope.header.to = WS::ADDRESSING::URIType(MDPWS::WS_ADDRESSING_ANONYMOUS);
  }
  if (request.header.messageID.has_value())
  {
    envelope.header.relatesTo = WS::ADDRESSING::RelatesToType(request.header.messageID.value());
  }
  envelope.header.messageID = WS::ADDRESSING::URIType{MicroSDC::calculate_message_id()};
}

void DiscoveryService::build_resolve_match_message(MESSAGEMODEL::Envelope& envelope,
                                                   const MESSAGEMODEL::Envelope& request)
{
  auto& resolve_matches = envelope.body.resolveMatches = WS::DISCOVERY::ResolveMatchesType({});
  auto& match = resolve_matches->resolveMatch.emplace_back(
      WS::ADDRESSING::EndpointReferenceType(endpoint_reference_), metadata_version_);
  if (!scopes_.empty())
  {
    match.scopes = scopes_;
  }
  if (!types_.empty())
  {
    match.types = types_;
  }
  if (!x_addresses_.empty())
  {
    match.xAddrs = x_addresses_;
  }

  envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_RESOLVE_MATCHES);
  if (request.header.replyTo.has_value())
  {
    envelope.header.to = request.header.replyTo->address;
  }
  else
  {
    envelope.header.to = WS::ADDRESSING::URIType(MDPWS::WS_ADDRESSING_ANONYMOUS);
  }
  if (request.header.messageID.has_value())
  {
    envelope.header.relatesTo = WS::ADDRESSING::RelatesToType(request.header.messageID.value());
  }
  envelope.header.messageID = WS::ADDRESSING::URIType{MicroSDC::calculate_message_id()};
}
