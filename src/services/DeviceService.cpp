#include "services/DeviceService.hpp"
#include "Log.hpp"
#include "MetadataProvider.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "services/SoapFault.hpp"

static constexpr const char* TAG = "DeviceService";

DeviceService::DeviceService(std::shared_ptr<const MetadataProvider> metadata)
  : metadata_(std::move(metadata))
{
}

std::string DeviceService::get_uri() const
{
  return MetadataProvider::get_device_service_path();
}

void DeviceService::handle_request(std::unique_ptr<Request> req)
{
  const auto request_envelope = req->get_envelope();
  const auto& soap_action = request_envelope.header.action;
  if (soap_action == MDPWS::WS_ACTION_GET)
  {
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_RESPONSE);
    metadata_->fill_device_metadata(response_envelope);
    req->respond(response_envelope);
  }
  else if (soap_action == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    LOG(LogLevel::WARNING, "HANDLE ACTION_GETMETADATA_REQUEST");
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soap_action);
    req->respond(SoapFault().envelope());
  }
}
