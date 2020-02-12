#pragma once

#include "ws-addressing.hpp"
#include <memory>
#include <vector>

namespace WS::DISCOVERY
{
  class QName
  {
  public:
    using NameSpaceString = const char*;
    QName(NameSpaceString ns, const std::string& name);

    NameSpaceString ns() const;

    const std::string& name() const;
    std::string& name();

  protected:
    NameSpaceString ns_;
    std::string name_;
  };

  class QNameListType : public std::vector<QName>
  {
  public:
    QNameListType() = default;
    explicit QNameListType(const rapidxml::xml_node<>& node);

  protected:
    void parse(const rapidxml::xml_node<>& node);
  };

  class UriListType : public std::vector<WS::ADDRESSING::URIType>
  {
  public:
    UriListType() = default;
  };

  class ScopesType : public WS::DISCOVERY::UriListType
  {
  public:
    ScopesType() = default;
    ScopesType(const ScopesType& x);
    using MatchByType = WS::ADDRESSING::URIType;
    using MatchByOptional = std::optional<MatchByType>;
    const MatchByOptional& MatchBy() const;
    MatchByOptional& MatchBy();

  protected:
    MatchByOptional MatchBy_;
  };

  class AppSequenceType
  {
  public:
    AppSequenceType();
    AppSequenceType(const uint64_t& instanceId, const uint64_t& messageNumber);
    AppSequenceType(const AppSequenceType& x);
    using InstanceIdType = unsigned int;
    const InstanceIdType& InstanceId() const;
    InstanceIdType& InstanceId();

    using SequenceIdType = WS::ADDRESSING::URIType;
    using SequenceIdOptional = std::optional<SequenceIdType>;
    const SequenceIdOptional& SequenceId() const;
    SequenceIdOptional& SequenceId();

    using MessageNumberType = unsigned int;
    const MessageNumberType& MessageNumber() const;
    MessageNumberType& MessageNumber();

  protected:
    InstanceIdType InstanceId_;
    SequenceIdOptional SequenceId_;
    MessageNumberType MessageNumber_;
  };

  class ByeType
  {
  public:
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;

    using TypesType = WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;

    using MetadataVersionType = unsigned int;
    using MetadataVersionOptional = std::optional<MetadataVersionType>;

  protected:
    EndpointReferenceType EndpointReference_;
    TypesOptional Types_;
    ScopesOptional Scopes_;
    XAddrsOptional XAddrs_;
    MetadataVersionOptional MetadataVersion_;
  };

  class HelloType
  {
  public:
    // EndpointReference
    //
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    const EndpointReferenceType& EndpointReference() const;
    EndpointReferenceType& EndpointReference();

    // Types
    //
    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    const TypesOptional& Types() const;
    TypesOptional& Types();
    void Types(const TypesType& x);

    // Scopes
    //
    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    const ScopesOptional& Scopes() const;
    ScopesOptional& Scopes();
    void Scopes(const ScopesType& x);

    // XAddrs
    //
    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    const XAddrsOptional& XAddrs() const;
    XAddrsOptional& XAddrs();
    void XAddrs(const XAddrsType& x);

    // MetadataVersion
    //
    using MetadataVersionType = unsigned int;
    const MetadataVersionType& MetadataVersion() const;
    MetadataVersionType& MetadataVersion();

    HelloType(const EndpointReferenceType& epr, const MetadataVersionType& metadataVersion);
    HelloType(const HelloType& x);

  protected:
    EndpointReferenceType EndpointReference_;
    TypesOptional Types_;
    ScopesOptional Scopes_;
    XAddrsOptional XAddrs_;
    MetadataVersionType MetadataVersion_;
  };

  class ProbeType
  {
  public:
    ProbeType(const ProbeType& x);
    explicit ProbeType(const rapidxml::xml_node<>& node);

    // Types
    //
    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    const TypesOptional& Types() const;
    TypesOptional& Types();

    // Scopes
    //
    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    const ScopesOptional& Scopes() const;
    ScopesOptional& Scopes();

