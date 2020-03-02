#pragma once

#include "datamodel/ws-addressing.hpp"
#include "datamodel/ws-eventing.hpp"
#include <string>
#include <vector>

class SubscriptionManager
{

  /// End Point Reference of the Event Sink (where to send notifications)
  struct EventSinkEPR
  {
    std::string address;
    std::string reference;
  };

  /// store stateful information about subscription
  struct Subscription
  {
    EventSinkEPR eventSinkEPR;
    std::vector<std::string> filters;
    std::string expiry;
  };

public:
  SubscriptionManager();
  WS::EVENTING::SubscribeResponse dispatch(const WS::EVENTING::Subscribe& subscribeRequest);

private:
  Subscription subscription_;
  /// The Identifier of this Subscription Manager
  std::string id_;
};
