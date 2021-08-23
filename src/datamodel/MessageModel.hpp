#pragma once

#include "BICEPS_MessageModel.hpp"
#include "ws-MetadataExchange.hpp"
#include "ws-addressing.hpp"
#include "ws-discovery.hpp"
#include "ws-eventing.hpp"
#include <memory>
#include <boost/optional.hpp>
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
    AppSequenceOptional app_sequence;

    using FaultToType = WS::ADDRESSING::EndpointReferenceType;
    using FaultToOptional = std::optional<FaultToType>;
    FaultToOptional fault_to;

    using FromType = WS::ADDRESSING::EndpointReferenceType;
    using FromOptional = std::optional<FromType>;
    FromOptional from;

    using MessageIDType = WS::ADDRESSING::URIType;
    using MessageIDOptional = std::optional<MessageIDType>;
    MessageIDOptional message_id;

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = std::optional<ReferenceParametersType>;
    ReferenceParametersOptional reference_parameters;

    using RelatesToType = WS::ADDRESSING::RelatesToType;
    using RelatesToOptional = std::optional<RelatesToType>;
    RelatesToOptional relates_to;

    using ReplyToType = WS::ADDRESSING::EndpointReferenceType;
    using ReplyToOptional = std::optional<ReplyToType>;
    ReplyToOptional reply_to;

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
    ProbeMatchesOptional probe_matches;

    using ResolveType = WS::DISCOVERY::ResolveType;
    using ResolveOptional = std::optional<ResolveType>;
    ResolveOptional resolve;

    using ResolveMatchesType = WS::DISCOVERY::ResolveMatchesType;
    using ResolveMatchesOptional = std::optional<ResolveMatchesType>;
    ResolveMatchesOptional resolve_matches;

    using GetMetadataType = WS::MEX::GetMetadata;
    using GetMetadataOptional = std::optional<GetMetadataType>;
    GetMetadataOptional get_metadata;

    using MetadataType = WS::MEX::Metadata;
    using MetadataOptional = std::optional<MetadataType>;
    MetadataOptional metadata;

    using GetMdibType = BICEPS::MM::GetMdib;
    using GetMdibOptional = std::optional<GetMdibType>;
    GetMdibOptional get_mdib;

    using GetMdibResponseType = BICEPS::MM::GetMdibResponse;
    using GetMdibResponseOptional = std::optional<GetMdibResponseType>;
    GetMdibResponseOptional get_mdib_response;

    using SubscribeType = WS::EVENTING::Subscribe;
    using SubscribeOptional = std::optional<SubscribeType>;
    SubscribeOptional subscribe;

    using SubscribeResponseType = WS::EVENTING::SubscribeResponse;
    using SubscribeResponseOptional = std::optional<SubscribeResponseType>;
    SubscribeResponseOptional subscribe_response;

    using RenewType = WS::EVENTING::Renew;
    using RenewOptional = std::optional<RenewType>;
    RenewOptional renew;

    using RenewResponseType = WS::EVENTING::RenewResponse;
    using RenewResponseOptional = std::optional<RenewResponseType>;
    RenewResponseOptional renew_response;

    using UnsubscribeType = WS::EVENTING::Unsubscribe;
    using UnsubscribeOptional = std::optional<UnsubscribeType>;
    UnsubscribeOptional unsubscribe;

    using SetStringType = BICEPS::MM::SetString;
    using SetStringOptional = std::optional<SetStringType>;
    SetStringOptional set_string;

    using SetStringResponseType = BICEPS::MM::SetStringResponse;
    using SetStringResponseOptional = std::optional<SetStringResponseType>;
    SetStringResponseOptional set_string_response;

    using SetValueType = BICEPS::MM::SetValue;
    using SetValueOptional = std::optional<SetValueType>;
    SetValueOptional set_value;

    using SetValueResponseType = BICEPS::MM::SetValueResponse;
    using SetValueResponseOptional = std::optional<SetValueResponseType>;
    SetValueResponseOptional set_value_response;

    using EpisodicMetricReportType = BICEPS::MM::EpisodicMetricReport;
    using EpisodicMetricReportOptional = std::optional<EpisodicMetricReportType>;
    EpisodicMetricReportOptional episodic_metric_report;

    using EpisodicComponentReportType = BICEPS::MM::EpisodicComponentReport;
    using EpisodicComponentReportOptional = std::optional<EpisodicComponentReportType>;
    EpisodicComponentReportOptional episodic_component_report;

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
