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
    ActionType action;

    using AppSequenceType = WS::DISCOVERY::AppSequenceType;
    using AppSequenceOptional = std::optional<AppSequenceType>;
    AppSequenceOptional appSequence;

    using FaultToType = WS::ADDRESSING::EndpointReferenceType;
    using FaultToOptional = std::optional<FaultToType>;
    FaultToOptional faultTo;

    using FromType = WS::ADDRESSING::EndpointReferenceType;
    using FromOptional = std::optional<FromType>;
    FromOptional from;

    using MessageIDType = WS::ADDRESSING::URIType;
    using MessageIDOptional = std::optional<MessageIDType>;
    MessageIDOptional messageID;

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = std::optional<ReferenceParametersType>;
    ReferenceParametersOptional referenceParameters;

    using RelatesToType = WS::ADDRESSING::RelatesToType;
    using RelatesToOptional = std::optional<RelatesToType>;
    RelatesToOptional relatesTo;

    using ReplyToType = WS::ADDRESSING::EndpointReferenceType;
    using ReplyToOptional = std::optional<ReplyToType>;
    ReplyToOptional replyTo;

    using ToType = WS::ADDRESSING::URIType;
    using ToOptional = std::optional<ToType>;
    ToOptional to;

    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = std::optional<IdentifierType>;
    IdentifierOptional identifier;

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
    ByeOptional bye;

    using HelloType = WS::DISCOVERY::HelloType;
    using HelloOptional = std::optional<HelloType>;
    HelloOptional hello;

    using ProbeType = WS::DISCOVERY::ProbeType;
    using ProbeOptional = std::optional<ProbeType>;
    ProbeOptional probe;

    using ProbeMatchesType = WS::DISCOVERY::ProbeMatchesType;
    using ProbeMatchesOptional = std::optional<ProbeMatchesType>;
    ProbeMatchesOptional probeMatches;

    using ResolveType = WS::DISCOVERY::ResolveType;
    using ResolveOptional = std::optional<ResolveType>;
    ResolveOptional resolve;

    using ResolveMatchesType = WS::DISCOVERY::ResolveMatchesType;
    using ResolveMatchesOptional = std::optional<ResolveMatchesType>;
    ResolveMatchesOptional resolveMatches;

    using GetMetadataType = WS::MEX::GetMetadata;
    using GetMetadataOptional = std::optional<GetMetadataType>;
    GetMetadataOptional getMetadata;

    using MetadataType = WS::MEX::Metadata;
    using MetadataOptional = std::optional<MetadataType>;
    MetadataOptional metadata;

    using GetMdibType = BICEPS::MM::GetMdib;
    using GetMdibOptional = std::optional<GetMdibType>;
    GetMdibOptional getMdib;

    using GetMdibResponseType = BICEPS::MM::GetMdibResponse;
    using GetMdibResponseOptional = std::optional<GetMdibResponseType>;
    GetMdibResponseOptional getMdibResponse;

    using SubscribeType = WS::EVENTING::Subscribe;
    using SubscribeOptional = std::optional<SubscribeType>;
    SubscribeOptional subscribe;

    using SubscribeResponseType = WS::EVENTING::SubscribeResponse;
    using SubscribeResponseOptional = std::optional<SubscribeResponseType>;
    SubscribeResponseOptional subscribeResponse;

    using RenewType = WS::EVENTING::Renew;
    using RenewOptional = std::optional<RenewType>;
    RenewOptional renew;

    using RenewResponseType = WS::EVENTING::RenewResponse;
    using RenewResponseOptional = std::optional<RenewResponseType>;
    RenewResponseOptional renewResponse;

    using UnsubscribeType = WS::EVENTING::Unsubscribe;
    using UnsubscribeOptional = std::optional<UnsubscribeType>;
    UnsubscribeOptional unsubscribe;

    using SetValueType = BICEPS::MM::SetValue;
    using SetValueOptional = std::optional<SetValueType>;
    SetValueOptional setValue;

    using SetValueResponseType = BICEPS::MM::SetValueResponse;
    using SetValueResponseOptional = std::optional<SetValueResponseType>;
    SetValueResponseOptional setValueResponse;

    using EpisodicMetricReportType = BICEPS::MM::EpisodicMetricReport;
    using EpisodicMetricReportOptional = std::optional<EpisodicMetricReportType>;
    EpisodicMetricReportOptional episodicMetricReport;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct Envelope
  {
  public:
    using HeaderType = MESSAGEMODEL::Header;
    HeaderType header;

    using BodyType = MESSAGEMODEL::Body;
    BodyType body;

    explicit Envelope(const rapidxml::xml_node<>& node);
    Envelope() = default;

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

} // namespace MESSAGEMODEL
