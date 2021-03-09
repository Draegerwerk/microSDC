#include "BICEPS_ParticipantModel.hpp"
#include <utility>

namespace BICEPS::PM
{

  CodedValue::CodedValue(CodeType code)
    : code(std::move(code))
  {
  }

  AbstractDescriptor::AbstractDescriptor(const DescriptorKind kind, HandleType handle)
    : handle(std::move(handle))
    , kind_(kind)
  {
  }

  AbstractDescriptor::DescriptorKind AbstractDescriptor::getKind() const
  {
    return kind_;
  }

  AbstractOperationDescriptor::AbstractOperationDescriptor(const DescriptorKind kind,
                                                           const HandleType& handle,
                                                           OperationTargetType operationTarget)
    : AbstractDescriptor(kind, handle)
    , operationTarget(std::move(operationTarget))
  {
  }

  bool AbstractOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::OPERATION_DESCRIPTOR &&
           other->getKind() < DescriptorKind::LAST_OPERATION_DESCRIPTOR;
  }

  SetStringOperationDescriptor::SetStringOperationDescriptor(
      const HandleType& handle, const OperationTargetType& operationTarget)
    : AbstractOperationDescriptor(DescriptorKind::SET_STRING_OPERATION_DESCRIPTOR, handle,
                                  operationTarget)
  {
  }
  bool SetStringOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::SET_STRING_OPERATION_DESCRIPTOR;
  }

  SetValueOperationDescriptor::SetValueOperationDescriptor(
      const HandleType& handle, const OperationTargetType& operation_target)
    : AbstractOperationDescriptor(DescriptorKind::SET_VALUE_OPERATION_DESCRIPTOR, handle,
                                  operation_target)
  {
  }
  bool SetValueOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::SET_VALUE_OPERATION_DESCRIPTOR;
  }

  AbstractContextDescriptor::AbstractContextDescriptor(const DescriptorKind kind,
                                                       const HandleType& handle)
    : AbstractDescriptor(kind, handle)
  {
  }

  bool AbstractContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::CONTEXT_DESCRIPTOR &&
           other->getKind() < DescriptorKind::LAST_CONTEXT_DESCRIPTOR;
  }

  PatientContextDescriptor::PatientContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::PATIENT_CONTEXT_DESCRIPTOR, handle)
  {
  }

  bool PatientContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::PATIENT_CONTEXT_DESCRIPTOR;
  }

  LocationContextDescriptor::LocationContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::LOCATION_CONTEXT_DESCRIPTOR, handle)
  {
  }

  bool LocationContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::LOCATION_CONTEXT_DESCRIPTOR;
  }

  SystemContextDescriptor::SystemContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::SYSTEM_CONTEXT_DESCRIPTOR, handle)
  {
  }

  bool SystemContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::SYSTEM_CONTEXT_DESCRIPTOR;
  }

  AbstractDeviceComponentDescriptor::AbstractDeviceComponentDescriptor(const DescriptorKind kind,
                                                                       const HandleType& handle)
    : AbstractDescriptor(kind, handle)
  {
  }

  bool AbstractDeviceComponentDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::DEVICE_COMPONENT_DESCRIPTOR &&
           other->getKind() < AbstractDescriptor::DescriptorKind::LAST_DEVICE_COMPONENT_DESCRIPTOR;
  }

  AbstractMetricDescriptor::AbstractMetricDescriptor(
      const DescriptorKind kind, const HandleType& handle, UnitType unit,
      const MetricCategoryType& metricCategory, const MetricAvailabilityType& metricAvailability)
    : AbstractDescriptor(kind, handle)
    , unit(std::move(unit))
    , metricCategory(metricCategory)
    , metricAvailability(metricAvailability)
  {
  }

  bool AbstractMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::METRIC_DESCRIPTOR &&
           other->getKind() < DescriptorKind::LAST_METRIC_DESCRIPTOR;
  }

  NumericMetricDescriptor::NumericMetricDescriptor(const HandleType& handle, const UnitType& unit,
                                                   const MetricCategoryType& metricCategory,
                                                   const MetricAvailabilityType& metricAvailability,
                                                   const ResolutionType& resolution)
    : AbstractMetricDescriptor(DescriptorKind::NUMERIC_METRIC_DESCRIPTOR, handle, unit,
                               metricCategory, metricAvailability)
    , resolution(resolution)
  {
  }

  bool NumericMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::NUMERIC_METRIC_DESCRIPTOR;
  }

  bool StringMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::STRING_METRIC_DESCRIPTOR;
  }
  StringMetricDescriptor::StringMetricDescriptor(const HandleType& handle, const UnitType& unit,
                                                 const MetricCategoryType& metric_category,
                                                 const MetricAvailabilityType& metric_availability)
    : AbstractMetricDescriptor(DescriptorKind::STRING_METRIC_DESCRIPTOR, handle, unit,
                               metric_category, metric_availability)
  {
  }

  Measurement::Measurement(MeasuredValueType measured_value, MeasurementUnitType measurement_unit)
    : measuredValue(measured_value)
    , measurementUnit(std::move(measurement_unit))
  {
  }

  AllowedValue::AllowedValue(ValueType value)
    : value(std::move(value))
  {
  }

  bool EnumStringMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::ENUM_STRING_METRIC_DESCRIPTOR;
  }
  EnumStringMetricDescriptor::EnumStringMetricDescriptor(
      const HandleType& handle, const UnitType& unit, const MetricCategoryType& metric_category,
      const MetricAvailabilityType& metric_availability, AllowedValueSequence allowed_value)
    : AbstractMetricDescriptor(DescriptorKind::ENUM_STRING_METRIC_DESCRIPTOR, handle, unit,
                               metric_category, metric_availability)
    , allowedValue(std::move(allowed_value))
  {
  }

  ChannelDescriptor::ChannelDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::CHANNEL_DESCRIPTOR, handle)
  {
  }

  bool ChannelDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::CHANNEL_DESCRIPTOR;
  }

  AbstractComplexDeviceComponentDescriptor::AbstractComplexDeviceComponentDescriptor(
      const DescriptorKind kind, const HandleType& handle)
    : AbstractDeviceComponentDescriptor(kind, handle)
  {
  }

  bool AbstractComplexDeviceComponentDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::COMPLEX_DEVICE_COMPONENT_DESCRIPTOR &&
           other->getKind() <
               AbstractDescriptor::DescriptorKind::LAST_COMPLEX_DEVICE_COMPONENT_DESCRIPTOR;
  }

  ScoDescriptor::ScoDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::SCO_DESCRIPTOR, handle)
  {
  }

  bool ScoDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::SCO_DESCRIPTOR;
  }

  VmdDescriptor::VmdDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(DescriptorKind::VMD_DESCRIPTOR, handle)
  {
  }

  bool VmdDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::VMD_DESCRIPTOR;
  }

  MdsDescriptor::MdsDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(DescriptorKind::MDS_DESCRIPTOR, handle)
  {
  }

  bool MdsDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::MDS_DESCRIPTOR;
  }

  MetricQuality::MetricQuality(const ValidityType& validity)
    : validity(validity)
  {
  }

  AbstractMetricValue::AbstractMetricValue(const MetricKind kind,
                                           const MetricQuality& metricQuality)
    : metricQuality(metricQuality)
    , kind_(kind)
  {
  }

  AbstractMetricValue::MetricKind AbstractMetricValue::getKind() const
  {
    return kind_;
  }

  NumericMetricValue::NumericMetricValue(const MetricQuality& metricQuality)
    : AbstractMetricValue(MetricKind::NUMERIC_METRIC, metricQuality)
  {
  }

  bool NumericMetricValue::classof(const AbstractMetricValue* other)
  {
    return other->getKind() == MetricKind::NUMERIC_METRIC;
  }

  AbstractState::AbstractState(const StateKind kind, DescriptorHandleType handle)
    : descriptorHandle(std::move(handle))
    , kind_(kind)
  {
  }

  AbstractState::StateKind AbstractState::getKind() const
  {
    return kind_;
  }

  AbstractMultiState::AbstractMultiState(const StateKind kind,
                                         const DescriptorHandleType& descriptorHandle,
                                         HandleType handle)
    : AbstractState(kind, descriptorHandle)
    , handle(std::move(handle))
  {
  }
  bool AbstractMultiState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::MULTI_STATE &&
           other->getKind() < StateKind::LAST_MULTI_STATE;
  }

  AbstractContextState::AbstractContextState(const StateKind kind,
                                             const DescriptorHandleType& descriptorHandle,
                                             const HandleType& handle)
    : AbstractMultiState(kind, descriptorHandle, handle)
  {
  }
  bool AbstractContextState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::CONTEXT_STATE &&
           other->getKind() < StateKind::LAST_CONTEXT_STATE;
  }

  LocationContextState::LocationContextState(const DescriptorHandleType& descriptorHandle,
                                             const HandleType& handle)
    : AbstractContextState(StateKind::LOCATION_CONTEXT_STATE, descriptorHandle, handle)
  {
  }

  bool LocationContextState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::LOCATION_CONTEXT_STATE;
  }

  AbstractDeviceComponentState::AbstractDeviceComponentState(StateKind kind,
                                                             DescriptorHandleType handle)
    : AbstractState(kind, std::move(handle))
  {
  }

  AbstractComplexDeviceComponentState::AbstractComplexDeviceComponentState(
      StateKind kind, DescriptorHandleType handle)
    : AbstractDeviceComponentState(kind, std::move(handle))
  {
  }

  MdsState::MdsState(DescriptorHandleType handle)
    : AbstractComplexDeviceComponentState(StateKind::MDS_STATE, std::move(handle))
  {
  }

  AbstractOperationState::AbstractOperationState(const StateKind kind,
                                                 const DescriptorHandleType& descriptorHandle,
                                                 const OperatingModeType& operatingMode)
    : AbstractState(kind, descriptorHandle)
    , operatingMode(operatingMode)
  {
  }

  bool AbstractOperationState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::OPERATION_STATE &&
           other->getKind() < StateKind::LAST_OPERATION_STATE;
  }

  SetStringOperationState::SetStringOperationState(const DescriptorHandleType& descriptor_handle,
                                                   const OperatingModeType& operating_mode)
    : AbstractOperationState(StateKind::SET_STRING_OPERATION_STATE, descriptor_handle,
                             operating_mode)
  {
  }
  bool SetStringOperationState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::SET_STRING_OPERATION_STATE;
  }

  SetValueOperationState::SetValueOperationState(const DescriptorHandleType& descriptorHandle,
                                                 const OperatingModeType& operatingMode)
    : AbstractOperationState(StateKind::SET_VALUE_OPERATION_STATE, descriptorHandle, operatingMode)
  {
  }
  bool SetValueOperationState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::SET_VALUE_OPERATION_STATE;
  }

  AbstractMetricState::AbstractMetricState(const StateKind kind, DescriptorHandleType handle)
    : AbstractState(kind, std::move(handle))
  {
  }

  bool AbstractMetricState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::METRIC_STATE &&
           other->getKind() < StateKind::LAST_METRIC_STATE;
  }

  NumericMetricState::NumericMetricState(DescriptorHandleType handle)
    : AbstractMetricState(StateKind::NUMERIC_METRIC_STATE, std::move(handle))
  {
  }
  bool NumericMetricState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::NUMERIC_METRIC_STATE;
  }

  bool StringMetricValue::classof(const AbstractMetricValue* other)
  {
    return other->getKind() == MetricKind::STRING_METRIC;
  }
  StringMetricValue::StringMetricValue(const MetricQuality& metric_quality)
    : AbstractMetricValue(MetricKind::STRING_METRIC, metric_quality)
  {
  }

  bool StringMetricState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::STRING_METRIC_STATE;
  }
  StringMetricState::StringMetricState(DescriptorHandleType handle)
    : AbstractMetricState(StateKind::STRING_METRIC_STATE, std::move(handle))
  {
  }

  Mdib::Mdib(SequenceIdType sequenceIdType)
    : sequenceId(std::move(sequenceIdType))
  {
  }
} // namespace BICEPS::PM
