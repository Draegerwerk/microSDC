#pragma once

#include "ws-addressing.hpp"
#include "ws-dpws.hpp"
#include <vector>

namespace WS::MEX
{
  class GetMetadata
  {
  public:
    // Dialect
    //
    using DialectType = WS::ADDRESSING::URIType;
    using DialectOptional = std::optional<DialectType>;

    // Identifier
    //
    using IdentifierType = WS::ADDRESSING::URIType;
    using IdentifierOptional = std::optional<IdentifierType>;

    GetMetadata(const rapidxml::xml_node<>& node);

  protected:
    DialectOptional Dialect_;
    IdentifierOptional Identifier_;

    void parse(const rapidxml::xml_node<>& node);
  };
  class MetadataReference
  {
  public:
  protected:
  };
  class MetadataSection
  {
  public:
    // MetadataReference
    //
    using MetadataReferenceType = WS::MEX::MetadataReference;
    using MetadataReferenceOptional = std::optional<MetadataReferenceType>;

    // Location
    //
    using LocationType = WS::ADDRESSING::URIType;
    using LocationOptional = std::optional<LocationType>;
    const LocationOptional& Location() const;
    LocationOptional& Location();

    // ThisModel
    //
    using ThisModelType = WS::DPWS::ThisModelType;
    using ThisModelOptional = std::optional<ThisModelType>;
    const ThisModelOptional& ThisModel() const;
    ThisModelOptional& ThisModel();

    // ThisDevice
    //
    using ThisDeviceType = WS::DPWS::ThisDeviceType;
    using ThisDeviceOptional = std::optional<ThisDeviceType>;
    const ThisDeviceOptional& ThisDevice() const;
    ThisDeviceOptional& ThisDevice();

    // Relationship
    //
    using RelationshipType = WS::DPWS::Relationship;
    using RelationshipOptional = std::optional<RelationshipType>;
    const RelationshipOptional& Relationship() const;
    RelationshipOptional& Relationship();

    // Dialect
    //
    using DialectType = WS::ADDRESSING::URIType;
    const DialectType& Dialect() const;
    DialectType& Dialect();

    // Identifier
    //
    using IdentifierType = WS::ADDRESSING::URIType;
    using IdentifierOptional = std::optional<IdentifierType>;

    MetadataSection(const DialectType& dialect);

  protected:
    MetadataReferenceOptional MetadataReference_;
    LocationOptional Location_;
    ThisModelOptional ThisModel_;
    ThisDeviceOptional ThisDevice_;
    RelationshipOptional Relationship_;
    DialectType Dialect_;
    IdentifierOptional Identifier_;
  };

  class Metadata
  {
  public:
    using MetadataSectionType = WS::MEX::MetadataSection;
    using MetadataSectionSequence = std::vector<MetadataSectionType>;
    const MetadataSectionSequence& MetadataSection() const;
    MetadataSectionSequence& MetadataSection();

  protected:
    MetadataSectionSequence MetadataSection_;
  };
} // namespace WS::MEX
