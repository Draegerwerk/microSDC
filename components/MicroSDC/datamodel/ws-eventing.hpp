#pragma once

#include "ws-addressing.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace WS::EVENTING
{
  class DeliveryType
  {
  public:
    using NotifyToType = WS::ADDRESSING::EndpointReferenceType;
    const NotifyToType& NotifyTo() const;
    NotifyToType& NotifyTo();

    using ModeType = std::string;
    using ModeOptional = std::optional<ModeType>;
    const ModeOptional& Mode() const;
    ModeOptional& Mode();

    explicit DeliveryType(const NotifyToType& notifyTo);
    DeliveryType(const rapidxml::xml_node<>& node);
    DeliveryType() = default;

  protected:
    NotifyToType NotifyTo_;
    ModeOptional Mode_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class ExpirationType : public std::string
  {
  };

  class ActionList : public std::vector<std::string>
  {
  };

  class FilterType : public ActionList
  {
  public:
    using DialectType = std::string;
    const DialectType& Dialect() const;
    DialectType& Dialect();

    explicit FilterType(const DialectType& dialect);
    FilterType(const rapidxml::xml_node<>& node);

  protected:
    DialectType Dialect_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class Subscribe
  {
  public:
    using EndToType = WS::ADDRESSING::EndpointReferenceType;
    using EndToOptional = std::optional<EndToType>;
    const EndToOptional& EndTo() const;
    EndToOptional& EndTo();

    using DeliveryType = WS::EVENTING::DeliveryType;
    const DeliveryType& Delivery() const;
    DeliveryType& Delivery();

    using ExpirationType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = std::optional<ExpirationType>;
    const ExpiresOptional& Expires() const;
    ExpiresOptional& Expires();

    using FilterType = WS::EVENTING::FilterType;
    using FilterOptional = std::optional<FilterType>;
    const FilterOptional& Filter() const;
    FilterOptional& Filter();

    explicit Subscribe(const DeliveryType& delivery);
    Subscribe(const rapidxml::xml_node<>& node);
    Subscribe() = default;

  protected:
    EndToOptional EndTo_;
    DeliveryType Delivery_;
    ExpiresOptional Expires_;
    FilterOptional Filter_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class SubscribeResponse
  {
  public:
    using SubscriptionManagerType = WS::ADDRESSING::EndpointReferenceType;
    const SubscriptionManagerType& SubscriptionManager() const;
    SubscriptionManagerType& SubscriptionManager();

    using ExpiresType = ExpirationType;
    const ExpiresType& Expires() const;
    ExpiresType& Expires();

    SubscribeResponse(const SubscriptionManagerType& subscriptionManager,
                      const ExpiresType& expires);

  protected:
    SubscriptionManagerType SubscriptionManager_;
    ExpiresType Expires_;
  };

} // namespace WS::EVENTING
