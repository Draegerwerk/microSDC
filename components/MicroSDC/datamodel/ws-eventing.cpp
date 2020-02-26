#include "ws-eventing.hpp"

namespace WS::EVENTING
{

  // DeliveryType
  //
  DeliveryType::DeliveryType(const NotifyToType& notifyTo)
    : NotifyTo_(notifyTo)
  {
  }
  DeliveryType::DeliveryType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void DeliveryType::parse(const rapidxml::xml_node<>& node)
  {
    // TODO:: parse
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
    // TODO:: parse 
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
  {
    this->parse(node);
  }
  void Subscribe::parse(const rapidxml::xml_node<>& node)
  {
    // TODO: parse
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

} // namespace WS::EVENTING
