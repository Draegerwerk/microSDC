#pragma once

#include <memory>
#include <optional>
#include <rapidxml.hpp>

namespace WS::EVENTING
{
  class Identifier : public std::string
  {
  public:
    using IsReferenceParameterType = bool;
    using IsReferenceParameterOptional = std::optional<IsReferenceParameterType>;

    Identifier(std::string identifier);

  protected:
    IsReferenceParameterOptional IsReferenceParameter_;
  };
} // namespace WS::EVENTING

namespace WS::ADDRESSING
{
  class URIType
  {
  public:
    explicit URIType(const rapidxml::xml_node<>& node);
    URIType(std::string);
    URIType() = default;

    const std::string& uri() const;

  protected:
    std::string uri_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class RelationshipTypeOpenEnum : public std::string
  {
  public:
  protected:
  };

  class RelatesToType : public WS::ADDRESSING::URIType
  {
  public:
    using RelationshipTypeType = ::WS::ADDRESSING::RelationshipTypeOpenEnum;
    using RelationshipTypeOptional = std::optional<RelationshipTypeType>;

    RelatesToType(const URIType& x);
    RelatesToType(const RelatesToType& x);

  protected:
    RelationshipTypeOptional RelationshipType_;
  };

  class ReferenceParametersType
  {
  public:
    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = std::optional<IdentifierType>;
    const IdentifierOptional& Identifier() const;
    IdentifierOptional& Identifier();

    ReferenceParametersType(const IdentifierType& identifier);

  protected:
    IdentifierOptional Identifier_;
  };

  class EndpointReferenceType
  {
  public:
    using AddressType = ::WS::ADDRESSING::URIType;
    const AddressType& Address() const;
    AddressType& Address();

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = std::optional<ReferenceParametersType>;
    const ReferenceParametersOptional& ReferenceParameters() const;
    ReferenceParametersOptional& ReferenceParameters();

    /* is this allowed?
    EndpointReferenceType(const EndpointReferenceType& epr);
    */

    EndpointReferenceType(const AddressType& address);
    EndpointReferenceType(const rapidxml::xml_node<>& node);
    EndpointReferenceType() = default;

  protected:
    AddressType Address_;
    ReferenceParametersOptional ReferenceParameters_;

    void parse(const rapidxml::xml_node<>& node);
  };
} // namespace WS::ADDRESSING
