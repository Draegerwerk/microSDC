#pragma once

#include "ws-addressing.hpp"
#include <memory>
#include <vector>

namespace WS::DISCOVERY
{
  struct QName
  {
    using NameSpaceString = const char*;
    QName(NameSpaceString ns, std::string name);

    NameSpaceString ns;
    std::string name;
  };

  struct QNameListType : public std::vector<QName>
  {
  public:
    QNameListType() = default;
    explicit QNameListType(const rapidxml::xml_node<>& node);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  using UriListType = std::vector<WS::ADDRESSING::URIType>;

  struct ScopesType : public WS::DISCOVERY::UriListType
  {
  public:
    using MatchByType = WS::ADDRESSING::URIType;
    using MatchByOptional = std::optional<MatchByType>;
    MatchByOptional matchBy;
  };

  struct AppSequenceType
  {
  public:
    AppSequenceType(const uint64_t& instanceId, const uint64_t& messageNumber);
    using InstanceIdType = unsigned int;
    InstanceIdType instanceId;

    using SequenceIdType = WS::ADDRESSING::URIType;
    using SequenceIdOptional = std::optional<SequenceIdType>;
    SequenceIdOptional sequenceId;

    using MessageNumberType = unsigned int;
    MessageNumberType messageNumber;
  };

  struct ByeType
  {
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpointReference;

    using TypesType = WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    XAddrsOptional xAddrs;

    using MetadataVersionType = unsigned int;
    using MetadataVersionOptional = std::optional<MetadataVersionType>;
    MetadataVersionOptional metadataVersion;

    explicit ByeType(EndpointReferenceType epr);
  };

  struct HelloType
  {
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpointReference;

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    XAddrsOptional xAddrs;

    using MetadataVersionType = unsigned int;
    MetadataVersionType metadataVersion;

    HelloType(EndpointReferenceType epr, MetadataVersionType metadataVersion);
  };

  struct ProbeType
  {
    explicit ProbeType(const rapidxml::xml_node<>& node);

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    ScopesOptional scopes;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct ProbeMatchType
  {
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpointReference;

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    XAddrsOptional xAddrs;

    using MetadataVersionType = unsigned int;
    MetadataVersionType metadataVersion;

    ProbeMatchType(EndpointReferenceType epr, MetadataVersionType metadataVersion);
  };

  struct ProbeMatchesType
  {
    using ProbeMatchType = WS::DISCOVERY::ProbeMatchType;
    using ProbeMatchSequence = std::vector<ProbeMatchType>;
    ProbeMatchSequence probeMatch;

    explicit ProbeMatchesType(ProbeMatchSequence x);
  };

  struct ResolveType
  {
  public:
    explicit ResolveType(const rapidxml::xml_node<>& node);

    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpointReference;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct ResolveMatchType
  {
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpointReference;

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = std::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = std::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = std::optional<XAddrsType>;
    XAddrsOptional xAddrs;

    using MetadataVersionType = unsigned int;
    MetadataVersionType metadataVersion;

    ResolveMatchType(EndpointReferenceType epr, MetadataVersionType metadataVersion);
  };

  struct ResolveMatchesType
  {
  public:
    using ResolveMatchType = WS::DISCOVERY::ResolveMatchType;
    using ResolveMatchSequence = std::vector<ResolveMatchType>;
    ResolveMatchSequence resolveMatch;

    explicit ResolveMatchesType(ResolveMatchSequence x);
  };
} // namespace WS::DISCOVERY
