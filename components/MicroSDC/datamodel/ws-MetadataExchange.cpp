#include "ws-MetadataExchange.hpp"
#include "MDPWSConstants.hpp"

#include <cstring>

namespace WS::MEX
{
  GetMetadata::GetMetadata(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void GetMetadata::parse(const rapidxml::xml_node<>& node)
  {

    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (strncmp(entry->name(), "Dialect", entry->name_size()) == 0 &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_METADATA_EXCHANGE.data(), entry->xmlns_size()) ==
              0)
      {
        Dialect_ =
            std::make_optional<DialectType>(std::string(entry->value(), entry->value_size()));
      }
      else if (strncmp(entry->name(), "Identifier", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_METADATA_EXCHANGE.data(),
                       entry->xmlns_size()) == 0)
      {
        Identifier_ =
            std::make_optional<IdentifierType>(std::string(entry->value(), entry->value_size()));
      }
    }
  }

  const MetadataSection::LocationOptional& MetadataSection::Location() const
  {
    return Location_;
  }
  MetadataSection::LocationOptional& MetadataSection::Location()
  {
    return Location_;
  }

  const MetadataSection::ThisModelOptional& MetadataSection::ThisModel() const
  {
    return ThisModel_;
  }
  MetadataSection::ThisModelOptional& MetadataSection::ThisModel()
  {
    return ThisModel_;
  }

  const MetadataSection::ThisDeviceOptional& MetadataSection::ThisDevice() const
  {
    return ThisDevice_;
  }
  MetadataSection::ThisDeviceOptional& MetadataSection::ThisDevice()
  {
    return ThisDevice_;
  }

  const MetadataSection::RelationshipOptional& MetadataSection::Relationship() const
  {
    return Relationship_;
  }
  MetadataSection::RelationshipOptional& MetadataSection::Relationship()
  {
    return Relationship_;
  }

  const MetadataSection::DialectType& MetadataSection::Dialect() const
  {
    return Dialect_;
  }
  MetadataSection::DialectType& MetadataSection::Dialect()
  {
    return Dialect_;
  }

  MetadataSection::MetadataSection(const DialectType& dialect)
    : Dialect_(dialect)
  {
  }

  const Metadata::MetadataSectionSequence& Metadata::MetadataSection() const
  {
    return MetadataSection_;
  }
  Metadata::MetadataSectionSequence& Metadata::MetadataSection()
  {
    return MetadataSection_;
  }
} // namespace WS::MEX
