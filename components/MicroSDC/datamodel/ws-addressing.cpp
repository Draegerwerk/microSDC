#include "ws-addressing.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"

namespace WS::ADDRESSING
{
  URIType::URIType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  URIType::URIType(std::string uri)
    : uri_(std::move(uri))
  {
  }
  const std::string& URIType::uri() const
  {
    return uri_;
  }
  void URIType::parse(const rapidxml::xml_node<>& node)
  {
    uri_ = {node.value(), node.value_size()};
  }

  EndpointReferenceType::EndpointReferenceType(const EndpointReferenceType& epr)
    : Address_(epr.Address_)
  {
  }
  EndpointReferenceType::EndpointReferenceType(const AddressType& address)
    : Address_(address)
  {
  }
  const EndpointReferenceType::AddressType& EndpointReferenceType::Address() const
  {
    return Address_;
  }
  EndpointReferenceType::EndpointReferenceType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void EndpointReferenceType::parse(const rapidxml::xml_node<>& node)
  {
    auto addressNode = node.first_node("Address", MDPWS::WS_NS_ADDRESSING.data());
    if (addressNode == nullptr)
    {
      throw ExpectedElement("Address", MDPWS::WS_NS_ADDRESSING.data());
    }
    Address_ = URIType({addressNode->value(), addressNode->value_size()});
  }

  RelatesToType::RelatesToType(const URIType& x)
    : URIType(x)
  {
  }
  RelatesToType::RelatesToType(const RelatesToType& x)
    : URIType(x)
  {
    RelationshipType_ = x.RelationshipType_;
  }

  const ReferenceParametersType::IdentifierOptional& ReferenceParametersType::Identifier() const
  {
    return Identifier_;
  }

  ReferenceParametersType::IdentifierOptional& ReferenceParametersType::Identifier()
  {
    return Identifier_;
  }

} // namespace WS::ADDRESSING
