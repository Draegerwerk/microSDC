#include "SoapService.hpp"
#include "MicroSDC.hpp"
#include "datamodel/MessageModel.hpp"

void SoapService::fill_response_message_from_request_message(MESSAGEMODEL::Envelope& envelope,
                                                             const MESSAGEMODEL::Envelope& request)
{
  using MessageIDType = MESSAGEMODEL::Envelope::HeaderType::MessageIDType;
  envelope.header.message_id = MessageIDType(MicroSDC::calculate_message_id());
  envelope.header.relates_to = WS::ADDRESSING::RelatesToType(request.header.message_id.value());
}
