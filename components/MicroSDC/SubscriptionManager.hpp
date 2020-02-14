#pragma once

#include "datamodel/ws-eventing.hpp"

class SubscriptionManager
{
public:
  SubscriptionManager();

  WS::EVENTING::SubscribeResponse dispatch(const WS::EVENTING::Subscribe& subscribeRequest);
};
