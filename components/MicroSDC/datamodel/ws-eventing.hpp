#pragma once

//#include "datamodel/ws-addressing.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace WS::EVENTING
{
  class Identifier : public std::string
  {
  public:
    using IsReferenceParameterType = bool;
    using IsReferenceParameterOptional = std::optional<IsReferenceParameterType>;

  protected:
    IsReferenceParameterOptional IsReferenceParameter_;
  };

  /*
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

  protected:
    NotifyToType NotifyTo_;
    ModeOptional Mode_;
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

  protected:
    DialectType Dialect_;
  };

  class Subscribe
  {
  public:
    using EndToType = WS::ADDRESSING::EndpointReferenceType;
    using EndToOptional = std::optional<EndToType>;
    const EndToOptional& EndTo() const;
    EndToOptional& EndTo();

    using DeliveryType = DeliveryType;
    const DeliveryType& Delivery() const;
    DeliveryType& Delivery();

    using ExpirationType = ExpirationType;
    using ExpiresOptional = std::optional<ExpirationType>;
    const ExpiresOptional& Expires() const;
    ExpiresOptional& Expires();

    using FilterType = FilterType;
    using FilterOptional = std::optional<FilterType>;
    const FilterOptional& Filter() const;
    FilterOptional& Filter();

    explicit Subscribe(const DeliveryType& delivery);

  protected:
    EndToOptional EndTo_;
    DeliveryType Delivery_;
    ExpiresOptional Expires_;
    FilterOptional Filter_;
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
  */
} // namespace WS::EVENTING
