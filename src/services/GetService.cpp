#include "GetService.hpp"

#include "Log.hpp"
#include "MetadataProvider.hpp"
#include "MicroSDC.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "services/SoapFault.hpp"
#include "uuid/UUIDGenerator.hpp"

static constexpr const char* TAG = "GetService";

GetService::GetService(const MicroSDC& micro_sdc, std::shared_ptr<const MetadataProvider> metadata)
  : micro_sdc_(micro_sdc)
  , metadata_(std::move(metadata))
{
}

std::string GetService::get_uri() const
{
  return MetadataProvider::get_get_service_path();
}

void GetService::handle_request(std::unique_ptr<Request> req)
{
  const auto& request_envelope = req->get_envelope();
  const auto& soap_action = request_envelope.header.action;
  if (soap_action == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    metadata_->fill_get_service_metadata(response_envelope);
    response_envelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(response_envelope);
  }
  else if (soap_action == SDC::ACTION_GET_MDIB_REQUEST)
  {
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(SDC::ACTION_GET_MDIB_RESPONSE);
    WS::ADDRESSING::URIType sequence_id("0");
    BICEPS::PM::MdibVersionGroup version_group{sequence_id};
    version_group.mdib_version = micro_sdc_.get_mdib().mdib_version_group.mdib_version.value_or(0);
    response_envelope.body.get_mdib_response =
        boost::optional<MESSAGEMODEL::Body::GetMdibResponseType>(version_group,
                                                                    micro_sdc_.get_mdib());
    req->respond(response_envelope);
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soap_action);
    req->respond(SoapFault().envelope());
  }
}
