#include "services/DeviceService.hpp"
#include "Log.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "MetadataProvider.hpp"
#include "services/SoapFault.hpp"

static constexpr const char* TAG = "DeviceService";

DeviceService::DeviceService(std::shared_ptr<const MetadataProvider> metadata)
  : metadata_(std::move(metadata))
{
}

std::string DeviceService::getURI() const
{
  return metadata_->getDeviceServicePath();
}

void DeviceService::handleRequest(std::unique_ptr<Request> req)
{
  const auto requestEnvelope = req->getEnvelope();
  const auto& soapAction = requestEnvelope.Header().Action();
  if (soapAction == MDPWS::WS_ACTION_GET)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_RESPONSE);
    metadata_->fillDeviceMetadata(responseEnvelope);
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    LOG(LogLevel::WARNING, "HANDLE ACTION_GETMETADATA_REQUEST");
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soapAction);
    req->respond(SoapFault().envelope());
  }
}
