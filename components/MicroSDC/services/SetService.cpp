#include "SetService.hpp"
#include "UUIDGenerator.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "esp_log.h"
#include <utility>

static constexpr const char* TAG = "SetService";

SetService::SetService(const MicroSDC& microSDC, MetadataProvider metadata,
                       std::shared_ptr<SubscriptionManager> subscriptionManager)
  : microSDC_(microSDC)
  , metadata_(std::move(metadata))
  , subscriptionManager_(std::move(subscriptionManager))
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
    ESP_LOGD(TAG, "Sending GetMetadataResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else if (soapAction.uri() == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    ESP_LOGD(TAG, "Got Subscribe: \n %s", message);
    auto subscribeRequest = requestEnvelope.Body().Subscribe();
    auto response = subscriptionManager_->dispatch(subscribeRequest.value());
    response.SubscriptionManager().Address() = metadata_.getSetServiceURI();

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
  else if (soapAction.uri() == SDC::ACTION_SET_VALUE)
  {
    ESP_LOGI(TAG, "Got SetValue: \n %s", message);
    auto setValueRequest = requestEnvelope.Body().SetValue().value();
    auto setValueResponse = this->dispatch(setValueRequest);
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() = WS::ADDRESSING::URIType(SDC::ACTION_SET_VALUE_RESPONSE);
    responseEnvelope.Body().SetValueResponse() = setValueResponse;
    MessageSerializer serializer;
    serializer.serialize(responseEnvelope);
    const auto message = serializer.str();
    ESP_LOGI(TAG, "Sending SetValueResponse: \n %s", message.c_str());
    httpd_resp_send(req, message.c_str(), message.length());
  }
  else
  {
    ESP_LOGE(TAG, "Unknown soap action %s", soapAction.uri().c_str());
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error");
  }
}

BICEPS::MM::SetValueResponse SetService::dispatch(const BICEPS::MM::SetValue& setValueRequest)
{
  // TODO: check if request is valid and update Mdib
  WS::ADDRESSING::URIType sequenceId("uuid:" + UUIDGenerator().create().toString());
  unsigned int transactionId = 0;
  BICEPS::MM::InvocationInfo invocationInfo(transactionId, BICEPS::MM::InvocationState::Fin);
  invocationInfo.InvocationErrorMessage() = std::string("MicroSDC rocks!");
  BICEPS::MM::SetValueResponse setValueResponse(sequenceId, invocationInfo);
  return setValueResponse;
}
