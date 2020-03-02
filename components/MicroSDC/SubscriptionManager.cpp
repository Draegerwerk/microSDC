#include "SubscriptionManager.hpp"
#include "SDCConstants.hpp"
#include "UUIDGenerator.hpp"
#include "datamodel/ws-addressing.hpp"
#include "datamodel/xs_duration.hpp"
#include <algorithm>
#include <iostream>

WS::EVENTING::SubscribeResponse
SubscriptionManager::dispatch(const WS::EVENTING::Subscribe& subscribeRequest)
{
  if (!subscribeRequest.Filter().has_value())
  {
    throw std::runtime_error("No filter specified");
  }
  for (const auto& filterAction : subscribeRequest.Filter().value())
  {
    if (std::find(allowedSubscriptionEventActions_.begin(), allowedSubscriptionEventActions_.end(),
                  filterAction) == allowedSubscriptionEventActions_.end())
    {
      throw std::runtime_error("Unknown event action");
    }
  }
  const auto identifier = WS::EVENTING::Identifier("uuid:" + UUIDGenerator().create().toString());

  const Duration duration(
      subscribeRequest.Expires().value_or(WS::EVENTING::ExpirationType("PT1H")));
  const auto expires = duration.toExpirationTimePoint();
  SubscriptionInformation info(subscribeRequest.Delivery().NotifyTo(),
                               subscribeRequest.Filter().value(), expires);

  subscriptions_.emplace(identifier, info);

  WS::EVENTING::SubscribeResponse::SubscriptionManagerType subscriptionManager(
      WS::ADDRESSING::EndpointReferenceType(
          WS::ADDRESSING::URIType("To be filled by Soap Service")));
  subscriptionManager.ReferenceParameters() = WS::ADDRESSING::ReferenceParametersType(identifier);
  WS::EVENTING::SubscribeResponse subscribeResponse(subscriptionManager,
                                                    WS::EVENTING::ExpirationType(duration.str()));
  return subscribeResponse;
}
