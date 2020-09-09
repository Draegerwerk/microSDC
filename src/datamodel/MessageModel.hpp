#pragma once

#include "BICEPS_MessageModel.hpp"
#include "ws-MetadataExchange.hpp"
#include "ws-addressing.hpp"
#include "ws-discovery.hpp"
#include "ws-eventing.hpp"
#include <memory>
#include <optional>
#include <string>

namespace MESSAGEMODEL
{
  struct Header
  {
  public:
    Header() = default;
    explicit Header(const rapidxml::xml_node<>& node);

    using ActionType = WS::ADDRESSING::URIType;
    ActionType Action;

    using AppSequenceType = WS::DISCOVERY::AppSequenceType;
    using AppSequenceOptional = std::optional<AppSequenceType>;
    AppSequenceOptional AppSequence;

    using FaultToType = WS::ADDRESSING::EndpointReferenceType;
    using FaultToOptional = std::optional<FaultToType>;
    FaultToOptional FaultTo;

    using FromType = WS::ADDRESSING::EndpointReferenceType;
    using FromOptional = std::optional<FromType>;
    FromOptional From;

    using MessageIDType = WS::ADDRESSING::URIType;
    using MessageIDOptional = std::optional<MessageIDType>;
    MessageIDOptional MessageID;

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = std::optional<ReferenceParametersType>;
    ReferenceParametersOptional ReferenceParameters;

    using RelatesToType = WS::ADDRESSING::RelatesToType;
    using RelatesToOptional = std::optional<RelatesToType>;
    RelatesToOptional RelatesTo;

    using ReplyToType = WS::ADDRESSING::EndpointReferenceType;
    using ReplyToOptional = std::optional<ReplyToType>;
    ReplyToOptional ReplyTo;

    using ToType = WS::ADDRESSING::URIType;
    using ToOptional = std::optional<ToType>;
    ToOptional To;

    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = std::optional<IdentifierType>;
    IdentifierOptional Identifier;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct Body
  {
  public:
    Body() = default;
    explicit Body(const rapidxml::xml_node<>& node);

    using ByeType = WS::DISCOVERY::ByeType;
    using ByeOptional = std::optional<ByeType>;
    ByeOptional Bye;

    using HelloType = WS::DISCOVERY::HelloType;
    using HelloOptional = std::optional<HelloType>;
    HelloOptional Hello;

    using ProbeType = WS::DISCOVERY::ProbeType;
    using ProbeOptional = std::optional<ProbeType>;
    ProbeOptional Probe;

    using ProbeMatchesType = WS::DISCOVERY::ProbeMatchesType;
    using ProbeMatchesOptional = std::optional<ProbeMatchesType>;
    ProbeMatchesOptional ProbeMatches;

    using ResolveType = WS::DISCOVERY::ResolveType;
    using ResolveOptional = std::optional<ResolveType>;
    ResolveOptional Resolve;

    using ResolveMatchesType = WS::DISCOVERY::ResolveMatchesType;
    using ResolveMatchesOptional = std::optional<ResolveMatchesType>;
    ResolveMatchesOptional ResolveMatches;

    using GetMetadataType = WS::MEX::GetMetadata;
    using GetMetadataOptional = std::optional<GetMetadataType>;
    GetMetadataOptional GetMetadata;

    using MetadataType = WS::MEX::Metadata;
    using MetadataOptional = std::optional<MetadataType>;
    MetadataOptional Metadata;

    using GetMdibType = BICEPS::MM::GetMdib;
    using GetMdibOptional = std::optional<GetMdibType>;
    GetMdibOptional GetMdib;

    using GetMdibResponseType = BICEPS::PM::Mdib;
    using GetMdibResponseOptional = std::optional<GetMdibResponseType>;
    GetMdibResponseOptional GetMdibResponse;

    using SubscribeType = WS::EVENTING::Subscribe;
    using SubscribeOptional = std::optional<SubscribeType>;
    SubscribeOptional Subscribe;

    using SubscribeResponseType = WS::EVENTING::SubscribeResponse;
    using SubscribeResponseOptional = std::optional<SubscribeResponseType>;
    SubscribeResponseOptional SubscribeResponse;

    using RenewType = WS::EVENTING::Renew;
    using RenewOptional = std::optional<RenewType>;
    RenewOptional Renew;

    using RenewResponseType = WS::EVENTING::RenewResponse;
    using RenewResponseOptional = std::optional<RenewResponseType>;
    RenewResponseOptional RenewResponse;

    using UnsubscribeType = WS::EVENTING::Unsubscribe;
    using UnsubscribeOptional = std::optional<UnsubscribeType>;
    UnsubscribeOptional Unsubscribe;

    using SetValueType = BICEPS::MM::SetValue;
    using SetValueOptional = std::optional<SetValueType>;
    SetValueOptional SetValue;

    using SetValueResponseType = BICEPS::MM::SetValueResponse;
    using SetValueResponseOptional = std::optional<SetValueResponseType>;
    SetValueResponseOptional SetValueResponse;

    using EpisodicMetricReportType = BICEPS::MM::EpisodicMetricReport;
    using EpisodicMetricReportOptional = std::optional<EpisodicMetricReportType>;
    EpisodicMetricReportOptional EpisodicMetricReport;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct Envelope
  {
  public:
    using HeaderType = MESSAGEMODEL::Header;
    HeaderType Header;

    using BodyType = MESSAGEMODEL::Body;
    BodyType Body;

    explicit Envelope(const rapidxml::xml_node<>& node);
    Envelope() = default;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

} // namespace MESSAGEMODEL
