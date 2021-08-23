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
    using MatchByOptional = boost::optional<MatchByType>;
    MatchByOptional match_by;
  };

  struct AppSequenceType
  {
  public:
    AppSequenceType(const uint64_t& instance_id, const uint64_t& message_number);
    using InstanceIdType = unsigned int;
    InstanceIdType instance_id;

    using SequenceIdType = WS::ADDRESSING::URIType;
    using SequenceIdOptional = boost::optional<SequenceIdType>;
    SequenceIdOptional sequence_id;

    using MessageNumberType = unsigned int;
    MessageNumberType message_number;
  };

  struct ByeType
  {
    using EndpointReferenceType = WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpoint_reference;

    using TypesType = WS::DISCOVERY::QNameListType;
    using TypesOptional = boost::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = boost::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = boost::optional<XAddrsType>;
    XAddrsOptional x_addrs;

    using MetadataVersionType = unsigned int;
    using MetadataVersionOptional = boost::optional<MetadataVersionType>;
    MetadataVersionOptional metadata_version;

    explicit ByeType(EndpointReferenceType epr);
  };

  struct HelloType
  {
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpoint_reference;

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = boost::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = boost::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = boost::optional<XAddrsType>;
    XAddrsOptional x_addrs;

    using MetadataVersionType = unsigned int;
    MetadataVersionType metadata_version{0};

    HelloType(EndpointReferenceType epr, MetadataVersionType metadata_version);
  };

  struct ProbeType
  {
    explicit ProbeType(const rapidxml::xml_node<>& node);

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = boost::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = boost::optional<ScopesType>;
    ScopesOptional scopes;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct ProbeMatchType
  {
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpoint_reference;

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = boost::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = boost::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = boost::optional<XAddrsType>;
    XAddrsOptional x_addrs;

    using MetadataVersionType = unsigned int;
    MetadataVersionType metadata_version{0};

    ProbeMatchType(EndpointReferenceType epr, MetadataVersionType metadata_version);
  };

  struct ProbeMatchesType
  {
    using ProbeMatchType = WS::DISCOVERY::ProbeMatchType;
    using ProbeMatchSequence = std::vector<ProbeMatchType>;
    ProbeMatchSequence probe_match;

    explicit ProbeMatchesType(ProbeMatchSequence x);
  };

  struct ResolveType
  {
  public:
    explicit ResolveType(const rapidxml::xml_node<>& node);

    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpoint_reference;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct ResolveMatchType
  {
    using EndpointReferenceType = ::WS::ADDRESSING::EndpointReferenceType;
    EndpointReferenceType endpoint_reference;

    using TypesType = ::WS::DISCOVERY::QNameListType;
    using TypesOptional = boost::optional<TypesType>;
    TypesOptional types;

    using ScopesType = WS::DISCOVERY::ScopesType;
    using ScopesOptional = boost::optional<ScopesType>;
    ScopesOptional scopes;

    using XAddrsType = WS::DISCOVERY::UriListType;
    using XAddrsOptional = boost::optional<XAddrsType>;
    XAddrsOptional x_addrs;

    using MetadataVersionType = unsigned int;
    MetadataVersionType metadata_version{0};

    ResolveMatchType(EndpointReferenceType epr, MetadataVersionType metadata_version);
  };

  struct ResolveMatchesType
  {
  public:
    using ResolveMatchType = WS::DISCOVERY::ResolveMatchType;
    using ResolveMatchSequence = std::vector<ResolveMatchType>;
    ResolveMatchSequence resolve_match;

    explicit ResolveMatchesType(ResolveMatchSequence x);
  };
} // namespace WS::DISCOVERY
