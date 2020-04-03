#pragma once

#include "rapidxml.hpp"
#include <memory>
#include <optional>

namespace WS::EVENTING
{
  class Identifier : public std::string
  {
  public:
    using IsReferenceParameterType = bool;
    using IsReferenceParameterOptional = std::optional<IsReferenceParameterType>;

    explicit Identifier(const rapidxml::xml_node<>& node);
    explicit Identifier(const std::string& other);
    explicit Identifier(std::string&& other);
    using std::string::string;
    using std::string::operator=;

  private:
    IsReferenceParameterOptional IsReferenceParameter_;

    void parse(const rapidxml::xml_node<>& node);
  };
} // namespace WS::EVENTING

namespace WS::ADDRESSING
{
  class URIType : public std::string
  {
  public:
    explicit URIType(const rapidxml::xml_node<>& node);
    explicit URIType(const std::string& other);
    explicit URIType(std::string&& other);
    URIType() = default;
    using std::string::string;
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

    explicit RelatesToType(const URIType& x);

  private:
    RelationshipTypeOptional RelationshipType_;
  };

  class ReferenceParametersType
  {
  public:
    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = std::optional<IdentifierType>;
    const IdentifierOptional& Identifier() const;
    IdentifierOptional& Identifier();

    explicit ReferenceParametersType(const IdentifierType& identifier);

  private:
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

    explicit EndpointReferenceType(AddressType address);
    explicit EndpointReferenceType(const rapidxml::xml_node<>& node);

  private:
    AddressType Address_;
    ReferenceParametersOptional ReferenceParameters_;

    void parse(const rapidxml::xml_node<>& node);
  };
} // namespace WS::ADDRESSING
