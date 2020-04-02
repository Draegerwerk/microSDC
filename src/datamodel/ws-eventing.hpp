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
    explicit DeliveryType(const rapidxml::xml_node<>& node);

  private:
    NotifyToType NotifyTo_;
    ModeOptional Mode_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class ExpirationType : public std::string
  {
  public:
    explicit ExpirationType(std::string expiration);
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
    explicit FilterType(const rapidxml::xml_node<>& node);

  private:
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
    explicit Subscribe(const rapidxml::xml_node<>& node);

  private:
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

  private:
    SubscriptionManagerType SubscriptionManager_;
    ExpiresType Expires_;
  };

  class Renew
  {
  public:
    explicit Renew(const rapidxml::xml_node<>& node);
    using ExpiresType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = std::optional<ExpiresType>;
    const ExpiresOptional& Expires() const;
    ExpiresOptional& Expires();

  private:
    ExpiresOptional Expires_;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  class RenewResponse
  {
  public:
    using ExpiresType = WS::EVENTING::ExpirationType;
    using ExpiresOptional = std::optional<ExpiresType>;
    const ExpiresOptional& Expires() const;
    ExpiresOptional& Expires();

  private:
    ExpiresOptional Expires_;
  };

  class Unsubscribe
  {
  public:
    explicit Unsubscribe(const rapidxml::xml_node<>& node);
  };

} // namespace WS::EVENTING
