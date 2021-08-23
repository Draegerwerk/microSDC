#pragma once

#include "ws-addressing.hpp"
#include "xs_duration.hpp"
#include <memory>
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace WS::EVENTING
{
  struct DeliveryType
  {
  public:
    using NotifyToType = WS::ADDRESSING::EndpointReferenceType;
    NotifyToType notify_to;

    using ModeType = std::string;
    using ModeOptional = boost::optional<ModeType>;
    ModeOptional mode;

    explicit DeliveryType(NotifyToType notify_to);
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
    DialectType dialect;

    explicit FilterType(DialectType dialect);
    explicit FilterType(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct Subscribe
  {
  public:
    using EndToType = WS::ADDRESSING::EndpointReferenceType;
    using EndToOptional = boost::optional<EndToType>;
    EndToOptional end_to;

    using DeliveryType = WS::EVENTING::DeliveryType;
    DeliveryType delivery;

    using ExpirationType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = boost::optional<ExpirationType>;
    ExpiresOptional expires;

    using FilterType = WS::EVENTING::FilterType;
    using FilterOptional = boost::optional<FilterType>;
    FilterOptional filter;

    explicit Subscribe(DeliveryType delivery);
    explicit Subscribe(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct SubscribeResponse
  {
  public:
    using SubscriptionManagerType = WS::ADDRESSING::EndpointReferenceType;
    SubscriptionManagerType subscription_manager;

    using ExpiresType = ExpirationType;
    ExpiresType expires;

    SubscribeResponse(SubscriptionManagerType subscription_manager, ExpiresType expires);
  };

  struct Renew
  {
  public:
    using ExpiresType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = boost::optional<ExpiresType>;
    ExpiresOptional expires;

    explicit Renew(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct RenewResponse
  {
    using ExpiresType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = boost::optional<ExpiresType>;
    ExpiresOptional expires;
  };

  struct Unsubscribe
  {
    explicit Unsubscribe(const rapidxml::xml_node<>& node);
    // TODO
  };

} // namespace WS::EVENTING
