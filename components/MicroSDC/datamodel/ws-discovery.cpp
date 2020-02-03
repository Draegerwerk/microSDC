#include "ws-discovery.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include <iterator>
#include <sstream>
#include <string>

namespace WS::DISCOVERY
{
  QName::QName(const frozen::string& ns, const std::string& name)
    : ns_(ns)
    , name_(name)
  {
  }

  const frozen::string& QName::ns() const
  {
    return ns_;
  }
  frozen::string& QName::ns()
  {
    return ns_;
  }

  const std::string& QName::name() const
  {
    return name_;
  }
  std::string& QName::name()
  {
    return name_;
  }

  QNameListType::QNameListType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void QNameListType::parse(const rapidxml::xml_node<>& node) {}

  ScopesType::ScopesType(const ScopesType& x)
  {
    MatchBy_ = x.MatchBy();
  }

  const ScopesType::MatchByOptional& ScopesType::MatchBy() const
  {
    return MatchBy_;
  }
  ScopesType::MatchByOptional& ScopesType::MatchBy()
  {
    return MatchBy_;
  }

  AppSequenceType::AppSequenceType(const uint64_t& instanceId, const uint64_t& messageNumber)
    : InstanceId_(instanceId)
    , MessageNumber_(messageNumber)
  {
  }
  AppSequenceType::AppSequenceType(const AppSequenceType& x)
  {
    InstanceId_ = x.InstanceId();
    SequenceId_ = x.SequenceId();
    MessageNumber_ = x.MessageNumber();
  }

  const AppSequenceType::InstanceIdType& AppSequenceType::InstanceId() const
  {
    return InstanceId_;
  }
  AppSequenceType::InstanceIdType& AppSequenceType::InstanceId()
  {
    return InstanceId_;
  }

  const AppSequenceType::SequenceIdOptional& AppSequenceType::SequenceId() const
  {
    return SequenceId_;
  }
  AppSequenceType::SequenceIdOptional& AppSequenceType::SequenceId()
  {
    return SequenceId_;
  }

  const AppSequenceType::MessageNumberType& AppSequenceType::MessageNumber() const
  {
    return MessageNumber_;
  }
  AppSequenceType::MessageNumberType& AppSequenceType::MessageNumber()
  {
    return MessageNumber_;
  }

  HelloType::HelloType(const EndpointReferenceType& epr, const MetadataVersionType& metadataVersion)
    : EndpointReference_(epr)
    , MetadataVersion_(metadataVersion)
  {
  }

  HelloType::HelloType(const HelloType& x)
    : EndpointReference_(x.EndpointReference_)
    , Types_(x.Types_)
    , Scopes_(x.Scopes_)
    , XAddrs_(x.XAddrs_)
    , MetadataVersion_(x.MetadataVersion_)
  {
  }

  // EndpointReference
  //
  const HelloType::EndpointReferenceType& HelloType::EndpointReference() const
  {
    return EndpointReference_;
  }
  HelloType::EndpointReferenceType& HelloType::EndpointReference()
  {
    return EndpointReference_;
  }

  // Types
  //
  const HelloType::TypesOptional& HelloType::Types() const
  {
    return Types_;
  }
  HelloType::TypesOptional& HelloType::Types()
  {
    return Types_;
  }
  void HelloType::Types(const TypesType& x)
  {
    Types_ = x;
  }

  // Scopes
  //
  const HelloType::ScopesOptional& HelloType::Scopes() const
  {
    return Scopes_;
  }
  HelloType::ScopesOptional& HelloType::Scopes()
  {
    return Scopes_;
  }
  void HelloType::Scopes(const ScopesType& x)
  {
    Scopes_ = x;
  }

  // XAddrs
  //
  const HelloType::XAddrsOptional& HelloType::XAddrs() const
  {
    return XAddrs_;
  }
  HelloType::XAddrsOptional& HelloType::XAddrs()
  {
    return XAddrs_;
  }
  void HelloType::XAddrs(const XAddrsType& x)
  {
    XAddrs_ = x;
  }

  // MetadataVersion
  //
  const HelloType::MetadataVersionType& HelloType::MetadataVersion() const
  {
    return MetadataVersion_;
  }
  HelloType::MetadataVersionType& HelloType::MetadataVersion()
  {
    return MetadataVersion_;
  }


  ProbeType::ProbeType(const ProbeType& x)
    : Types_(x.Types_)
    , Scopes_(x.Scopes_)
  {
  }

