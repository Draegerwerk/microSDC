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

  AppSequenceType::AppSequenceType(const uint64_t& instanceId, const uint64_t& messageNumber)
    : InstanceId(instanceId)
    , MessageNumber(messageNumber)
  {
  }

  ByeType::ByeType(EndpointReferenceType epr)
    : EndpointReference(std::move(epr))
  {
  }

  HelloType::HelloType(EndpointReferenceType epr, MetadataVersionType metadataVersion)
    : EndpointReference(std::move(epr))
    , MetadataVersion(metadataVersion)
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


  ProbeMatchType::ProbeMatchType(EndpointReferenceType epr, MetadataVersionType metadataVersion)
    : EndpointReference(std::move(epr))
    , MetadataVersion(metadataVersion)
  {
  }

  ProbeMatchesType::ProbeMatchesType(ProbeMatchSequence x)
    : ProbeMatch(std::move(x))
  {
  }

  ResolveType::ResolveType(const rapidxml::xml_node<>& node)
    : EndpointReference(WS::ADDRESSING::URIType(""))
  {
    this->parse(node);
  }

  void ResolveType::parse(const rapidxml::xml_node<>& node)
  {
    const auto* eprNode = node.first_node("EndpointReference", MDPWS::WS_NS_ADDRESSING);
    if (eprNode == nullptr)
    {
      throw ExpectedElement("EndpointReference", MDPWS::WS_NS_ADDRESSING);
    }
    EndpointReference = EndpointReferenceType(*eprNode);
  }

  ResolveMatchType::ResolveMatchType(EndpointReferenceType epr, MetadataVersionType metadataVersion)
    : EndpointReference(std::move(epr))
    , MetadataVersion(metadataVersion)
  {
  }

  ResolveMatchesType::ResolveMatchesType(ResolveMatchSequence x)
    : ResolveMatch(std::move(x))
  {
  }

} // namespace WS::DISCOVERY
