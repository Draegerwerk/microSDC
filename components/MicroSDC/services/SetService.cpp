#include "SetService.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "esp_log.h"

static constexpr const char* TAG = "SetService";

SetService::SetService(const MicroSDC& microSDC, MetadataProvider metadata)
  : microSDC_(microSDC)
  , metadata_(std::move(metadata))
{
}

std::string SetService::getURI() const
{
  return metadata_.getSetServicePath();
}

void SetService::handleRequest(httpd_req* req, char* message)
{
  MESSAGEMODEL::Envelope requestEnvelope = parse(message);
  const auto& soapAction = requestEnvelope.Header().Action();
  if (soapAction.uri() == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    metadata_.fillSetServiceMetadata(responseEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGI(TAG, "Sending GetMetadataResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else if (soapAction.uri() == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    ESP_LOGI(TAG, "Got Subscribe: \n %s", message);
    // TODO: Implement SubscriptionManager
    // auto subscriptionManager = std::make_shared<SubscriptionManager>();
    // subscriptionManagers_.emplace_back(subscriptionManager);

    // TODO: parse subscribe
    // requestEnvelope.Body().Subscribe();
  }
  else
  {
    ESP_LOGI(TAG, "Unknown soap action %s", soapAction.uri().c_str());
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error");
  }
}
