#include "ws-addressing.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"

namespace WS::EVENTING
{
  // Identifier
  //
  Identifier::Identifier(std::string identifier)
    : std::string(identifier)
  {
  }
} // namespace WS::EVENTING

namespace WS::ADDRESSING
{
  // URIType
  //
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

  // EndpointReferenceType
  //
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
  const EndpointReferenceType::ReferenceParametersOptional&
  EndpointReferenceType::ReferenceParameters() const
  {
    return ReferenceParameters_;
  }
  EndpointReferenceType::ReferenceParametersOptional& EndpointReferenceType::ReferenceParameters()
  {
    return ReferenceParameters_;
  }
  void EndpointReferenceType::parse(const rapidxml::xml_node<>& node)
  {
    auto addressNode = node.first_node("Address", MDPWS::WS_NS_ADDRESSING);
    if (addressNode == nullptr)
    {
      throw ExpectedElement("Address", MDPWS::WS_NS_ADDRESSING);
    }
    Address_ = URIType({addressNode->value(), addressNode->value_size()});
    auto referenceParameters =
        addressNode->next_sibling("ReferenceParameters", MDPWS::WS_NS_ADDRESSING);
    if (referenceParameters != nullptr &&
        referenceParameters->first_node("Identifier", MDPWS::WS_NS_EVENTING) != nullptr)
    {
      ReferenceParameters_ = std::make_optional<ReferenceParametersType>(
          std::string(referenceParameters->first_node()->value(),
                      referenceParameters->first_node()->value_size()));
    }
  }

  // RelatesToType
  //
  RelatesToType::RelatesToType(const URIType& x)
    : URIType(x)
  {
  }
  RelatesToType::RelatesToType(const RelatesToType& x)
    : URIType(x)
  {
    RelationshipType_ = x.RelationshipType_;
  }

  // ReferenceParametersType
  //
  ReferenceParametersType::ReferenceParametersType(const IdentifierType& identifier)
    : Identifier_(identifier)
  {
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
