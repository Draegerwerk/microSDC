#include "ws-discovery.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include <iterator>
#include <sstream>
#include <string>

namespace WS::DISCOVERY
{
  QName::QName(NameSpaceString ns, std::string name)
    : ns(ns)
    , name(std::move(name))
  {
  }

  QNameListType::QNameListType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void QNameListType::parse(const rapidxml::xml_node<>& node)
  {
    // TODO
  }

  AppSequenceType::AppSequenceType(const uint64_t& instance_id, const uint64_t& message_number)
    : instance_id(instance_id)
    , message_number(message_number)
  {
  }

  ByeType::ByeType(EndpointReferenceType epr)
    : endpoint_reference(std::move(epr))
  {
  }

  HelloType::HelloType(EndpointReferenceType epr, MetadataVersionType metadata_version)
    : endpoint_reference(std::move(epr))
    , metadata_version(metadata_version)
  {
  }

  ProbeType::ProbeType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void ProbeType::parse(const rapidxml::xml_node<>& node)
  {
    // TODO
  }


  ProbeMatchType::ProbeMatchType(EndpointReferenceType epr, MetadataVersionType metadata_version)
    : endpoint_reference(std::move(epr))
    , metadata_version(metadata_version)
  {
  }

  ProbeMatchesType::ProbeMatchesType(ProbeMatchSequence x)
    : probe_match(std::move(x))
  {
  }

  ResolveType::ResolveType(const rapidxml::xml_node<>& node)
    : endpoint_reference(WS::ADDRESSING::URIType(""))
  {
    this->parse(node);
  }

  void ResolveType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* epr_node = node.first_node("EndpointReference", MDPWS::WS_NS_ADDRESSING);
    if (epr_node == nullptr)
    {
      throw ExpectedElement("EndpointReference", MDPWS::WS_NS_ADDRESSING);
    }
    endpoint_reference = EndpointReferenceType(*epr_node);
  }

  ResolveMatchType::ResolveMatchType(EndpointReferenceType epr,
                                     MetadataVersionType metadata_version)
    : endpoint_reference(std::move(epr))
    , metadata_version(metadata_version)
  {
  }

  ResolveMatchesType::ResolveMatchesType(ResolveMatchSequence x)
    : resolve_match(std::move(x))
  {
  }

} // namespace WS::DISCOVERY
