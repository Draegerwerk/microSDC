#include "StateEventService.hpp"

#include "Log.hpp"
#include "MetadataProvider.hpp"
#include "SubscriptionManager.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "services/SoapFault.hpp"

StateEventService::StateEventService(const MicroSDC& microSDC,
                                     std::shared_ptr<const MetadataProvider> metadata,
                                     std::shared_ptr<SubscriptionManager> subscriptionManager)
  : microSDC_(microSDC)
  , metadata_(std::move(metadata))
  , subscriptionManager_(std::move(subscriptionManager))
{
}

std::string StateEventService::getURI() const
{
  return MetadataProvider::getStateEventServicePath();
}

void StateEventService::handleRequest(std::unique_ptr<Request> req)
{
  const auto& requestEnvelope = req->getEnvelope();
  const auto& soapAction = requestEnvelope.header.action;
  if (soapAction == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    metadata_->fillStateEventServiceMetadata(responseEnvelope);
    responseEnvelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    auto subscribeRequest = requestEnvelope.body.subscribe;
    auto response = subscriptionManager_->dispatch(subscribeRequest.value());
    response.subscriptionManager.address = metadata_->getStateEventServiceURI();

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
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soapAction);
    req->respond(SoapFault().envelope());
  }
}
