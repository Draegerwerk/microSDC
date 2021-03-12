#pragma once

#include "MDPWSConstants.hpp"
#include "MessageModel.hpp"
#include "SDCConstants.hpp"
#include <sstream>
#include <string>

class MessageSerializer
{
public:
  /**
   * @brief Construct a new MessageSerializer
   */
  MessageSerializer();
  /**
   * @brief get the serialized string
   */
  std::string str() const;

  void serialize(const MESSAGEMODEL::Envelope& message);
  void serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Envelope& message);
  void serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Header& header);
  void serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Body& body);
  void serialize(rapidxml::xml_node<>* parent, const WS::ADDRESSING::RelatesToType& relates_to);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::ADDRESSING::EndpointReferenceType& endpoint_reference);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::AppSequenceType& app_sequence);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::HelloType& hello);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ByeType& bye);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ProbeMatchType& probe_match);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::DISCOVERY::ProbeMatchesType& probe_matches);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::DISCOVERY::ResolveMatchType& resolve_match);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::DISCOVERY::ResolveMatchesType& resolve_matches);
  void serialize(rapidxml::xml_node<>* parent, const WS::MEX::Metadata& metadata);
  void serialize(rapidxml::xml_node<>* parent, const WS::MEX::MetadataSection& metadata_section);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ScopesType& scopes);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::ThisModelType& this_model);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::ThisDeviceType& this_device);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::Relationship& relationship);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::HostServiceType& host);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::HostedServiceType& hosted);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::MM::GetMdibResponse& get_mdib_response);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Mdib& mdib);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdDescription& md_description);

  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::LocalizedText& localized_text);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::CodedValue& coded_value);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractDescriptor& abstract_descriptor);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractDeviceComponentDescriptor& device);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractComplexDeviceComponentDescriptor& complex_device);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdsDescriptor& mds_descriptor);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Metadata& metadata);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::SystemContextDescriptor& system_context);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractContextDescriptor& context);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::PatientContextDescriptor& patient_context);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::LocationContextDescriptor& location_context);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::VmdDescriptor& vmd);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::ChannelDescriptor& channel);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractMetricDescriptor& abstract_metric_descriptor);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Range& range);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdState& md_state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AbstractState& state);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractDeviceComponentState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::CalibrationInfo& info);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::CalibrationDocumentation& documentation);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::PhysicalConnectorInfo& info);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::SystemContextState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::ChannelState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::OperationGroup& operation_group);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::ScoState& state);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractComplexDeviceComponentState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdsState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::VmdState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AbstractMetricState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::NumericMetricState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::StringMetricState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::EnumStringMetricState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AbstractMultiState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AbstractContextState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::LocationContextState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::LocationDetail& location_detail);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AbstractMetricValue& metric_value);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::NumericMetricValue& metric_value);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::StringMetricValue& metric_value);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MetricQuality& quality);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AbstractOperationState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::SetValueOperationState& state);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::SetStringOperationState& state);

  void serialize(rapidxml::xml_node<>* parent,
                 const WS::EVENTING::SubscribeResponse& subscribe_response);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::ADDRESSING::ReferenceParametersType& reference_parameters);
  void serialize(rapidxml::xml_node<>* parent, const WS::EVENTING::RenewResponse& renew_response);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::MM::AbstractSetResponse& set_response);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::MM::InvocationInfo& invocation_info);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::MM::EpisodicMetricReport& report);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::MM::MetricReportPart&);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::ScoDescriptor& sco);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::AbstractOperationDescriptor& operation);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::SetStringOperationDescriptor& operation);
  void serialize(rapidxml::xml_node<>* parent,
                 const BICEPS::PM::SetValueOperationDescriptor& operation);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::InstanceIdentifier& identifier);
  void serialize(rapidxml::xml_node<>* parent, const WS::EVENTING::ExpirationType& expiration);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::AllowedValue& allowed_value);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Measurement& measurement);

  template <typename T>
  void append_to_string_attribute(rapidxml::xml_node<>* parent, const char* name, T value)
  {
    auto* str = xml_document_->allocate_string(std::to_string(value).c_str());
    auto* attr = xml_document_->allocate_attribute(name, str);
    parent->append_attribute(attr);
  }
  void append_attribute(rapidxml::xml_node<>* parent, const char* name, const std::string& value);

  static std::string to_string(BICEPS::PM::SafetyClassification);
  static std::string to_string(const WS::DISCOVERY::UriListType& uri_list);
  static std::string to_string(const WS::DISCOVERY::QNameListType& q_name_list);
  static std::string to_string(BICEPS::PM::LocalizedTextWidth);
  static std::string to_string(BICEPS::PM::MetricCategory);
  static std::string to_string(BICEPS::PM::MetricAvailability);
  static std::string to_string(BICEPS::PM::MeasurementValidity);
  static std::string to_string(BICEPS::MM::InvocationState invocation_state);
  static std::string to_string(BICEPS::MM::InvocationError invocation_error);
  static std::string to_string(BICEPS::PM::ContextAssociation context_association);
  static std::string to_string(BICEPS::PM::OperatingMode operating_mode);
  static std::string to_string(BICEPS::PM::ComponentActivation activation);
  static std::string to_string(BICEPS::PM::MdsOperatingMode operating_mode);
  static std::string to_string(BICEPS::PM::CalibrationState calib_state);
  static std::string to_string(BICEPS::PM::CalibrationType calib_type);
  static std::string to_string(Duration duration);

private:
  std::unique_ptr<rapidxml::xml_document<>> xml_document_;
};
