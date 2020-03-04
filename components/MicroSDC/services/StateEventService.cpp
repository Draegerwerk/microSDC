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
    ESP_LOGD(TAG, "Sending GetMetadataResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else if (soapAction.uri() == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    ESP_LOGD(TAG, "Got Subscribe: \n %s", message);
    auto subscribeRequest = requestEnvelope.Body().Subscribe();
    auto response = subscriptionManager_->dispatch(subscribeRequest.value());
    response.SubscriptionManager().Address() = metadata_.getStateEventServiceURI();

    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_SUBSCRIBE_RESPONSE);
    responseEnvelope.Body().SubscribeResponse() = response;
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGD(TAG, "Sending SubscribeResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else if (soapAction.uri() == MDPWS::WS_ACTION_RENEW)
  {
    ESP_LOGD(TAG, "Got Renew: \n %s", message);
    auto renewRequest = requestEnvelope.Body().Renew().value();
    if (!requestEnvelope.Header().Identifier().has_value())
    {
      throw ExpectedElement("Identifier", MDPWS::WS_NS_EVENTING);
    }
    auto response =
        subscriptionManager_->dispatch(renewRequest, requestEnvelope.Header().Identifier().value());
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_RENEW_RESPONSE);
    responseEnvelope.Body().RenewResponse() = response;
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGD(TAG, "Sending RenewResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else if (soapAction.uri() == MDPWS::WS_ACTION_UNSUBSCRIBE)
  {
    ESP_LOGD(TAG, "Got Unsubscribe: \n %s", message);
    auto unsubscribeRequest = requestEnvelope.Body().Unsubscribe().value();
    subscriptionManager_->dispatch(unsubscribeRequest,
                                   requestEnvelope.Header().Identifier().value());
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_UNSUBSCRIBE_RESPONSE);
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGD(TAG, "Sending UnsubscribeResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else
  {
    ESP_LOGD(TAG, "Unknown soap action %s \n %s", soapAction.uri().c_str(), message);
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error");
  }
}
