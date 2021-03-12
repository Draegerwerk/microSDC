#include "ws-addressing.hpp"

#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include <cstring>
#include <utility>

namespace WS::EVENTING
{
  Identifier::Identifier(const rapidxml::xml_node<>& node)
  {
    parse(node);
  }

  Identifier::Identifier(std::string x)
    : std::string(std::move(x))
  {
  }

  void Identifier::parse(const rapidxml::xml_node<>& node)
  {
    *this = std::string(node.value(), node.value_size());
    const auto* is_reference_parameter_node = node.first_attribute("IsReferenceParameter");
    if (is_reference_parameter_node != nullptr)
    {
      if (strncmp(is_reference_parameter_node->value(), "true",
                  is_reference_parameter_node->name_size()) == 0)
      {
        is_reference_parameter = true;
      }
      else if (strncmp(is_reference_parameter_node->value(), "false",
                       is_reference_parameter_node->name_size()) == 0)
      {
        is_reference_parameter = false;
      }
    }
  }
} // namespace WS::EVENTING

namespace WS::ADDRESSING
{
  // URIType
  //
  URIType::URIType(const rapidxml::xml_node<>& node)
    : std::string{node.value(), node.value_size()}
  {
  }
  URIType::URIType(std::string other)
    : std::string(std::move(other))
  {
  }

  // EndpointReferenceType
  //
  EndpointReferenceType::EndpointReferenceType(AddressType address)
    : address(std::move(address))
  {
  }
  EndpointReferenceType::EndpointReferenceType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void EndpointReferenceType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* address_node = node.first_node("Address", MDPWS::WS_NS_ADDRESSING);
    if (address_node == nullptr)
    {
      throw ExpectedElement("Address", MDPWS::WS_NS_ADDRESSING);
    }
    address = URIType{address_node->value(), address_node->value_size()};
  }

  // RelatesToType
  //
  RelatesToType::RelatesToType(URIType x)
    : URIType(std::move(x))
  {
  }

  // ReferenceParametersType
  //
  ReferenceParametersType::ReferenceParametersType(const IdentifierType& identifier)
    : identifier(identifier)
  {
  }

} // namespace WS::ADDRESSING
