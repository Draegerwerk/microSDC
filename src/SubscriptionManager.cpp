#include "SubscriptionManager.hpp"

#include "Log.hpp"
#include "MicroSDC.hpp"
#include "SDCConstants.hpp"
#include "SessionManager/SessionManager.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "datamodel/ws-addressing.hpp"
#include "datamodel/xs_duration.hpp"
#include "uuid/UUIDGenerator.hpp"
#include <algorithm>

static constexpr const char* TAG = "SubscriptionManager";

SubscriptionManager::SubscriptionManager(std::shared_ptr<SessionManagerInterface> sessionManager)
  : sessionManager_(std::move(sessionManager))
{
}

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
  const auto identifier = WS::EVENTING::Identifier("uuid:" + UUIDGenerator{}().toString());

  const Duration duration(
      subscribeRequest.Expires().value_or(WS::EVENTING::ExpirationType("PT1H")));
  const auto expires = duration.toExpirationTimePoint();
  SubscriptionInformation info(subscribeRequest.Delivery().NotifyTo(),
                               subscribeRequest.Filter().value(), expires);

  {
    std::lock_guard<std::mutex> lock(subscriptionMutex_);
    subscriptions_.emplace(identifier, info);
  }
  sessionManager_->createSession(info.notifyTo.Address());

  WS::EVENTING::SubscribeResponse::SubscriptionManagerType subscriptionManager(
      WS::ADDRESSING::EndpointReferenceType(
          WS::ADDRESSING::URIType("To be filled by Soap Service")));
  subscriptionManager.ReferenceParameters() = WS::ADDRESSING::ReferenceParametersType(identifier);
  WS::EVENTING::SubscribeResponse subscribeResponse(subscriptionManager,
                                                    WS::EVENTING::ExpirationType(duration.str()));
  LOG(LogLevel::INFO, "Successfully created subscription for " << identifier);
  printSubscriptions();
  return subscribeResponse;
}

WS::EVENTING::RenewResponse
SubscriptionManager::dispatch(const WS::EVENTING::Renew& renewRequest,
                              const WS::EVENTING::Identifier& identifier)
{
  std::lock_guard<std::mutex> lock(subscriptionMutex_);
  const Duration duration(renewRequest.Expires().value_or(WS::EVENTING::ExpirationType("PT1H")));
  auto subscriptionInfo = subscriptions_.find(identifier);
  if (subscriptionInfo == subscriptions_.end())
  {
    throw std::runtime_error("Could not find subscription corresponding to Renew Identifier " +
                             identifier);
  }
  subscriptionInfo->second.expirationTime = duration.toExpirationTimePoint();
  WS::EVENTING::RenewResponse renewResponse;
  renewResponse.Expires() = WS::EVENTING::ExpirationType(duration.str());
  LOG(LogLevel::INFO, "Successfully renewed subscription for " << identifier);
  printSubscriptions();
  return renewResponse;
}

void SubscriptionManager::dispatch(const WS::EVENTING::Unsubscribe& /*unsubscribeRequest*/,
                                   const WS::EVENTING::Identifier& identifier)
{
  std::lock_guard<std::mutex> lock(subscriptionMutex_);
  auto subscriptionInfo = subscriptions_.find(identifier);
  if (subscriptionInfo == subscriptions_.end())
  {
    throw std::runtime_error("Could not find subscription corresponding to Renew Identifier " +
                             identifier);
  }
  const auto& notifyTo = subscriptionInfo->second.notifyTo.Address();
  const auto numSameClient =
      std::count_if(subscriptions_.begin(), subscriptions_.end(),
                    [&](const auto& it) { return it.second.notifyTo.Address() == notifyTo; });

  if (numSameClient == 1)
  {
    sessionManager_->deleteSession(notifyTo);
  }
  subscriptions_.erase(subscriptionInfo);
  printSubscriptions();
}

void SubscriptionManager::fireEvent(const BICEPS::MM::EpisodicMetricReport& report)
{
  LOG(LogLevel::DEBUG, "Fire Event: EpisodicMetricReport");
  std::lock_guard<std::mutex> lock(subscriptionMutex_);
  std::vector<const SubscriptionInformation*> subscriber;
  for (const auto& [id, info] : subscriptions_)
  {
    if (std::find(info.filter.begin(), info.filter.end(), SDC::ACTION_EPISODIC_METRIC_REPORT) !=
        info.filter.end())
    {
      subscriber.emplace_back(&info);
    }
  }
  if (subscriber.empty())
  {
    return;
  }
  MESSAGEMODEL::Header header;
  header.MessageID() = MESSAGEMODEL::Header::MessageIDType(MicroSDC::calculateMessageID());
  header.Action() = WS::ADDRESSING::URIType(SDC::ACTION_EPISODIC_METRIC_REPORT);

  MESSAGEMODEL::Body body;
  body.EpisodicMetricReport() = report;

  MESSAGEMODEL::Envelope notifyEnvelope;
  notifyEnvelope.Header() = std::move(header);
  notifyEnvelope.Body() = std::move(body);

  MessageSerializer serializer;
  serializer.serialize(notifyEnvelope);
  const auto messageStr = serializer.str();
  LOG(LogLevel::DEBUG, "SENDING: " << messageStr);
  for (const auto* const info : subscriber)
  {
    sessionManager_->sendToSession(info->notifyTo.Address(), messageStr);
  }
}

void SubscriptionManager::printSubscriptions() const
{
  std::stringstream out;
  out << "Subscriptions:\n";
  for (const auto& [key, val] : subscriptions_)
  {
    out << key << " : " << val.notifyTo.Address() << " : "
        << std::chrono::duration_cast<std::chrono::seconds>(val.expirationTime -
                                                            std::chrono::system_clock::now())
               .count()
        << "s : ";
    for (const auto& action : val.filter)
    {
      out << action << " ";
    }
    out << "\n";
  }
  out << std::endl;
  LOG(LogLevel::DEBUG, out.str());
}
