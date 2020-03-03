#include "StateEventService.hpp"

#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "esp_log.h"
#include <utility>

static constexpr const char* TAG = "StateEventService";

StateEventService::StateEventService(const MicroSDC& microSDC, MetadataProvider metadata,
                                     std::shared_ptr<SubscriptionManager> subscriptionManager)
  : microSDC_(microSDC)
  , metadata_(std::move(metadata))
  , subscriptionManager_(std::move(subscriptionManager))
{
}

std::string StateEventService::getURI() const
{
  return metadata_.getStateEventServicePath();
}

void StateEventService::handleRequest(httpd_req* req, char* message)
{
  MESSAGEMODEL::Envelope requestEnvelope = parse(message);
  const auto& soapAction = requestEnvelope.Header().Action();
  if (soapAction.uri() == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    metadata_.fillStateEventServiceMetadata(responseEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGI(TAG, "Sending GetMetadataResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else
  {
    ESP_LOGI(TAG, "Unknown soap action %s \n %s", soapAction.uri().c_str(), message);
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error");
  }
}