  protected:
    TypesOptional Types_;
    ScopesOptional Scopes_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class ProbeMatchType
  {
  public:
    // EndpointReference
    //
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    const EndpointReferenceType& EndpointReference() const;
    EndpointReferenceType& EndpointReference();

    // Types
    //
    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    const TypesOptional& Types() const;
    TypesOptional& Types();
    void Types(const TypesType& x);

    // Scopes
    //
    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    const ScopesOptional& Scopes() const;
    ScopesOptional& Scopes();
    void Scopes(const ScopesType& x);

    // XAddrs
    //
    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    const XAddrsOptional& XAddrs() const;
    XAddrsOptional& XAddrs();
    void XAddrs(const XAddrsType& x);

    // MetadataVersion
    //
    using MetadataVersionType = unsigned int;
    const MetadataVersionType& MetadataVersion() const;
    MetadataVersionType& MetadataVersion();

    ProbeMatchType(const ProbeMatchType& x);
    ProbeMatchType(const EndpointReferenceType& epr, const MetadataVersionType& metadataVersion);

  protected:
    EndpointReferenceType EndpointReference_;
    TypesOptional Types_;
    ScopesOptional Scopes_;
    XAddrsOptional XAddrs_;
    MetadataVersionType MetadataVersion_;
  };

  class ProbeMatchesType
  {
  public:
    // ProbeMatch
    //
    using ProbeMatchType = WS::DISCOVERY::ProbeMatchType;
    using ProbeMatchSequence = std::vector<ProbeMatchType>;

    const ProbeMatchSequence& ProbeMatch() const;
    ProbeMatchSequence& ProbeMatch();

    ProbeMatchesType(const ProbeMatchesType& x);
    ProbeMatchesType(const ProbeMatchSequence& x);
    ProbeMatchesType() = default;

  protected:
    ProbeMatchSequence ProbeMatch_;
  };

  class ResolveType
  {
  public:
    explicit ResolveType(const rapidxml::xml_node<>& node);

    // EndpointReference
    //
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    const EndpointReferenceType& EndpointReference() const;
    EndpointReferenceType& EndpointReference();

  protected:
    EndpointReferenceType EndpointReference_;

    void parse(const rapidxml::xml_node<>& node);
  };
  class ResolveMatchType
  {
  public:
    // EndpointReference
    //
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    const EndpointReferenceType& EndpointReference() const;
    EndpointReferenceType& EndpointReference();

    // Types
    //
    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    const TypesOptional& Types() const;
    TypesOptional& Types();
    void Types(const TypesType& x);

    // Scopes
    //
    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    const ScopesOptional& Scopes() const;
    ScopesOptional& Scopes();
    void Scopes(const ScopesType& x);

    // XAddrs
    //
    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    const XAddrsOptional& XAddrs() const;
    XAddrsOptional& XAddrs();
    void XAddrs(const XAddrsType& x);

    // MetadataVersion
    //
    using MetadataVersionType = unsigned int;
    const MetadataVersionType& MetadataVersion() const;
    MetadataVersionType& MetadataVersion();

    ResolveMatchType(const ResolveMatchType& x);
    ResolveMatchType(const EndpointReferenceType& epr, const MetadataVersionType& metadataVersion);

  protected:
    EndpointReferenceType EndpointReference_;
    TypesOptional Types_;
    ScopesOptional Scopes_;
    XAddrsOptional XAddrs_;
    MetadataVersionType MetadataVersion_;
  };
  class ResolveMatchesType
  {
  public:
    // ResolveMatch
    //
    using ResolveMatchType = WS::DISCOVERY::ResolveMatchType;
    using ResolveMatchSequence = std::vector<ResolveMatchType>;
    const ResolveMatchSequence& ResolveMatch() const;
    ResolveMatchSequence& ResolveMatch();

    ResolveMatchesType(const ResolveMatchesType& x);
    ResolveMatchesType(const ResolveMatchSequence& x);
    ResolveMatchesType() = default;

  protected:
    ResolveMatchSequence ResolveMatch_;
  };
} // namespace WS::DISCOVERY
