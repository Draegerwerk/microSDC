#pragma once

#include "rapidxml.hpp"
#include <memory>
#include <boost/optional.hpp>

namespace WS::EVENTING
{
  struct Identifier : public std::string
  {
    using IsReferenceParameterType = bool;
    using IsReferenceParameterOptional = std::optional<IsReferenceParameterType>;
    IsReferenceParameterOptional is_reference_parameter;

    explicit Identifier(const rapidxml::xml_node<>& node);
    explicit Identifier(std::string x);
    using std::string::string;
    using std::string::operator=;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };
} // namespace WS::EVENTING

namespace WS::ADDRESSING
{
  struct URIType : public std::string
  {
    URIType() = default;
    explicit URIType(std::string other);
    explicit URIType(const rapidxml::xml_node<>& node);
    using std::string::string;
  };

  using RelationshipTypeOpenEnum = std::string;

  struct RelatesToType : public WS::ADDRESSING::URIType
  {
    using RelationshipTypeType = ::WS::ADDRESSING::RelationshipTypeOpenEnum;
    using RelationshipTypeOptional = std::optional<RelationshipTypeType>;
    RelationshipTypeOptional relationship_type;

    explicit RelatesToType(URIType x);
  };

  struct ReferenceParametersType
  {
    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = std::optional<IdentifierType>;
    IdentifierOptional identifier;

    explicit ReferenceParametersType(const IdentifierType& identifier);
  };

  struct EndpointReferenceType
  {
    using AddressType = ::WS::ADDRESSING::URIType;
    AddressType address;

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = std::optional<ReferenceParametersType>;
    ReferenceParametersOptional reference_parameters;

    explicit EndpointReferenceType(AddressType address);
    explicit EndpointReferenceType(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };
} // namespace WS::ADDRESSING
