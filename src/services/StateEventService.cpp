#include "StateEventService.hpp"

#include "Log.hpp"
#include "MetadataProvider.hpp"
#include "SubscriptionManager.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "services/SoapFault.hpp"

StateEventService::StateEventService(const MicroSDC& micro_sdc,
                                     std::shared_ptr<const MetadataProvider> metadata,
                                     std::shared_ptr<SubscriptionManager> subscription_manager)
  : micro_sdc_(micro_sdc)
  , metadata_(std::move(metadata))
  , subscription_manager_(std::move(subscription_manager))
{
}

std::string StateEventService::get_uri() const
{
  return MetadataProvider::get_state_event_service_path();
}

void StateEventService::handle_request(std::unique_ptr<Request> req)
{
  const auto& request_envelope = req->get_envelope();
  const auto& soap_action = request_envelope.header.action;
  if (soap_action == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    metadata_->fill_state_event_service_metadata(response_envelope);
    response_envelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(response_envelope);
  }
  else if (soap_action == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    auto subscribe_request = request_envelope.body.subscribe;
    auto response = subscription_manager_->dispatch(subscribe_request.value());
    response.subscriptionManager.address = metadata_->get_state_event_service_uri();

    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_SUBSCRIBE_RESPONSE);
    response_envelope.body.subscribeResponse = response;
    req->respond(response_envelope);
  }
  else if (soap_action == MDPWS::WS_ACTION_RENEW)
  {
    auto renew_request = request_envelope.body.renew.value();
    if (!request_envelope.header.identifier.has_value())
    {
      throw ExpectedElement("Identifier", MDPWS::WS_NS_EVENTING);
    }
    auto response =
        subscription_manager_->dispatch(renew_request, request_envelope.header.identifier.value());
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_RENEW_RESPONSE);
    response_envelope.body.renewResponse = response;
    req->respond(response_envelope);
  }
  else if (soap_action == MDPWS::WS_ACTION_UNSUBSCRIBE)
  {
    auto unsubscribe_request = request_envelope.body.unsubscribe.value();
    subscription_manager_->dispatch(unsubscribe_request,
                                    request_envelope.header.identifier.value());
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_UNSUBSCRIBE_RESPONSE);
    req->respond(response_envelope);
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soap_action);
    req->respond(SoapFault().envelope());
  }
}
