#include "ws-eventing.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include <cstring>
#include <sstream>

namespace WS::EVENTING
{

  // DeliveryType
  //
  DeliveryType::DeliveryType(const NotifyToType& notifyTo)
    : NotifyTo_(notifyTo)
  {
  }
  DeliveryType::DeliveryType(const rapidxml::xml_node<>& node)
    : NotifyTo_(WS::ADDRESSING::URIType(""))
  {
    this->parse(node);
  }
  void DeliveryType::parse(const rapidxml::xml_node<>& node)
  {
    auto nodeAttr = node.first_attribute("Mode", 0);
    if (nodeAttr == nullptr || strncmp(nodeAttr->value(), ::MDPWS::WS_EVENTING_DELIVERYMODE_PUSH,
                                       nodeAttr->value_size()) == 0)
    {
      Mode_ = ::MDPWS::WS_EVENTING_DELIVERYMODE_PUSH;
    }
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (strncmp(entry->name(), "NotifyTo", entry->name_size()) == 0 &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        NotifyTo_ = NotifyToType(*entry);
      }
    }
  }
  const DeliveryType::NotifyToType& DeliveryType::NotifyTo() const
  {
    return NotifyTo_;
  }
  DeliveryType::NotifyToType& DeliveryType::NotifyTo()
  {
    return NotifyTo_;
  }
  const DeliveryType::ModeOptional& DeliveryType::Mode() const
  {
    return Mode_;
  }
  DeliveryType::ModeOptional& DeliveryType::Mode()
  {
    return Mode_;
  }

  // DeliveryType
  //
  ExpirationType::ExpirationType(std::string expiration)
    : std::string(expiration)
  {
  }

  // FilterType
  //
  FilterType::FilterType(const DialectType& dialect)
    : Dialect_(dialect)
  {
  }
  FilterType::FilterType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void FilterType::parse(const rapidxml::xml_node<>& node)
  {
    auto dialectAttr = node.first_attribute("Dialect", 0);
    if (dialectAttr == nullptr || strncmp(dialectAttr->value(), ::MDPWS::WS_EVENTING_FILTER_ACTION,
                                          dialectAttr->value_size()) != 0)
    {
      throw ExpectedElement("Dialect", ::MDPWS::WS_EVENTING_FILTER_ACTION);
    }
    Dialect_ = ::MDPWS::WS_EVENTING_FILTER_ACTION;
    if (node.value() != nullptr)
    {
      // extract white space delimited filters
      std::string filters(node.value(), node.value_size());
      std::istringstream stringStream(filters);
      do
      {
        std::string subString;
        stringStream >> subString;
        this->emplace_back(subString);
      } while (stringStream);
    }
  }
  const FilterType::DialectType& FilterType::Dialect() const
  {
    return Dialect_;
  }
  FilterType::DialectType& FilterType::Dialect()
  {
    return Dialect_;
  }

  // Subscribe
  //
  Subscribe::Subscribe(const DeliveryType& delivery)
    : Delivery_(delivery)
  {
  }
  Subscribe::Subscribe(const rapidxml::xml_node<>& node)
    : Delivery_(WS::ADDRESSING::EndpointReferenceType(WS::ADDRESSING::URIType("")))
  {
    this->parse(node);
  }
  void Subscribe::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (strncmp(entry->name(), "EndTo", entry->name_size()) == 0 &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        EndTo_ = std::make_optional<EndToType>(*entry);
      }
      else if (strncmp(entry->name(), "Delivery", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Delivery_ = DeliveryType(*entry);
      }
      else if (strncmp(entry->name(), "Expires", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Expires_ =
            std::make_optional<ExpirationType>(std::string(entry->value(), entry->value_size()));
      }
      else if (strncmp(entry->name(), "Filter", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Filter_ = std::make_optional<FilterType>(*entry);
      }
    }
  }
  const Subscribe::EndToOptional& Subscribe::EndTo() const
  {
    return EndTo_;
  }
  Subscribe::EndToOptional& Subscribe::EndTo()
  {
    return EndTo_;
  }
  const Subscribe::DeliveryType& Subscribe::Delivery() const
  {
    return Delivery_;
  }
  Subscribe::DeliveryType& Subscribe::Delivery()
  {
    return Delivery_;
  }
  const Subscribe::ExpiresOptional& Subscribe::Expires() const
  {
    return Expires_;
  }
  Subscribe::ExpiresOptional& Subscribe::Expires()
  {
    return Expires_;
  }
  const Subscribe::FilterOptional& Subscribe::Filter() const
  {
    return Filter_;
  }
  Subscribe::FilterOptional& Subscribe::Filter()
  {
    return Filter_;
  }

  // SubscribeResponse
  //
  SubscribeResponse::SubscribeResponse(const SubscriptionManagerType& subscriptionManager,
                                       const ExpiresType& expires)
    : SubscriptionManager_(subscriptionManager)
    , Expires_(expires)
  {
  }

} // namespace WS::EVENTING
