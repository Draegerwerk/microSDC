#pragma once

#include "ws-addressing.hpp"
#include "ws-dpws.hpp"
#include <vector>

namespace WS::MEX
{
  struct GetMetadata
  {
  public:
    // Dialect
    //
    using DialectType = WS::ADDRESSING::URIType;
    using DialectOptional = std::optional<DialectType>;
    DialectOptional dialect;

    // Identifier
    //
    using IdentifierType = WS::ADDRESSING::URIType;
    using IdentifierOptional = std::optional<IdentifierType>;
    IdentifierOptional identifier;

    explicit GetMetadata(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct MetadataReference
  {
    // TODO
  };

  struct MetadataSection
  {
    // MetadataReference
    //
    using MetadataReferenceType = WS::MEX::MetadataReference;
    using MetadataReferenceOptional = std::optional<MetadataReferenceType>;
    MetadataReferenceOptional metadataReference;

    // Location
    //
    using LocationType = WS::ADDRESSING::URIType;
    using LocationOptional = std::optional<LocationType>;
    LocationOptional location;

    // ThisModel
    //
    using ThisModelType = WS::DPWS::ThisModelType;
    using ThisModelOptional = std::optional<ThisModelType>;
    ThisModelOptional thisModel;

    // ThisDevice
    //
    using ThisDeviceType = WS::DPWS::ThisDeviceType;
    using ThisDeviceOptional = std::optional<ThisDeviceType>;
    ThisDeviceOptional thisDevice;

    // Relationship
    //
    using RelationshipType = WS::DPWS::Relationship;
    using RelationshipOptional = std::optional<RelationshipType>;
    RelationshipOptional relationship;

    // Dialect
    //
    using DialectType = WS::ADDRESSING::URIType;
    DialectType dialect;

    // Identifier
    //
    using IdentifierType = WS::ADDRESSING::URIType;
    using IdentifierOptional = std::optional<IdentifierType>;
    IdentifierOptional identifier;

    explicit MetadataSection(DialectType dialect);
  };

  struct Metadata
  {
    using MetadataSectionType = WS::MEX::MetadataSection;
    using MetadataSectionSequence = std::vector<MetadataSectionType>;
    MetadataSectionSequence metadataSection;
  };
} // namespace WS::MEX
