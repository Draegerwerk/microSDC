#pragma once

#include "BICEPS_MessageModel.hpp"
#include "NamespaceMap.hpp"
#include "ws-MetadataExchange.hpp"
#include "ws-addressing.hpp"
#include "ws-discovery.hpp"
#include "ws-eventing.hpp"
#include <memory>
#include <optional>
#include <rapidxml.hpp>
#include <string>

namespace MESSAGEMODEL
{
  class Header
  {
  public:
    Header() = default;
    explicit Header(const rapidxml::xml_node<>& node);

    using ActionType = WS::ADDRESSING::URIType;
    const ActionType& Action() const;
    ActionType& Action();

    using AppSequenceType = WS::DISCOVERY::AppSequenceType;
    using AppSequenceOptional = std::optional<AppSequenceType>;
    const AppSequenceOptional& AppSequence() const;
    AppSequenceOptional& AppSequence();

    using FaultToType = WS::ADDRESSING::EndpointReferenceType;
    using FaultToOptional = std::optional<FaultToType>;
    const FaultToOptional& FaultTo() const;
    FaultToOptional& FaultTo();

    using FromType = WS::ADDRESSING::EndpointReferenceType;
    using FromOptional = std::optional<FromType>;
    const FromOptional& From() const;
    FromOptional& From();

    using MessageIDType = WS::ADDRESSING::URIType;
    using MessageIDOptional = std::optional<MessageIDType>;
    const MessageIDOptional& MessageID() const;
    MessageIDOptional& MessageID();

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = std::optional<ReferenceParametersType>;
    const ReferenceParametersOptional& ReferenceParameters() const;
    ReferenceParametersOptional& ReferenceParameters();

    using RelatesToType = WS::ADDRESSING::RelatesToType;
    using RelatesToOptional = std::optional<RelatesToType>;
    const RelatesToOptional& RelatesTo() const;
    RelatesToOptional& RelatesTo();

    using ReplyToType = WS::ADDRESSING::EndpointReferenceType;
    using ReplyToOptional = std::optional<ReplyToType>;
    const ReplyToOptional& ReplyTo() const;
    ReplyToOptional& ReplyTo();

    using ToType = WS::ADDRESSING::URIType;
    using ToOptional = std::optional<ToType>;
    const ToOptional& To() const;
    ToOptional& To();

    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = std::optional<IdentifierType>;
    const IdentifierOptional& Identifier() const;
    IdentifierOptional& Identifier();

  protected:
    ActionType Action_;
    AppSequenceOptional AppSequence_;
    FaultToOptional FaultTo_;
    FromOptional From_;
    MessageIDOptional MessageID_;
    ReferenceParametersOptional ReferenceParameters_;
    RelatesToOptional RelatesTo_;
    ReplyToOptional ReplyTo_;
    ToOptional To_;
    IdentifierOptional Identifier_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class Body
  {
  public:
    Body() = default;
    explicit Body(const rapidxml::xml_node<>& node);

    using ByeType = WS::DISCOVERY::ByeType;
    using ByeOptional = std::optional<ByeType>;
    const ByeOptional& Bye() const;
    ByeOptional& Bye();

    using HelloType = WS::DISCOVERY::HelloType;
    using HelloOptional = std::optional<HelloType>;
    const HelloOptional& Hello() const;
    HelloOptional& Hello();
    void Hello(const HelloType& x);

    using ProbeType = WS::DISCOVERY::ProbeType;
    using ProbeOptional = std::optional<ProbeType>;
    const ProbeOptional& Probe() const;
    ProbeOptional& Probe();

    using ProbeMatchesType = WS::DISCOVERY::ProbeMatchesType;
    using ProbeMatchesOptional = std::optional<ProbeMatchesType>;
    const ProbeMatchesOptional& ProbeMatches() const;
    ProbeMatchesOptional& ProbeMatches();
    void ProbeMatches(const ProbeMatchesType& x);

    using ResolveType = WS::DISCOVERY::ResolveType;
    using ResolveOptional = std::optional<ResolveType>;
    const ResolveOptional& Resolve() const;
    ResolveOptional& Resolve();

    using ResolveMatchesType = WS::DISCOVERY::ResolveMatchesType;
    using ResolveMatchesOptional = std::optional<ResolveMatchesType>;
    const ResolveMatchesOptional& ResolveMatches() const;
    ResolveMatchesOptional& ResolveMatches();

    // GetMetadata
    //
    using GetMetadataType = WS::MEX::GetMetadata;
    using GetMetadataOptional = std::optional<GetMetadataType>;
    const GetMetadataOptional& GetMetadata() const;
    GetMetadataOptional& GetMetadata();

    // Metadata
    //
    using MetadataType = WS::MEX::Metadata;
    using MetadataOptional = std::optional<MetadataType>;
    const MetadataOptional& Metadata() const;
    MetadataOptional& Metadata();

    // GetMdib
    //
    using GetMdibType = BICEPS::MM::GetMdib;
    using GetMdibOptional = std::optional<GetMdibType>;
    const GetMdibOptional& GetMdib() const;
    GetMdibOptional& GetMdib();

    // GetMdibResponse
    //
    using GetMdibResponseType = std::string;
    using GetMdibResponseOptional = std::optional<GetMdibResponseType>;
    const GetMdibResponseOptional& GetMdibResponse() const;
    GetMdibResponseOptional& GetMdibResponse();

  protected:
    ByeOptional Bye_;
    HelloOptional Hello_;
    ProbeOptional Probe_;
    ProbeMatchesOptional ProbeMatches_;
    ResolveOptional Resolve_;
    ResolveMatchesOptional ResolveMatches_;
    GetMetadataOptional GetMetadata_;
    MetadataOptional Metadata_;
    GetMdibOptional GetMdib_;
    GetMdibResponseOptional GetMdibResponse_;

    void parse(const rapidxml::xml_node<>& node);
  };

  class Envelope
  {
  public:
    // Header
    using HeaderType = MESSAGEMODEL::Header;
    const HeaderType& Header() const;
    HeaderType& Header();
    // Body
    using BodyType = MESSAGEMODEL::Body;
    const BodyType& Body() const;
    BodyType& Body();

    explicit Envelope(const rapidxml::xml_node<>& node);
    Envelope() = default;

  protected:
    HeaderType Header_;
    BodyType Body_;

    void parse(const rapidxml::xml_node<>& node);
  };

} // namespace MESSAGEMODEL
