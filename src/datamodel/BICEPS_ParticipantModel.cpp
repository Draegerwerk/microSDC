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

  SetValueOperationDescriptor::SetValueOperationDescriptor(
      const HandleType& handle, const OperationTargetType& operationTarget)
    : AbstractOperationDescriptor(DescriptorKind::SET_VALUE_OPERATION_DESCRIPTOR, handle,
                                  operationTarget)
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

  MetricQualityType::MetricQualityType(const ValidityType& validity)
    : validity(validity)
  {
  }

  AbstractMetricValue::AbstractMetricValue(const MetricKind kind,
                                           const MetricQualityType& metricQuality)
    : metricQuality(metricQuality)
    , kind_(kind)
  {
  }

  AbstractMetricValue::MetricKind AbstractMetricValue::getKind() const
  {
    return kind_;
  }

  NumericMetricValue::NumericMetricValue(const MetricQualityType& metricQuality)
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

  Mdib::Mdib(SequenceIdType sequenceIdType)
    : sequenceId(std::move(sequenceIdType))
  {
  }
} // namespace BICEPS::PM
