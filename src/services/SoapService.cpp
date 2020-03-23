#include "SoapService.hpp"
#include "MicroSDC.hpp"
#include "datamodel/MessageModel.hpp"

void SoapService::fillResponseMessageFromRequestMessage(MESSAGEMODEL::Envelope& envelope,
                                                        const MESSAGEMODEL::Envelope& request)
{
  using MessageIDType = MESSAGEMODEL::Envelope::HeaderType::MessageIDType;
  envelope.Header().MessageID() = MessageIDType(MicroSDC::calculateMessageID());
  envelope.Header().RelatesTo() =
      WS::ADDRESSING::RelatesToType(request.Header().MessageID().value());
}
