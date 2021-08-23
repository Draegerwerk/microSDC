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
    using AppSequenceOptional = boost::optional<AppSequenceType>;
    AppSequenceOptional app_sequence;

    using FaultToType = WS::ADDRESSING::EndpointReferenceType;
    using FaultToOptional = boost::optional<FaultToType>;
    FaultToOptional fault_to;

    using FromType = WS::ADDRESSING::EndpointReferenceType;
    using FromOptional = boost::optional<FromType>;
    FromOptional from;

    using MessageIDType = WS::ADDRESSING::URIType;
    using MessageIDOptional = boost::optional<MessageIDType>;
    MessageIDOptional message_id;

    using ReferenceParametersType = WS::ADDRESSING::ReferenceParametersType;
    using ReferenceParametersOptional = boost::optional<ReferenceParametersType>;
    ReferenceParametersOptional reference_parameters;

    using RelatesToType = WS::ADDRESSING::RelatesToType;
    using RelatesToOptional = boost::optional<RelatesToType>;
    RelatesToOptional relates_to;

    using ReplyToType = WS::ADDRESSING::EndpointReferenceType;
    using ReplyToOptional = boost::optional<ReplyToType>;
    ReplyToOptional reply_to;

    using ToType = WS::ADDRESSING::URIType;
    using ToOptional = boost::optional<ToType>;
    ToOptional to;

    using IdentifierType = WS::EVENTING::Identifier;
    using IdentifierOptional = boost::optional<IdentifierType>;
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
    using ByeOptional = boost::optional<ByeType>;
    ByeOptional bye;

    using HelloType = WS::DISCOVERY::HelloType;
    using HelloOptional = boost::optional<HelloType>;
    HelloOptional hello;

    using ProbeType = WS::DISCOVERY::ProbeType;
    using ProbeOptional = boost::optional<ProbeType>;
    ProbeOptional probe;

    using ProbeMatchesType = WS::DISCOVERY::ProbeMatchesType;
    using ProbeMatchesOptional = boost::optional<ProbeMatchesType>;
    ProbeMatchesOptional probe_matches;

    using ResolveType = WS::DISCOVERY::ResolveType;
    using ResolveOptional = boost::optional<ResolveType>;
    ResolveOptional resolve;

    using ResolveMatchesType = WS::DISCOVERY::ResolveMatchesType;
    using ResolveMatchesOptional = boost::optional<ResolveMatchesType>;
    ResolveMatchesOptional resolve_matches;

    using GetMetadataType = WS::MEX::GetMetadata;
    using GetMetadataOptional = boost::optional<GetMetadataType>;
    GetMetadataOptional get_metadata;

    using MetadataType = WS::MEX::Metadata;
    using MetadataOptional = boost::optional<MetadataType>;
    MetadataOptional metadata;

    using GetMdibType = BICEPS::MM::GetMdib;
    using GetMdibOptional = boost::optional<GetMdibType>;
    GetMdibOptional get_mdib;

    using GetMdibResponseType = BICEPS::MM::GetMdibResponse;
    using GetMdibResponseOptional = boost::optional<GetMdibResponseType>;
    GetMdibResponseOptional get_mdib_response;

    using SubscribeType = WS::EVENTING::Subscribe;
    using SubscribeOptional = boost::optional<SubscribeType>;
    SubscribeOptional subscribe;

    using SubscribeResponseType = WS::EVENTING::SubscribeResponse;
    using SubscribeResponseOptional = boost::optional<SubscribeResponseType>;
    SubscribeResponseOptional subscribe_response;

    using RenewType = WS::EVENTING::Renew;
    using RenewOptional = boost::optional<RenewType>;
    RenewOptional renew;

    using RenewResponseType = WS::EVENTING::RenewResponse;
    using RenewResponseOptional = boost::optional<RenewResponseType>;
    RenewResponseOptional renew_response;

    using UnsubscribeType = WS::EVENTING::Unsubscribe;
    using UnsubscribeOptional = boost::optional<UnsubscribeType>;
    UnsubscribeOptional unsubscribe;

    using SetStringType = BICEPS::MM::SetString;
    using SetStringOptional = boost::optional<SetStringType>;
    SetStringOptional set_string;

    using SetStringResponseType = BICEPS::MM::SetStringResponse;
    using SetStringResponseOptional = boost::optional<SetStringResponseType>;
    SetStringResponseOptional set_string_response;

    using SetValueType = BICEPS::MM::SetValue;
    using SetValueOptional = boost::optional<SetValueType>;
    SetValueOptional set_value;

    using SetValueResponseType = BICEPS::MM::SetValueResponse;
    using SetValueResponseOptional = boost::optional<SetValueResponseType>;
    SetValueResponseOptional set_value_response;

    using EpisodicMetricReportType = BICEPS::MM::EpisodicMetricReport;
    using EpisodicMetricReportOptional = boost::optional<EpisodicMetricReportType>;
    EpisodicMetricReportOptional episodic_metric_report;

    using EpisodicComponentReportType = BICEPS::MM::EpisodicComponentReport;
    using EpisodicComponentReportOptional = boost::optional<EpisodicComponentReportType>;
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
