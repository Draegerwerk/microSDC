#include "SubscriptionManager.hpp"
#include "MicroSDC.hpp"
#include "SDCConstants.hpp"
#include "UUIDGenerator.hpp"
#include "datamodel/MessageModel.hpp"
#include "datamodel/MessageSerializer.hpp"
#include "datamodel/ws-addressing.hpp"
#include "datamodel/xs_duration.hpp"
#include "esp_http_client.h"
#include "esp_log.h"
#include <algorithm>

static constexpr const char* TAG = "SubscriptionManager";

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

  {
    std::lock_guard<std::mutex> lock(subscriptionMutex_);
    subscriptions_.emplace(identifier, info);
  }
  createClient(info.notifyTo.Address());

  WS::EVENTING::SubscribeResponse::SubscriptionManagerType subscriptionManager(
      WS::ADDRESSING::EndpointReferenceType(
          WS::ADDRESSING::URIType("To be filled by Soap Service")));
  subscriptionManager.ReferenceParameters() = WS::ADDRESSING::ReferenceParametersType(identifier);
  WS::EVENTING::SubscribeResponse subscribeResponse(subscriptionManager,
                                                    WS::EVENTING::ExpirationType(duration.str()));
  ESP_LOGI(TAG, "Successfully created subscription for %s", identifier.c_str());
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
  ESP_LOGI(TAG, "Successfully renewed subscription for %s", identifier.c_str());
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
    auto clientSession = clients_.at(notifyTo);
    auto err = esp_http_client_cleanup(clientSession);
    if (err == ESP_OK)
    {
      ESP_LOGI(TAG, "Cleaned up client session for address %s", notifyTo.c_str());
    }
    else
    {
      ESP_LOGE(TAG, "Error cleaning up client session %s", esp_err_to_name(err));
    }
  }
  subscriptions_.erase(subscriptionInfo);
  printSubscriptions();
}

void SubscriptionManager::fireEvent(const BICEPS::MM::EpisodicMetricReport& report)
{
  ESP_LOGD(TAG, "Fire Event: EpisodicMetricReport");
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
  ESP_LOGD(TAG, "SENDING: %s", messageStr.c_str());
  for (const auto info : subscriber)
  {
    auto clientIt = clients_.find(info->notifyTo.Address());
    if (clientIt == clients_.end())
    {
      ESP_LOGE(TAG, "Cannot find client session with address %s!",
               info->notifyTo.Address().c_str());
      continue;
    }
    ESP_LOGI(TAG, "Sending to %s", clientIt->first.c_str());
    esp_http_client_set_post_field(clientIt->second, messageStr.c_str(), messageStr.length());
    esp_err_t err = esp_http_client_perform(clientIt->second);
    if (err == ESP_OK)
    {
      ESP_LOGD(TAG, "HTTPS Status = %d, content_length = %d",
               esp_http_client_get_status_code(clientIt->second),
               esp_http_client_get_content_length(clientIt->second));
    }
    else
    {
      ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
  }
}

void SubscriptionManager::createClient(const std::string& notifyTo)
{
  if (clients_.count(notifyTo) != 0)
  {
    ESP_LOGI(TAG, "Client session already exists");
    return;
  }
  extern const char serverCert_pem_start[] asm("_binary_serverCert_pem_start");
  extern const char serverKey_pem_start[] asm("_binary_serverKey_pem_start");
  esp_http_client_config_t config;
  config.url = notifyTo.c_str();
  config.host = nullptr;
  config.port = 0;
  config.username = nullptr;
  config.password = nullptr;
  config.auth_type = HTTP_AUTH_TYPE_NONE;
  config.path = nullptr;
  config.query = nullptr;
  config.cert_pem = nullptr;
  config.client_cert_pem = serverCert_pem_start;
  config.client_key_pem = serverKey_pem_start;
  config.method = HTTP_METHOD_POST;
  config.timeout_ms = 0;
  config.disable_auto_redirect = false;
  config.max_redirection_count = 0;
  config.event_handler = nullptr;
  config.transport_type = HTTP_TRANSPORT_OVER_TCP;
  config.buffer_size = 0;
  config.buffer_size_tx = 0;
  config.user_data = nullptr;
  config.is_async = false;
  config.use_global_ca_store = true;
  config.skip_cert_common_name_check = true;
  auto client = esp_http_client_init(&config);
  {
    std::lock_guard<std::mutex> lock(subscriptionMutex_);
    clients_.emplace(notifyTo, client);
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
  ESP_LOGD(TAG, "%s", out.str().c_str());
}
