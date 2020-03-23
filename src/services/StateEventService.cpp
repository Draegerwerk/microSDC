#include "StateEventService.hpp"

#include "Log.hpp"
#include "Request.hpp"
#include "SubscriptionManager.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "dpws/MetadataProvider.hpp"
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
  return metadata_->getStateEventServicePath();
}

void StateEventService::handleRequest(std::shared_ptr<Request> req)
{
  const auto& requestEnvelope = req->getEnvelope();
  const auto& soapAction = requestEnvelope.Header().Action();
  if (soapAction == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    metadata_->fillStateEventServiceMetadata(responseEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    auto subscribeRequest = requestEnvelope.Body().Subscribe();
    auto response = subscriptionManager_->dispatch(subscribeRequest.value());
    response.SubscriptionManager().Address() = metadata_->getStateEventServiceURI();

    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_SUBSCRIBE_RESPONSE);
    responseEnvelope.Body().SubscribeResponse() = response;
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_RENEW)
  {
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
    req->respond(responseEnvelope);
  }
  else if (soapAction == MDPWS::WS_ACTION_UNSUBSCRIBE)
  {
    auto unsubscribeRequest = requestEnvelope.Body().Unsubscribe().value();
    subscriptionManager_->dispatch(unsubscribeRequest,
                                   requestEnvelope.Header().Identifier().value());
    MESSAGEMODEL::Envelope responseEnvelope;
    fillResponseMessageFromRequestMessage(responseEnvelope, requestEnvelope);
    responseEnvelope.Header().Action() =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_UNSUBSCRIBE_RESPONSE);
    req->respond(responseEnvelope);
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soapAction);
    req->respond(SoapFault().envelope());
  }
}
