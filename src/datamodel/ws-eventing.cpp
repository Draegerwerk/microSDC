#include "ws-eventing.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include <cstring>
#include <sstream>
#include <utility>

namespace WS::EVENTING
{

  // DeliveryType
  //
  DeliveryType::DeliveryType(NotifyToType notify_to)
    : notify_to(std::move(notify_to))
  {
  }
  DeliveryType::DeliveryType(const rapidxml::xml_node<>& node)
    : notify_to(WS::ADDRESSING::URIType(""))
  {
    this->parse(node);
  }
  void DeliveryType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* node_attr = node.first_attribute("Mode");
    if (node_attr == nullptr || (node_attr->value() != nullptr &&
                                 strncmp(node_attr->value(), MDPWS::WS_EVENTING_DELIVERYMODE_PUSH,
                                         node_attr->value_size()) == 0))
    {
      mode = ::MDPWS::WS_EVENTING_DELIVERYMODE_PUSH;
    }
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() != nullptr && strncmp(entry->name(), "NotifyTo", entry->name_size()) == 0 &&
          entry->xmlns() != nullptr &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        notify_to = NotifyToType(*entry);
      }
    }
  }

  ExpirationType::ExpirationType(const Duration& duration)
    : Duration(duration)
  {
  }

  // FilterType
  //
  FilterType::FilterType(DialectType dialect)
    : dialect(std::move(dialect))
  {
  }
  FilterType::FilterType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void FilterType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* dialect_attr = node.first_attribute("Dialect");
    if (dialect_attr == nullptr || strncmp(dialect_attr->value(), MDPWS::WS_EVENTING_FILTER_ACTION,
                                           dialect_attr->value_size()) != 0)
    {
      throw ExpectedElement("Dialect", MDPWS::WS_EVENTING_FILTER_ACTION);
    }
    dialect = MDPWS::WS_EVENTING_FILTER_ACTION;
    if (node.value() != nullptr)
    {
      // extract white space delimited filters
      std::string filters(node.value(), node.value_size());
      std::istringstream iss(filters);
      for (std::string s; iss >> s;)
      {
        this->emplace_back(s);
      }
    }
  }

  // Subscribe
  //
  Subscribe::Subscribe(DeliveryType delivery)
    : delivery(std::move(delivery))
  {
  }
  Subscribe::Subscribe(const rapidxml::xml_node<>& node)
    : delivery(WS::ADDRESSING::EndpointReferenceType(WS::ADDRESSING::URIType("")))
  {
    this->parse(node);
  }
  void Subscribe::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() == nullptr || entry->xmlns() == nullptr)
      {
        continue;
      }
      if (strncmp(entry->name(), "EndTo", entry->name_size()) == 0 &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        end_to = std::make_optional<EndToType>(*entry);
      }
      else if (strncmp(entry->name(), "Delivery", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        delivery = DeliveryType(*entry);
      }
      else if (strncmp(entry->name(), "Expires", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        expires =
            std::make_optional<ExpirationType>(std::string(entry->value(), entry->value_size()));
      }
      else if (strncmp(entry->name(), "Filter", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        filter = std::make_optional<FilterType>(*entry);
      }
    }
  }

  // SubscribeResponse
  //
  SubscribeResponse::SubscribeResponse(SubscriptionManagerType subscription_manager,
                                       ExpiresType expires)
    : subscription_manager(std::move(subscription_manager))
    , expires(expires)
  {
  }

  // Renew
  //
  Renew::Renew(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void Renew::parse(const rapidxml::xml_node<>& node)
  {
    const auto* expires_node = node.first_node("Expires", MDPWS::WS_NS_EVENTING);
    if (expires_node != nullptr)
    {
      expires = ExpiresType(std::string{expires_node->value(), expires_node->value_size()});
    }
  }

  // Unsubscribe
  //
  Unsubscribe::Unsubscribe(const rapidxml::xml_node<>& node) {}

} // namespace WS::EVENTING
