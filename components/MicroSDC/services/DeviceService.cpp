#include "services/DeviceService.hpp"
#include "MicroSDC.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "esp_log.h"
#include "rapidxml.hpp"

static constexpr const char* TAG = "DeviceService";

DeviceService::DeviceService(MetadataProvider metadata)
  : metadata_(std::move(metadata))
{
}

std::string DeviceService::getURI() const
{
  return metadata_.getDeviceServicePath();
}

void DeviceService::handleRequest(httpd_req_t* req, char* message)
{
  MESSAGEMODEL::Envelope requestEnvelope = parse(message);
  const auto& soapAction = requestEnvelope.Header().Action();
  if (soapAction == MDPWS::WS_ACTION_GET)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_RESPONSE);
    metadata_.fillDeviceMetadata(responseEnvelope);
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGD(TAG, "Sending response: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else if (soapAction == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    ESP_LOGW(TAG, "HANDLE ACTION_GETMETADATA_REQUEST");
  }
  else
  {
    ESP_LOGE(TAG, "Unknown soap action %s", soapAction.c_str());
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error");
  }
}
