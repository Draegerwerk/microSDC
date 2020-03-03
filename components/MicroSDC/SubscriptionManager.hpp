#pragma once

#include "SDCConstants.hpp"
#include "datamodel/ws-addressing.hpp"
#include "datamodel/ws-eventing.hpp"
#include <chrono>
#include <map>
#include <string>
#include <vector>

class SubscriptionManager
{
  /// store stateful information about subscription
  struct SubscriptionInformation
  {
    SubscriptionInformation(WS::ADDRESSING::EndpointReferenceType notifyTo,
                            WS::EVENTING::FilterType filter,
                            std::chrono::system_clock::time_point expirationTime)
      : notifyTo(std::move(notifyTo))
      , filter(std::move(filter))
      , expirationTime(expirationTime)
    {
    }

    const WS::ADDRESSING::EndpointReferenceType notifyTo;
    const WS::EVENTING::FilterType filter;
    std::chrono::system_clock::time_point expirationTime;
  };

public:
  SubscriptionManager() = default;
  WS::EVENTING::SubscribeResponse dispatch(const WS::EVENTING::Subscribe& subscribeRequest);
  WS::EVENTING::RenewResponse dispatch(const WS::EVENTING::Renew& renewRequest,
                                       const WS::EVENTING::Identifier& identifier);

private:
  std::map<std::string, SubscriptionInformation> subscriptions_;
  std::vector<std::string> allowedSubscriptionEventActions_{SDC::ACTION_OPERATION_INVOKED_REPORT};
};
