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
    using DialectOptional = boost::optional<DialectType>;
    DialectOptional dialect;

    // Identifier
    //
    using IdentifierType = WS::ADDRESSING::URIType;
    using IdentifierOptional = boost::optional<IdentifierType>;
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
    using MetadataReferenceOptional = boost::optional<MetadataReferenceType>;
    MetadataReferenceOptional metadata_reference;

    // Location
    //
    using LocationType = WS::ADDRESSING::URIType;
    using LocationOptional = boost::optional<LocationType>;
    LocationOptional location;

    // ThisModel
    //
    using ThisModelType = WS::DPWS::ThisModelType;
    using ThisModelOptional = boost::optional<ThisModelType>;
    ThisModelOptional this_model;

    // ThisDevice
    //
    using ThisDeviceType = WS::DPWS::ThisDeviceType;
    using ThisDeviceOptional = boost::optional<ThisDeviceType>;
    ThisDeviceOptional this_device;

    // Relationship
    //
    using RelationshipType = WS::DPWS::Relationship;
    using RelationshipOptional = boost::optional<RelationshipType>;
    RelationshipOptional relationship;

    // Dialect
    //
    using DialectType = WS::ADDRESSING::URIType;
    DialectType dialect;

    // Identifier
    //
    using IdentifierType = WS::ADDRESSING::URIType;
    using IdentifierOptional = boost::optional<IdentifierType>;
    IdentifierOptional identifier;

    explicit MetadataSection(DialectType dialect);
  };

  struct Metadata
  {
    using MetadataSectionType = WS::MEX::MetadataSection;
    using MetadataSectionSequence = std::vector<MetadataSectionType>;
    MetadataSectionSequence metadata_section;
  };
} // namespace WS::MEX
