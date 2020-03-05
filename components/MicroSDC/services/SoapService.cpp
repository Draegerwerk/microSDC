#include "SoapService.hpp"
#include "MicroSDC.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "esp_log.h"
#include "rapidxml.hpp"

static constexpr const char* TAG = "SoapService";

MESSAGEMODEL::Envelope SoapService::parse(char* message)
{
  rapidxml::xml_document<> doc;
  doc.parse<rapidxml::parse_fastest>(message);

  auto envelopeNode = doc.first_node("Envelope", MDPWS::WS_NS_SOAP_ENVELOPE);
  if (envelopeNode == nullptr)
  {
    ESP_LOGE(TAG, "Cannot find soap envelope node in received message!");
    throw SoapFault();
  }
  try
  {
    return MESSAGEMODEL::Envelope(*envelopeNode);
  }
  catch (ExpectedElement& e)
  {
    ESP_LOGE(TAG, "ExpectedElement %s:%s not encountered\n", e.namespace_().c_str(),
             e.name().c_str());
    throw SoapFault();
  }
}

void SoapService::fillResponseMessageFromRequestMessage(MESSAGEMODEL::Envelope& envelope,
                                                        const MESSAGEMODEL::Envelope& request)
{
  using MessageIDType = MESSAGEMODEL::Envelope::HeaderType::MessageIDType;
  envelope.Header().MessageID() = MessageIDType(MicroSDC::calculateMessageID());
  envelope.Header().RelatesTo() =
      WS::ADDRESSING::RelatesToType(request.Header().MessageID().value());
}
