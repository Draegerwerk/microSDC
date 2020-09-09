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
    const auto* isReferenceParameterNode = node.first_attribute("IsReferenceParameter");
    if (isReferenceParameterNode != nullptr)
    {
      if (strncmp(isReferenceParameterNode->value(), "true",
                  isReferenceParameterNode->name_size()) == 0)
      {
        IsReferenceParameter = true;
      }
      else if (strncmp(isReferenceParameterNode->value(), "false",
                       isReferenceParameterNode->name_size()) == 0)
      {
        IsReferenceParameter = false;
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
    : Address(std::move(address))
  {
  }
  EndpointReferenceType::EndpointReferenceType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void EndpointReferenceType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* addressNode = node.first_node("Address", MDPWS::WS_NS_ADDRESSING);
    if (addressNode == nullptr)
    {
      throw ExpectedElement("Address", MDPWS::WS_NS_ADDRESSING);
    }
    Address = URIType{addressNode->value(), addressNode->value_size()};
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
    : Identifier(identifier)
  {
  }

} // namespace WS::ADDRESSING
