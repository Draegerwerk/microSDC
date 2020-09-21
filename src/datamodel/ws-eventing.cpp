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
  DeliveryType::DeliveryType(NotifyToType notifyTo)
    : NotifyTo(std::move(notifyTo))
  {
  }
  DeliveryType::DeliveryType(const rapidxml::xml_node<>& node)
    : NotifyTo(WS::ADDRESSING::URIType(""))
  {
    this->parse(node);
  }
  void DeliveryType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* nodeAttr = node.first_attribute("Mode");
    if (nodeAttr == nullptr || (nodeAttr->value() != nullptr &&
                                strncmp(nodeAttr->value(), MDPWS::WS_EVENTING_DELIVERYMODE_PUSH,
                                        nodeAttr->value_size()) == 0))
    {
      Mode = ::MDPWS::WS_EVENTING_DELIVERYMODE_PUSH;
    }
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() != nullptr && strncmp(entry->name(), "NotifyTo", entry->name_size()) == 0 &&
          entry->xmlns() != nullptr &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        NotifyTo = NotifyToType(*entry);
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
    : Dialect(std::move(dialect))
  {
  }
  FilterType::FilterType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void FilterType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* dialectAttr = node.first_attribute("Dialect");
    if (dialectAttr == nullptr ||
        (dialectAttr != nullptr && strncmp(dialectAttr->value(), MDPWS::WS_EVENTING_FILTER_ACTION,
                                           dialectAttr->value_size()) != 0))
    {
      throw ExpectedElement("Dialect", MDPWS::WS_EVENTING_FILTER_ACTION);
    }
    Dialect = MDPWS::WS_EVENTING_FILTER_ACTION;
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
    : Delivery(std::move(delivery))
  {
  }
  Subscribe::Subscribe(const rapidxml::xml_node<>& node)
    : Delivery(WS::ADDRESSING::EndpointReferenceType(WS::ADDRESSING::URIType("")))
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
        EndTo = std::make_optional<EndToType>(*entry);
      }
      else if (strncmp(entry->name(), "Delivery", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Delivery = DeliveryType(*entry);
      }
      else if (strncmp(entry->name(), "Expires", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Expires =
            std::make_optional<ExpirationType>(std::string(entry->value(), entry->value_size()));
      }
      else if (strncmp(entry->name(), "Filter", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Filter = std::make_optional<FilterType>(*entry);
      }
    }
  }

  // SubscribeResponse
  //
  SubscribeResponse::SubscribeResponse(SubscriptionManagerType subscriptionManager,
                                       ExpiresType expires)
    : SubscriptionManager(std::move(subscriptionManager))
    , Expires(std::move(expires))
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
    const auto* expiresNode = node.first_node("Expires", MDPWS::WS_NS_EVENTING);
    if (expiresNode != nullptr)
    {
      Expires = ExpiresType(std::string{expiresNode->value(), expiresNode->value_size()});
    }
  }

  // Unsubscribe
  //
  Unsubscribe::Unsubscribe(const rapidxml::xml_node<>& node) {}

} // namespace WS::EVENTING
