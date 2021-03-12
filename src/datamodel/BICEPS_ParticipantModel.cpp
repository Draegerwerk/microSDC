#include "BICEPS_ParticipantModel.hpp"
#include <utility>

namespace BICEPS::PM
{

  LocalizedText::LocalizedText(std::string content)
    : content(std::move(content))
  {
  }

  CodedValue::CodedValue(CodeType code)
    : code(std::move(code))
  {
  }

  AbstractDescriptor::AbstractDescriptor(const DescriptorKind kind, HandleType handle)
    : handle(std::move(handle))
    , kind_(kind)
  {
  }

  AbstractDescriptor::DescriptorKind AbstractDescriptor::get_kind() const
  {
    return kind_;
  }

  AbstractOperationDescriptor::AbstractOperationDescriptor(const DescriptorKind kind,
                                                           const HandleType& handle,
                                                           OperationTargetType operation_target)
    : AbstractDescriptor(kind, handle)
    , operation_target(std::move(operation_target))
  {
  }

  bool AbstractOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() >= DescriptorKind::OPERATION_DESCRIPTOR &&
           other->get_kind() < DescriptorKind::LAST_OPERATION_DESCRIPTOR;
  }

  SetStringOperationDescriptor::SetStringOperationDescriptor(
      const HandleType& handle, const OperationTargetType& operation_target)
    : AbstractOperationDescriptor(DescriptorKind::SET_STRING_OPERATION_DESCRIPTOR, handle,
                                  operation_target)
  {
  }
  bool SetStringOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::SET_STRING_OPERATION_DESCRIPTOR;
  }

  SetValueOperationDescriptor::SetValueOperationDescriptor(
      const HandleType& handle, const OperationTargetType& operation_target)
    : AbstractOperationDescriptor(DescriptorKind::SET_VALUE_OPERATION_DESCRIPTOR, handle,
                                  operation_target)
  {
  }
  bool SetValueOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::SET_VALUE_OPERATION_DESCRIPTOR;
  }

  AbstractContextDescriptor::AbstractContextDescriptor(const DescriptorKind kind,
                                                       const HandleType& handle)
    : AbstractDescriptor(kind, handle)
  {
  }

  bool AbstractContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() >= DescriptorKind::CONTEXT_DESCRIPTOR &&
           other->get_kind() < DescriptorKind::LAST_CONTEXT_DESCRIPTOR;
  }

  PatientContextDescriptor::PatientContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::PATIENT_CONTEXT_DESCRIPTOR, handle)
  {
  }

  bool PatientContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::PATIENT_CONTEXT_DESCRIPTOR;
  }

  LocationContextDescriptor::LocationContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::LOCATION_CONTEXT_DESCRIPTOR, handle)
  {
  }

  bool LocationContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::LOCATION_CONTEXT_DESCRIPTOR;
  }

  SystemContextDescriptor::SystemContextDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::SYSTEM_CONTEXT_DESCRIPTOR, handle)
  {
  }

  bool SystemContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::SYSTEM_CONTEXT_DESCRIPTOR;
  }

  AbstractDeviceComponentDescriptor::AbstractDeviceComponentDescriptor(const DescriptorKind kind,
                                                                       const HandleType& handle)
    : AbstractDescriptor(kind, handle)
  {
  }

  bool AbstractDeviceComponentDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() >= DescriptorKind::DEVICE_COMPONENT_DESCRIPTOR &&
           other->get_kind() < AbstractDescriptor::DescriptorKind::LAST_DEVICE_COMPONENT_DESCRIPTOR;
  }

  AbstractMetricDescriptor::AbstractMetricDescriptor(
      const DescriptorKind kind, const HandleType& handle, UnitType unit,
      const MetricCategoryType& metric_category, const MetricAvailabilityType& metric_availability)
    : AbstractDescriptor(kind, handle)
    , unit(std::move(unit))
    , metric_category(metric_category)
    , metric_availability(metric_availability)
  {
  }

  bool AbstractMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() >= DescriptorKind::METRIC_DESCRIPTOR &&
           other->get_kind() < DescriptorKind::LAST_METRIC_DESCRIPTOR;
  }

  NumericMetricDescriptor::NumericMetricDescriptor(
      const HandleType& handle, const UnitType& unit, const MetricCategoryType& metric_category,
      const MetricAvailabilityType& metric_availability, const ResolutionType& resolution)
    : AbstractMetricDescriptor(DescriptorKind::NUMERIC_METRIC_DESCRIPTOR, handle, unit,
                               metric_category, metric_availability)
    , resolution(resolution)
  {
  }

  bool NumericMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::NUMERIC_METRIC_DESCRIPTOR;
  }

  bool StringMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::STRING_METRIC_DESCRIPTOR;
  }
  StringMetricDescriptor::StringMetricDescriptor(const HandleType& handle, const UnitType& unit,
                                                 const MetricCategoryType& metric_category,
                                                 const MetricAvailabilityType& metric_availability)
    : AbstractMetricDescriptor(DescriptorKind::STRING_METRIC_DESCRIPTOR, handle, unit,
                               metric_category, metric_availability)
  {
  }

  Measurement::Measurement(MeasuredValueType measured_value, MeasurementUnitType measurement_unit)
    : measured_value(measured_value)
    , measurement_unit(std::move(measurement_unit))
  {
  }

  AllowedValue::AllowedValue(ValueType value)
    : value(std::move(value))
  {
  }

  bool EnumStringMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::ENUM_STRING_METRIC_DESCRIPTOR;
  }
  EnumStringMetricDescriptor::EnumStringMetricDescriptor(
      const HandleType& handle, const UnitType& unit, const MetricCategoryType& metric_category,
      const MetricAvailabilityType& metric_availability, AllowedValueSequence allowed_value)
    : AbstractMetricDescriptor(DescriptorKind::ENUM_STRING_METRIC_DESCRIPTOR, handle, unit,
                               metric_category, metric_availability)
    , allowed_value(std::move(allowed_value))
  {
  }

  ChannelDescriptor::ChannelDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::CHANNEL_DESCRIPTOR, handle)
  {
  }

  bool ChannelDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::CHANNEL_DESCRIPTOR;
  }

  AbstractComplexDeviceComponentDescriptor::AbstractComplexDeviceComponentDescriptor(
      const DescriptorKind kind, const HandleType& handle)
    : AbstractDeviceComponentDescriptor(kind, handle)
  {
  }

  bool AbstractComplexDeviceComponentDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() >= DescriptorKind::COMPLEX_DEVICE_COMPONENT_DESCRIPTOR &&
           other->get_kind() <
               AbstractDescriptor::DescriptorKind::LAST_COMPLEX_DEVICE_COMPONENT_DESCRIPTOR;
  }

  ScoDescriptor::ScoDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::SCO_DESCRIPTOR, handle)
  {
  }

  bool ScoDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::SCO_DESCRIPTOR;
  }

  VmdDescriptor::VmdDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(DescriptorKind::VMD_DESCRIPTOR, handle)
  {
  }

  bool VmdDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::VMD_DESCRIPTOR;
  }

  MdsDescriptor::MdsDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(DescriptorKind::MDS_DESCRIPTOR, handle)
  {
  }

  bool MdsDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->get_kind() == DescriptorKind::MDS_DESCRIPTOR;
  }

  MetricQuality::MetricQuality(const ValidityType& validity)
    : validity(validity)
  {
  }

  AbstractMetricValue::AbstractMetricValue(const MetricKind kind, MetricQuality metric_quality)
    : metric_quality(std::move(metric_quality))
    , kind_(kind)
  {
  }

  AbstractMetricValue::MetricKind AbstractMetricValue::get_kind() const
  {
    return kind_;
  }

  NumericMetricValue::NumericMetricValue(const MetricQuality& metric_quality)
    : AbstractMetricValue(MetricKind::NUMERIC_METRIC, metric_quality)
  {
  }

  bool NumericMetricValue::classof(const AbstractMetricValue* other)
  {
    return other->get_kind() == MetricKind::NUMERIC_METRIC;
  }

  AbstractState::AbstractState(const StateKind kind, DescriptorHandleType handle)
    : descriptor_handle(std::move(handle))
    , kind_(kind)
  {
  }

  AbstractState::StateKind AbstractState::get_kind() const
  {
    return kind_;
  }

  AbstractMultiState::AbstractMultiState(const StateKind kind,
                                         const DescriptorHandleType& descriptor_handle,
                                         HandleType handle)
    : AbstractState(kind, descriptor_handle)
    , handle(std::move(handle))
  {
  }
  bool AbstractMultiState::classof(const AbstractState* other)
  {
    return other->get_kind() >= StateKind::MULTI_STATE &&
           other->get_kind() < StateKind::LAST_MULTI_STATE;
  }

  AbstractContextState::AbstractContextState(const StateKind kind,
                                             const DescriptorHandleType& descriptor_handle,
                                             const HandleType& handle)
    : AbstractMultiState(kind, descriptor_handle, handle)
  {
  }
  bool AbstractContextState::classof(const AbstractState* other)
  {
    return other->get_kind() >= StateKind::CONTEXT_STATE &&
           other->get_kind() < StateKind::LAST_CONTEXT_STATE;
  }

  LocationContextState::LocationContextState(const DescriptorHandleType& descriptor_handle,
                                             const HandleType& handle)
    : AbstractContextState(StateKind::LOCATION_CONTEXT_STATE, descriptor_handle, handle)
  {
  }

  bool LocationContextState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::LOCATION_CONTEXT_STATE;
  }

  AbstractDeviceComponentState::AbstractDeviceComponentState(StateKind kind,
                                                             DescriptorHandleType handle)
    : AbstractState(kind, std::move(handle))
  {
  }

  bool SystemContextState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::SYSTEM_CONTEXT_STATE;
  }
  SystemContextState::SystemContextState(const DescriptorHandleType& descriptor_handle)
    : AbstractDeviceComponentState(StateKind::SYSTEM_CONTEXT_STATE, descriptor_handle)
  {
  }

  bool ChannelState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::CHANNEL_STATE;
  }
  ChannelState::ChannelState(const DescriptorHandleType& descriptor_handle)
    : AbstractDeviceComponentState(StateKind::CHANNEL_STATE, descriptor_handle)
  {
  }

  OperationGroup::OperationGroup(TypeType type)
    : type(std::move(type))
  {
  }

  bool ScoState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::SCO_STATE;
  }
  ScoState::ScoState(const DescriptorHandleType& descriptor_handle)
    : AbstractDeviceComponentState(StateKind::SCO_STATE, descriptor_handle)
  {
  }

  AbstractComplexDeviceComponentState::AbstractComplexDeviceComponentState(
      StateKind kind, DescriptorHandleType handle)
    : AbstractDeviceComponentState(kind, std::move(handle))
  {
  }

  bool MdsState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::MDS_STATE;
  }
  MdsState::MdsState(DescriptorHandleType handle)
    : AbstractComplexDeviceComponentState(StateKind::MDS_STATE, std::move(handle))
  {
  }

  bool VmdState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::VMD_STATE;
  }
  VmdState::VmdState(DescriptorHandleType handle)
    : AbstractComplexDeviceComponentState(StateKind::VMD_STATE, std::move(handle))
  {
  }

  AbstractOperationState::AbstractOperationState(const StateKind kind,
                                                 const DescriptorHandleType& descriptor_handle,
                                                 const OperatingModeType& operating_mode)
    : AbstractState(kind, descriptor_handle)
    , operating_mode(operating_mode)
  {
  }

  bool AbstractOperationState::classof(const AbstractState* other)
  {
    return other->get_kind() >= StateKind::OPERATION_STATE &&
           other->get_kind() < StateKind::LAST_OPERATION_STATE;
  }

  SetStringOperationState::SetStringOperationState(const DescriptorHandleType& descriptor_handle,
                                                   const OperatingModeType& operating_mode)
    : AbstractOperationState(StateKind::SET_STRING_OPERATION_STATE, descriptor_handle,
                             operating_mode)
  {
  }
  bool SetStringOperationState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::SET_STRING_OPERATION_STATE;
  }

  SetValueOperationState::SetValueOperationState(const DescriptorHandleType& descriptor_handle,
                                                 const OperatingModeType& operating_mode)
    : AbstractOperationState(StateKind::SET_VALUE_OPERATION_STATE, descriptor_handle,
                             operating_mode)
  {
  }
  bool SetValueOperationState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::SET_VALUE_OPERATION_STATE;
  }

  AbstractMetricState::AbstractMetricState(const StateKind kind, DescriptorHandleType handle)
    : AbstractState(kind, std::move(handle))
  {
  }

  bool AbstractMetricState::classof(const AbstractState* other)
  {
    return other->get_kind() >= StateKind::METRIC_STATE &&
           other->get_kind() < StateKind::LAST_METRIC_STATE;
  }

  NumericMetricState::NumericMetricState(DescriptorHandleType handle)
    : AbstractMetricState(StateKind::NUMERIC_METRIC_STATE, std::move(handle))
  {
  }
  bool NumericMetricState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::NUMERIC_METRIC_STATE;
  }

  bool StringMetricValue::classof(const AbstractMetricValue* other)
  {
    return other->get_kind() == MetricKind::STRING_METRIC;
  }
  StringMetricValue::StringMetricValue(const MetricQuality& metric_quality)
    : AbstractMetricValue(MetricKind::STRING_METRIC, metric_quality)
  {
  }

  bool StringMetricState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::STRING_METRIC_STATE;
  }
  StringMetricState::StringMetricState(DescriptorHandleType handle)
    : AbstractMetricState(StateKind::STRING_METRIC_STATE, std::move(handle))
  {
  }
  StringMetricState::StringMetricState(StateKind kind, DescriptorHandleType handle)
    : AbstractMetricState(kind, std::move(handle))
  {
  }

  bool EnumStringMetricState::classof(const AbstractState* other)
  {
    return other->get_kind() == StateKind::ENUM_STRING_METRIC_STATE;
  }
  EnumStringMetricState::EnumStringMetricState(DescriptorHandleType handle)
    : StringMetricState(StateKind::ENUM_STRING_METRIC_STATE, std::move(handle))
  {
  }

  Mdib::Mdib(SequenceIdType sequence_id_type)
    : sequence_id(std::move(sequence_id_type))
  {
  }
} // namespace BICEPS::PM
