#include "SetService.hpp"
#include "Log.hpp"
#include "SubscriptionManager.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "MetadataProvider.hpp"
#include "services/SoapFault.hpp"
#include "uuid/UUIDGenerator.hpp"

static constexpr const char* TAG = "SetService";

SetService::SetService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata,
                       std::shared_ptr<SubscriptionManager> subscriptionManager)
  : microSDC_(microSDC)
  , metadata_(std::move(metadata))
  , subscriptionManager_(std::move(subscriptionManager))
{
}

std::string SetService::getURI() const
{
  return MetadataProvider::getSetServicePath();
}

void SetService::handleRequest(std::unique_ptr<Request> req)
{
  const auto requestEnvelope = req->getEnvelope();
  const auto& soapAction = requestEnvelope.header.action;
  if (soapAction == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    metadata_->fillSetServiceMetadata(responseEnvelope);
    responseEnvelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    auto subscribeRequest = requestEnvelope.body.subscribe;
    auto response = subscriptionManager_->dispatch(subscribeRequest.value());
    response.subscriptionManager.address = metadata_->getSetServiceURI();

    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_SUBSCRIBE_RESPONSE);
    responseEnvelope.body.subscribeResponse = response;
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_RENEW)
  {
    auto renewRequest = requestEnvelope.body.renew.value();
    if (!requestEnvelope.header.identifier.has_value())
    {
      throw ExpectedElement("Identifier", MDPWS::WS_NS_EVENTING);
    }
    auto response =
        subscriptionManager_->dispatch(renewRequest, requestEnvelope.header.identifier.value());
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_RENEW_RESPONSE);
    responseEnvelope.body.renewResponse = response;
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_UNSUBSCRIBE)
  {
    auto unsubscribeRequest = requestEnvelope.body.unsubscribe.value();
    subscriptionManager_->dispatch(unsubscribeRequest, requestEnvelope.header.identifier.value());
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_UNSUBSCRIBE_RESPONSE);
    req->respond(responseEnvelope);
  }
  else if (soapAction == SDC::ACTION_SET_VALUE)
  {
    auto setValueRequest = requestEnvelope.body.setValue.value();
    auto setValueResponse = this->dispatch(setValueRequest);
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.header.action = WS::ADDRESSING::URIType(SDC::ACTION_SET_VALUE_RESPONSE);
    responseEnvelope.body.setValueResponse = setValueResponse;
    req->respond(responseEnvelope);
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soapAction);
    req->respond(SoapFault().envelope());
  }
}

BICEPS::MM::SetValueResponse SetService::dispatch(const BICEPS::MM::SetValue& setValueRequest)
{
  // TODO: check if request is valid and update Mdib
  WS::ADDRESSING::URIType sequenceId("uuid:" + UUIDGenerator{}().toString());
  unsigned int transactionId = 0;
  BICEPS::MM::InvocationInfo invocationInfo(transactionId, BICEPS::MM::InvocationState::Fin);
  invocationInfo.invocationErrorMessage = BICEPS::MM::InvocationErrorMessage("MicroSDC rocks!");
  BICEPS::MM::SetValueResponse setValueResponse(sequenceId, invocationInfo);
  return setValueResponse;
}
