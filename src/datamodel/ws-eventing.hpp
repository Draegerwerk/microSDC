#pragma once

#include "ws-addressing.hpp"
#include "xs_duration.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace WS::EVENTING
{
  struct DeliveryType
  {
  public:
    using NotifyToType = WS::ADDRESSING::EndpointReferenceType;
    NotifyToType NotifyTo;

    using ModeType = std::string;
    using ModeOptional = std::optional<ModeType>;
    ModeOptional Mode;

    explicit DeliveryType(NotifyToType notifyTo);
    explicit DeliveryType(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct ExpirationType : public Duration
  {
    using Duration::Duration;
    explicit ExpirationType(const Duration& duration);
  };

  using ActionList = std::vector<std::string>;

  struct FilterType : public ActionList
  {
  public:
    using DialectType = std::string;
    DialectType Dialect;

    explicit FilterType(DialectType dialect);
    explicit FilterType(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct Subscribe
  {
  public:
    using EndToType = WS::ADDRESSING::EndpointReferenceType;
    using EndToOptional = std::optional<EndToType>;
    EndToOptional EndTo;

    using DeliveryType = WS::EVENTING::DeliveryType;
    DeliveryType Delivery;

    using ExpirationType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = std::optional<ExpirationType>;
    ExpiresOptional Expires;

    using FilterType = WS::EVENTING::FilterType;
    using FilterOptional = std::optional<FilterType>;
    FilterOptional Filter;

    explicit Subscribe(DeliveryType delivery);
    explicit Subscribe(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct SubscribeResponse
  {
  public:
    using SubscriptionManagerType = WS::ADDRESSING::EndpointReferenceType;
    SubscriptionManagerType SubscriptionManager;

    using ExpiresType = ExpirationType;
    ExpiresType Expires;

    SubscribeResponse(SubscriptionManagerType subscriptionManager, ExpiresType expires);
  };

  struct Renew
  {
  public:
    using ExpiresType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = std::optional<ExpiresType>;
    ExpiresOptional Expires;

    explicit Renew(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct RenewResponse
  {
    using ExpiresType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = std::optional<ExpiresType>;
    ExpiresOptional Expires;
  };

  struct Unsubscribe
  {
    explicit Unsubscribe(const rapidxml::xml_node<>& node);
    // TODO
  };

} // namespace WS::EVENTING
