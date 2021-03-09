#include "SubscriptionManager.hpp"

#include "Log.hpp"
#include "MicroSDC.hpp"
#include "SDCConstants.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "datamodel/ws-addressing.hpp"
#include "datamodel/xs_duration.hpp"
#include "uuid/UUIDGenerator.hpp"
#include <algorithm>

static constexpr const char* TAG = "SubscriptionManager";

SubscriptionManager::SubscriptionManager(const bool use_tls)
  : session_manager_(use_tls)
{
}

WS::EVENTING::SubscribeResponse
SubscriptionManager::dispatch(const WS::EVENTING::Subscribe& subscribe_request)
{
  if (!subscribe_request.filter.has_value())
  {
    throw std::runtime_error("No filter specified");
  }
  for (const auto& filter_action : subscribe_request.filter.value())
  {
    if (std::find(allowed_subscription_event_actions_.begin(),
                  allowed_subscription_event_actions_.end(),
                  filter_action) == allowed_subscription_event_actions_.end())
    {
      throw std::runtime_error("Unknown event action");
    }
  }
  const auto identifier = "uuid:" + UUIDGenerator{}().to_string();

  const Duration duration(subscribe_request.expires.value_or(WS::EVENTING::ExpirationType(
      Duration(Duration::Years{0}, Duration::Months{0}, Duration::Days{0}, Duration::Hours{1},
               Duration::Minutes{0}, Duration::Seconds{0}, false))));
  const auto expires = duration.toExpirationTimePoint();
  SubscriptionInformation info{subscribe_request.delivery.notifyTo,
                               subscribe_request.filter.value(), expires};

  {
    std::lock_guard<std::mutex> lock(subscription_mutex_);
    subscriptions_.emplace(identifier, info);
  }
  session_manager_.create_session(info.notify_to.address);

  WS::EVENTING::SubscribeResponse::SubscriptionManagerType subscription_manager(
      WS::ADDRESSING::EndpointReferenceType(
          WS::ADDRESSING::URIType("To be filled by Soap Service")));
  subscription_manager.referenceParameters =
      WS::ADDRESSING::ReferenceParametersType(WS::EVENTING::Identifier{identifier});
  WS::EVENTING::SubscribeResponse subscribe_response(
      subscription_manager, WS::EVENTING::SubscribeResponse::ExpiresType{duration});
  LOG(LogLevel::INFO, "Successfully created subscription for " << identifier);
  print_subscriptions();
  return subscribe_response;
}

WS::EVENTING::RenewResponse
SubscriptionManager::dispatch(const WS::EVENTING::Renew& renew_request,
                              const WS::EVENTING::Identifier& identifier)
{
  std::lock_guard<std::mutex> lock(subscription_mutex_);
  const Duration duration(renew_request.expires.value_or(WS::EVENTING::ExpirationType("PT1H")));
  auto subscription_info = subscriptions_.find(identifier);
  if (subscription_info == subscriptions_.end())
  {
    throw std::runtime_error("Could not find subscription corresponding to Renew Identifier " +
                             identifier);
  }
  subscription_info->second.expiration_time = duration.toExpirationTimePoint();
  WS::EVENTING::RenewResponse renew_response;
  renew_response.expires = WS::EVENTING::RenewResponse::ExpiresType{duration};
  LOG(LogLevel::INFO, "Successfully renewed subscription for " << identifier);
  print_subscriptions();
  return renew_response;
}

void SubscriptionManager::dispatch(const WS::EVENTING::Unsubscribe& /*unsubscribeRequest*/,
                                   const WS::EVENTING::Identifier& identifier)
{
  std::lock_guard<std::mutex> lock(subscription_mutex_);
  auto subscription_info = subscriptions_.find(identifier);
  if (subscription_info == subscriptions_.end())
  {
    throw std::runtime_error("Could not find subscription corresponding to Renew Identifier " +
                             identifier);
  }
  const auto& notify_to = subscription_info->second.notify_to.address;
  const auto num_same_client =
      std::count_if(subscriptions_.begin(), subscriptions_.end(), [notify_to](const auto& it) {
        return it.second.notify_to.address == notify_to;
      });

  if (num_same_client == 1)
  {
    session_manager_.delete_session(notify_to);
  }
  subscriptions_.erase(subscription_info);
  print_subscriptions();
}

void SubscriptionManager::fire_event(const BICEPS::MM::EpisodicMetricReport& report)
{
  LOG(LogLevel::DEBUG, "Fire Event: EpisodicMetricReport");
  std::lock_guard<std::mutex> lock(subscription_mutex_);
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
  header.messageID = MESSAGEMODEL::Header::MessageIDType(MicroSDC::calculate_message_id());
  header.action = WS::ADDRESSING::URIType(SDC::ACTION_EPISODIC_METRIC_REPORT);

  MESSAGEMODEL::Body body;
  body.episodicMetricReport = report;

  MESSAGEMODEL::Envelope notify_envelope;
  notify_envelope.header = std::move(header);
  notify_envelope.body = std::move(body);

  MessageSerializer serializer;
  serializer.serialize(notify_envelope);
  const auto message_str = serializer.str();
  LOG(LogLevel::DEBUG, "SENDING: " << message_str);
  for (const auto* const info : subscriber)
  {
    session_manager_.send_to_session(info->notify_to.address, message_str);
  }
}

void SubscriptionManager::print_subscriptions() const
{
  std::stringstream out;
  out << "Subscriptions:\n";
  for (const auto& [key, val] : subscriptions_)
  {
    out << key << " : " << val.notify_to.address << " : "
        << std::chrono::duration_cast<std::chrono::seconds>(val.expiration_time -
                                                            std::chrono::steady_clock::now())
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
