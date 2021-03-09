#include "Request.hpp"

#include "Log.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "services/SoapFault.hpp"

Request::Request(std::string msg)
  : message_(std::move(msg))
{
}

const MESSAGEMODEL::Envelope& Request::get_envelope()
{
  if (envelope_ == nullptr)
  {
    parse();
  }
  return *envelope_;
}

const char* Request::data() const
{
  return message_.c_str();
}

void Request::respond(const MESSAGEMODEL::Envelope& response_envelope) const
{
  MessageSerializer serializer;
  serializer.serialize(response_envelope);
  send_response(serializer.str());
}

void Request::respond(const std::string& msg) const
{
  send_response(msg);
}

void Request::parse()
{
  rapidxml::xml_document<> doc;
  doc.parse<rapidxml::parse_fastest>(message_.data());

  auto* envelope_node = doc.first_node("Envelope", MDPWS::WS_NS_SOAP_ENVELOPE);
  if (envelope_node == nullptr)
  {
    LOG(LogLevel::ERROR, "Cannot find soap envelope node in received message!");
    throw SoapFault();
  }
  try
  {
    envelope_ = std::make_shared<MESSAGEMODEL::Envelope>(*envelope_node);
  }
  catch (ExpectedElement& e)
  {
    LOG(LogLevel::ERROR, "ExpectedElement " << e.ns() << ":" << e.name() << " not encountered");
    throw SoapFault();
  }
}
