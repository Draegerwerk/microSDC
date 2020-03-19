#pragma once

#include "SDCConstants.hpp"
#include "datamodel/BICEPS_MessageModel.hpp"
#include "datamodel/ws-addressing.hpp"
#include "datamodel/ws-eventing.hpp"
#include <chrono>
#include <map>
#include <mutex>
#include <string>
#include <vector>

struct esp_http_client;
class SessionManagerInterface;

class SubscriptionManager
{
public:
  WS::EVENTING::SubscribeResponse dispatch(const WS::EVENTING::Subscribe& subscribeRequest);
  WS::EVENTING::RenewResponse dispatch(const WS::EVENTING::Renew& renewRequest,
                                       const WS::EVENTING::Identifier& identifier);
  void dispatch(const WS::EVENTING::Unsubscribe& unsubscribeRequest,
                const WS::EVENTING::Identifier& identifier);
  void fireEvent(const BICEPS::MM::EpisodicMetricReport& report);

private:
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

  mutable std::mutex subscriptionMutex_;
  std::map<std::string, SubscriptionInformation> subscriptions_;
  std::shared_ptr<SessionManagerInterface> sessionManager_;
  std::vector<std::string> allowedSubscriptionEventActions_{
      SDC::ACTION_OPERATION_INVOKED_REPORT,
      SDC::ACTION_PERIODIC_ALERT_REPORT,
      SDC::ACTION_EPISODIC_ALERT_REPORT,
      SDC::ACTION_EPISODIC_COMPONENT_REPORT,
      SDC::ACTION_PERIODIC_COMPONENT_REPORT,
      SDC::ACTION_EPISODIC_METRIC_REPORT,
      SDC::ACTION_PERIODIC_METRIC_REPORT,
      SDC::ACTION_EPISODIC_OPERATIONAL_STATE_REPORT,
      SDC::ACTION_PERIODIC_OPERATIONAL_STATE_REPORT};

  void printSubscriptions() const;
};
