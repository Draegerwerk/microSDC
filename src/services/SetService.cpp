#include "SetService.hpp"
#include "Log.hpp"
#include "MetadataProvider.hpp"
#include "MicroSDC.hpp"
#include "SubscriptionManager.hpp"
#include "WebServer/Request.hpp"
#include "datamodel/ExpectedElement.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "services/SoapFault.hpp"
#include "uuid/UUIDGenerator.hpp"

static constexpr const char* TAG = "SetService";

SetService::SetService(MicroSDC* micro_sdc, std::shared_ptr<const MetadataProvider> metadata,
                       std::shared_ptr<SubscriptionManager> subscription_manager)
  : micro_sdc_(micro_sdc)
  , metadata_(std::move(metadata))
  , subscription_manager_(std::move(subscription_manager))
{
}

std::string SetService::get_uri() const
{
  return MetadataProvider::get_set_service_path();
}

void SetService::handle_request(std::unique_ptr<Request> req)
{
  const auto request_envelope = req->get_envelope();
  const auto& soap_action = request_envelope.header.action;
  if (soap_action == MDPWS::WS_ACTION_GET_METADATA_REQUEST)
  {
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    metadata_->fill_set_service_metadata(response_envelope);
    response_envelope.header.action =
        WS::ADDRESSING::URIType(MDPWS::WS_ACTION_GET_METADATA_RESPONSE);
    req->respond(response_envelope);
  }
  else if (soap_action == MDPWS::WS_ACTION_SUBSCRIBE)
  {
    auto subscribe_request = request_envelope.body.subscribe;
    auto response = subscription_manager_->dispatch(subscribe_request.value());
    response.subscription_manager.address = metadata_->get_set_service_uri();

    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(MDPWS::WS_ACTION_SUBSCRIBE_RESPONSE);
    response_envelope.body.subscribe_response = response;
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
    response_envelope.body.renew_response = response;
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
  else if (soap_action == SDC::ACTION_SET_VALUE)
  {
    auto set_value_request = request_envelope.body.set_value.value();
    auto set_value_response = this->dispatch(set_value_request);
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(SDC::ACTION_SET_VALUE_RESPONSE);
    response_envelope.body.set_value_response = set_value_response;
    req->respond(response_envelope);
  }
  else if (soap_action == SDC::ACTION_SET_STRING)
  {
    auto set_string_request = request_envelope.body.set_string.value();
    auto set_string_response = this->dispatch(set_string_request);
    MESSAGEMODEL::Envelope response_envelope;
    fill_response_message_from_request_message(response_envelope, request_envelope);
    response_envelope.header.action = WS::ADDRESSING::URIType(SDC::ACTION_SET_VALUE_RESPONSE);
    response_envelope.body.set_string_response = set_string_response;
    req->respond(response_envelope);
  }
  else
  {
    LOG(LogLevel::ERROR, "Unknown soap action " << soap_action);
    req->respond(SoapFault().envelope());
  }
}

BICEPS::MM::SetStringResponse SetService::dispatch(const BICEPS::MM::SetString& set_string_request)
{
  const auto invocation_state = micro_sdc_->request_state_change(set_string_request);
  WS::ADDRESSING::URIType sequence_id("uuid:" + UUIDGenerator{}().to_string());
  unsigned int transaction_id = 0;
  BICEPS::MM::InvocationInfo invocation_info(transaction_id, invocation_state);
  BICEPS::MM::SetStringResponse set_string_response(sequence_id, invocation_info);
  return set_string_response;
}

BICEPS::MM::SetValueResponse SetService::dispatch(const BICEPS::MM::SetValue& set_value_request)
{
  const auto invocation_state = micro_sdc_->request_state_change(set_value_request);
  WS::ADDRESSING::URIType sequence_id("uuid:" + UUIDGenerator{}().to_string());
  unsigned int transaction_id = 0;
  BICEPS::MM::InvocationInfo invocation_info(transaction_id, invocation_state);
  BICEPS::MM::SetValueResponse set_value_response(sequence_id, invocation_info);
  return set_value_response;
}
