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
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_METADATA_EXCHANGE, entry->xmlns_size()) == 0)
      {
        dialect = std::make_optional<DialectType>(entry->value(), entry->value_size());
      }
      else if (strncmp(entry->name(), "Identifier", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_METADATA_EXCHANGE, entry->xmlns_size()) == 0)
      {
        identifier = std::make_optional<IdentifierType>(entry->value(), entry->value_size());
      }
    }
  }

  MetadataSection::MetadataSection(DialectType dialect)
    : dialect(std::move(dialect))
  {
  }
} // namespace WS::MEX