  ProbeType::ProbeType(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  // Types
  //
  const ProbeType::TypesOptional& ProbeType::Types() const
  {
    return Types_;
  }
  ProbeType::TypesOptional& ProbeType::Types()
  {
    return Types_;
  }

  // Scopes
  //
  const ProbeType::ScopesOptional& ProbeType::Scopes() const
  {
    return Scopes_;
  }
  ProbeType::ScopesOptional& ProbeType::Scopes()
  {
    return Scopes_;
  }

  void ProbeType::parse(const rapidxml::xml_node<>& node) {}


  void ProbeMatchType::Types(const TypesType& x)
  {
    Types_ = x;
  }

  void ProbeMatchType::Scopes(const ScopesType& x)
  {
    Scopes_ = x;
  }

  void ProbeMatchType::XAddrs(const XAddrsType& x)
  {
    XAddrs_ = x;
  }

  const ProbeMatchType::EndpointReferenceType& ProbeMatchType::EndpointReference() const
  {
    return EndpointReference_;
  }
  ProbeMatchType::EndpointReferenceType& ProbeMatchType::EndpointReference()
  {
    return EndpointReference_;
  }
  const ProbeMatchType::TypesOptional& ProbeMatchType::Types() const
  {
    return Types_;
  }
  ProbeMatchType::TypesOptional& ProbeMatchType::Types()
  {
    return Types_;
  }
  const ProbeMatchType::ScopesOptional& ProbeMatchType::Scopes() const
  {
    return Scopes_;
  }
  ProbeMatchType::ScopesOptional& ProbeMatchType::Scopes()
  {
    return Scopes_;
  }
  const ProbeMatchType::XAddrsOptional& ProbeMatchType::XAddrs() const
  {
    return XAddrs_;
  }
  ProbeMatchType::XAddrsOptional& ProbeMatchType::XAddrs()
  {
    return XAddrs_;
  }
  const ProbeMatchType::MetadataVersionType& ProbeMatchType::MetadataVersion() const
  {
    return MetadataVersion_;
  }
  ProbeMatchType::MetadataVersionType& ProbeMatchType::MetadataVersion()
  {
    return MetadataVersion_;
  }

  ProbeMatchType::ProbeMatchType(const EndpointReferenceType& epr,
                                 const MetadataVersionType& metadataVersion)
    : EndpointReference_(epr)
    , MetadataVersion_(metadataVersion)
  {
  }

  ProbeMatchType::ProbeMatchType(const ProbeMatchType& x)
    : EndpointReference_(x.EndpointReference_)
    , Types_(x.Types_)
    , Scopes_(x.Scopes_)
    , XAddrs_(x.XAddrs_)
    , MetadataVersion_(x.MetadataVersion_)
  {
  }

  ProbeMatchesType::ProbeMatchesType(const ProbeMatchSequence& x)
  {
    ProbeMatch_ = x;
  }
  ProbeMatchesType::ProbeMatchesType(const ProbeMatchesType& x)
  {
    ProbeMatch_ = x.ProbeMatch_;
  }

  const ProbeMatchesType::ProbeMatchSequence& ProbeMatchesType::ProbeMatch() const
  {
    return ProbeMatch_;
  }
  ProbeMatchesType::ProbeMatchSequence& ProbeMatchesType::ProbeMatch()
  {
    return ProbeMatch_;
  }

  ResolveType::ResolveType(const rapidxml::xml_node<>& node)
    : EndpointReference_(WS::ADDRESSING::URIType(""))
  {
    this->parse(node);
  }

  const ResolveType::EndpointReferenceType& ResolveType::EndpointReference() const
  {
    return EndpointReference_;
  }

  ResolveType::EndpointReferenceType& ResolveType::EndpointReference()
  {
    return EndpointReference_;
  }

  void ResolveType::parse(const rapidxml::xml_node<>& node)
  {
    auto eprNode = node.first_node("EndpointReference", MDPWS::WS_NS_ADDRESSING.data());
    if (eprNode == nullptr)
    {
      throw ExpectedElement("EndpointReference", MDPWS::WS_NS_ADDRESSING.data());
    }
    EndpointReference_ = EndpointReferenceType(*eprNode);
  }

  void ResolveMatchType::Types(const TypesType& x)
  {
    Types_ = x;
  }

  void ResolveMatchType::Scopes(const ScopesType& x)
  {
    Scopes_ = x;
  }

  void ResolveMatchType::XAddrs(const XAddrsType& x)
  {
    XAddrs_ = x;
  }

  const ResolveMatchType::EndpointReferenceType& ResolveMatchType::EndpointReference() const
  {
    return EndpointReference_;
  }
  ResolveMatchType::EndpointReferenceType& ResolveMatchType::EndpointReference()
  {
    return EndpointReference_;
  }
  const ResolveMatchType::TypesOptional& ResolveMatchType::Types() const
  {
    return Types_;
  }
  ResolveMatchType::TypesOptional& ResolveMatchType::Types()
  {
    return Types_;
  }
  const ResolveMatchType::ScopesOptional& ResolveMatchType::Scopes() const
  {
    return Scopes_;
  }
  ResolveMatchType::ScopesOptional& ResolveMatchType::Scopes()
  {
    return Scopes_;
  }
  const ResolveMatchType::XAddrsOptional& ResolveMatchType::XAddrs() const
  {
    return XAddrs_;
  }
  ResolveMatchType::XAddrsOptional& ResolveMatchType::XAddrs()
  {
    return XAddrs_;
  }
  const ResolveMatchType::MetadataVersionType& ResolveMatchType::MetadataVersion() const
  {
    return MetadataVersion_;
  }
  ResolveMatchType::MetadataVersionType& ResolveMatchType::MetadataVersion()
  {
    return MetadataVersion_;
  }

  ResolveMatchType::ResolveMatchType(const EndpointReferenceType& epr,
                                     const MetadataVersionType& metadataVersion)
    : EndpointReference_(epr)
    , MetadataVersion_(metadataVersion)
  {
  }

  ResolveMatchType::ResolveMatchType(const ResolveMatchType& x)
    : EndpointReference_(x.EndpointReference_)
    , Types_(x.Types_)
    , Scopes_(x.Scopes_)
    , XAddrs_(x.XAddrs_)
    , MetadataVersion_(x.MetadataVersion_)
  {
  }

  ResolveMatchesType::ResolveMatchesType(const ResolveMatchSequence& x)
  {
    ResolveMatch_ = x;
  }
  ResolveMatchesType::ResolveMatchesType(const ResolveMatchesType& x)
  {
    ResolveMatch_ = x.ResolveMatch_;
  }

  const ResolveMatchesType::ResolveMatchSequence& ResolveMatchesType::ResolveMatch() const
  {
    return ResolveMatch_;
  }
  ResolveMatchesType::ResolveMatchSequence& ResolveMatchesType::ResolveMatch()
  {
    return ResolveMatch_;
  }

} // namespace WS::DISCOVERY
