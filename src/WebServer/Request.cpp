#include "Request.hpp"

#include "Log.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "services/SoapFault.hpp"

Request::Request(std::string msg)
  : message_(std::move(msg))
{
}

const MESSAGEMODEL::Envelope& Request::getEnvelope()
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

void Request::respond(const MESSAGEMODEL::Envelope& responseEnvelope) const
{
  MessageSerializer serializer;
  serializer.serialize(responseEnvelope);
  sendResponse(serializer.str());
}

void Request::respond(const std::string& msg) const
{
  sendResponse(msg);
}

void Request::parse()
{
  rapidxml::xml_document<> doc;
  doc.parse<rapidxml::parse_fastest>(message_.data());

  auto* envelopeNode = doc.first_node("Envelope", MDPWS::WS_NS_SOAP_ENVELOPE);
  if (envelopeNode == nullptr)
  {
    LOG(LogLevel::ERROR, "Cannot find soap envelope node in received message!");
    throw SoapFault();
  }
  try
  {
    envelope_ = std::make_shared<MESSAGEMODEL::Envelope>(*envelopeNode);
  }
  catch (ExpectedElement& e)
  {
    LOG(LogLevel::ERROR, "ExpectedElement " << e.ns() << ":" << e.name() << " not encountered");
    throw SoapFault();
  }
}
