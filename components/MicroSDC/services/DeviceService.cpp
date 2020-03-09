#include "services/DeviceService.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "services/SoapFault.hpp"

#include "esp_log.h"

static constexpr const char* TAG = "DeviceService";

DeviceService::DeviceService(std::shared_ptr<const MetadataProvider> metadata)
  : metadata_(std::move(metadata))
{
}

std::string DeviceService::getURI() const
{
  return metadata_->getDeviceServicePath();
}

void DeviceService::handleRequest(std::shared_ptr<Request> req)
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
    ESP_LOGW(TAG, "HANDLE ACTION_GETMETADATA_REQUEST");
  }
  else
  {
    ESP_LOGE(TAG, "Unknown soap action %s", soapAction.c_str());
    req->respond(SoapFault().envelope());
  }
}
