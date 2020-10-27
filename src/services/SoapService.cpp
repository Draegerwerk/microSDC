#include "SoapService.hpp"
#include "MicroSDC.hpp"
#include "datamodel/MessageModel.hpp"

void SoapService::fillResponseMessageFromRequestMessage(MESSAGEMODEL::Envelope& envelope,
                                                        const MESSAGEMODEL::Envelope& request)
{
  using MessageIDType = MESSAGEMODEL::Envelope::HeaderType::MessageIDType;
  envelope.header.messageID = MessageIDType(MicroSDC::calculateMessageID());
  envelope.header.relatesTo = WS::ADDRESSING::RelatesToType(request.header.messageID.value());
}
