#include "BICEPS_ParticipantModel.hpp"

#include <utility>

namespace BICEPS::PM
{

  // Range
  //
  const Range::LowerOptional& Range::Lower() const
  {
    return Lower_;
  }
  Range::LowerOptional& Range::Lower()
  {
    return Lower_;
  }
  const Range::UpperOptional& Range::Upper() const
  {
    return Upper_;
  }
  Range::UpperOptional& Range::Upper()
  {
    return Upper_;
  }
  const Range::StepWidthOptional& Range::StepWidth() const
  {
    return StepWidth_;
  }
  Range::StepWidthOptional& Range::StepWidth()
  {
    return StepWidth_;
  }
  const Range::RelativeAccuracyOptional& Range::RelativeAccuracy() const
  {
    return RelativeAccuracy_;
  }
  Range::RelativeAccuracyOptional& Range::RelativeAccuracy()
  {
    return RelativeAccuracy_;
  }
  const Range::AbsoluteAccuracyOptional& Range::AbsoluteAccuracy() const
  {
    return AbsoluteAccuracy_;
  }
  Range::AbsoluteAccuracyOptional& Range::AbsoluteAccuracy()
  {
    return AbsoluteAccuracy_;
  }

  // CodedValue
  //
  CodedValue::CodedValue(CodeType code)
    : Code_(std::move(code))
  {
  }
  const CodedValue::CodeType& CodedValue::Code() const
  {
    return Code_;
  }
  CodedValue::CodeType& CodedValue::Code()
  {
    return Code_;
  }

  // Metadata
  //
  const Metadata::ManufacturerSequence& Metadata::Manufacturer() const
  {
    return Manufacturer_;
  }
  Metadata::ManufacturerSequence& Metadata::Manufacturer()
  {
    return Manufacturer_;
  }
  const Metadata::ModelNameSequence& Metadata::ModelName() const
  {
    return ModelName_;
  }
  Metadata::ModelNameSequence& Metadata::ModelName()
  {
    return ModelName_;
  }
  const Metadata::ModelNumberOptional& Metadata::ModelNumber() const
  {
    return ModelNumber_;
  }
  Metadata::ModelNumberOptional& Metadata::ModelNumber()
  {
    return ModelNumber_;
  }
  const Metadata::SerialNumberSequence& Metadata::SerialNumber() const
  {
    return SerialNumber_;
  }
  Metadata::SerialNumberSequence& Metadata::SerialNumber()
  {
    return SerialNumber_;
  }

  // AbstractDescriptor
  //
  AbstractDescriptor::AbstractDescriptor(const DescriptorKind kind, HandleType handle)
    : kind_(kind)
    , Handle_(std::move(handle))
  {
  }
  AbstractDescriptor::DescriptorKind AbstractDescriptor::getKind() const
  {
    return kind_;
  }
  const AbstractDescriptor::TypeOptional& AbstractDescriptor::Type() const
  {
    return Type_;
  }
  AbstractDescriptor::TypeOptional& AbstractDescriptor::Type()
  {
    return Type_;
  }
  const AbstractDescriptor::HandleType& AbstractDescriptor::Handle() const
  {
    return Handle_;
  }
  AbstractDescriptor::HandleType& AbstractDescriptor::Handle()
  {
    return Handle_;
  }
  const AbstractDescriptor::DescriptorVersionOptional& AbstractDescriptor::DescriptorVersion() const
  {
    return DescriptorVersion_;
  }
  AbstractDescriptor::DescriptorVersionOptional& AbstractDescriptor::DescriptorVersion()
  {
    return DescriptorVersion_;
  }
  const AbstractDescriptor::SafetyClassificationOptional&
  AbstractDescriptor::SafetyClassification() const
  {
    return SafetyClassification_;
  }
  AbstractDescriptor::SafetyClassificationOptional& AbstractDescriptor::SafetyClassification()
  {
    return SafetyClassification_;
  }

  // AbstractOperationDescriptor
  //
  AbstractOperationDescriptor::AbstractOperationDescriptor(const DescriptorKind kind,
                                                           const HandleType& handle,
                                                           OperationTargetType operationTarget)
    : AbstractDescriptor(kind, handle)
    , OperationTarget_(std::move(operationTarget))
  {
  }
  const AbstractOperationDescriptor::OperationTargetType&
  AbstractOperationDescriptor::OperationTarget() const
  {
    return OperationTarget_;
  }
  AbstractOperationDescriptor::OperationTargetType& AbstractOperationDescriptor::OperationTarget()
  {
    return OperationTarget_;
  }
  bool AbstractOperationDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::OPERATION_DESCRIPTOR &&
           other->getKind() < DescriptorKind::LAST_OPERATION_DESCRIPTOR;
  }

  // SetValueOperationDescriptor
  //
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

  // AbstractContextDescriptor
  //
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

  // PatientContext
  //
  PatientContextDescriptor::PatientContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::PATIENT_CONTEXT_DESCRIPTOR, handle)
  {
  }
  bool PatientContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::PATIENT_CONTEXT_DESCRIPTOR;
  }

  // LocationContext
  //
  LocationContextDescriptor::LocationContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::LOCATION_CONTEXT_DESCRIPTOR, handle)
  {
  }
  bool LocationContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::LOCATION_CONTEXT_DESCRIPTOR;
  }

  // SystemContext
  //
  SystemContextDescriptor::SystemContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(DescriptorKind::SYSTEM_CONTEXT_DESCRIPTOR, handle)
  {
  }
  bool SystemContextDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::SYSTEM_CONTEXT_DESCRIPTOR;
  }
  const SystemContextDescriptor::PatientContextOptional&
  SystemContextDescriptor::PatientContext() const
  {
    return PatientContext_;
  }
  SystemContextDescriptor::PatientContextOptional& SystemContextDescriptor::PatientContext()
  {
    return PatientContext_;
  }
  const SystemContextDescriptor::LocationContextOptional&
  SystemContextDescriptor::LocationContext() const
  {
    return LocationContext_;
  }
  SystemContextDescriptor::LocationContextOptional& SystemContextDescriptor::LocationContext()
  {
    return LocationContext_;
  }

  // AbstractDeviceComponentDescriptor
  //
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

  // AbstractMetricDescriptor
  //
  AbstractMetricDescriptor::AbstractMetricDescriptor(
      const DescriptorKind kind, const HandleType& handle, UnitType unit,
      const MetricCategoryType& metricCategory, const MetricAvailabilityType& metricAvailability)
    : AbstractDescriptor(kind, handle)
    , Unit_(std::move(unit))
    , MetricCategory_(metricCategory)
    , MetricAvailability_(metricAvailability)
  {
  }
  bool AbstractMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() >= DescriptorKind::METRIC_DESCRIPTOR &&
           other->getKind() < DescriptorKind::LAST_METRIC_DESCRIPTOR;
  }
  const AbstractMetricDescriptor::UnitType& AbstractMetricDescriptor::Unit() const
  {
    return Unit_;
  }
  AbstractMetricDescriptor::UnitType& AbstractMetricDescriptor::Unit()
  {
    return Unit_;
  }
  const AbstractMetricDescriptor::MetricCategoryType&
  AbstractMetricDescriptor::MetricCategory() const
  {
    return MetricCategory_;
  }
  AbstractMetricDescriptor::MetricCategoryType& AbstractMetricDescriptor::MetricCategory()
  {
    return MetricCategory_;
  }
  const AbstractMetricDescriptor::MetricAvailabilityType&
  AbstractMetricDescriptor::MetricAvailability() const
  {
    return MetricAvailability_;
  }
  AbstractMetricDescriptor::MetricAvailabilityType& AbstractMetricDescriptor::MetricAvailability()
  {
    return MetricAvailability_;
  }


  // NumericMetricDescriptor
  //
  NumericMetricDescriptor::NumericMetricDescriptor(const HandleType& handle, const UnitType& unit,
                                                   const MetricCategoryType& metricCategory,
                                                   const MetricAvailabilityType& metricAvailability,
                                                   const ResolutionType& resolution)
    : AbstractMetricDescriptor(DescriptorKind::NUMERIC_METRIC_DESCRIPTOR, handle, unit,
                               metricCategory, metricAvailability)
    , Resolution_(resolution)
  {
  }

  bool NumericMetricDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::NUMERIC_METRIC_DESCRIPTOR;
  }

  const NumericMetricDescriptor::TechnicalRangeSequence&
  NumericMetricDescriptor::TechnicalRange() const
  {
    return TechnicalRange_;
  }
  NumericMetricDescriptor::TechnicalRangeSequence& NumericMetricDescriptor::TechnicalRange()
  {
    return TechnicalRange_;
  }
  const NumericMetricDescriptor::ResolutionType& NumericMetricDescriptor::Resolution() const
  {
    return Resolution_;
  }
  NumericMetricDescriptor::ResolutionType& NumericMetricDescriptor::Resolution()
  {
    return Resolution_;
  }
  const NumericMetricDescriptor::AveragingPeriodOptional&
  NumericMetricDescriptor::AveragingPeriod() const
  {
    return AveragingPeriod_;
  }
  NumericMetricDescriptor::AveragingPeriodOptional& NumericMetricDescriptor::AveragingPeriod()
  {
    return AveragingPeriod_;
  }

  // ChannelDescriptor
  //
  ChannelDescriptor::ChannelDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::CHANNEL_DESCRIPTOR, handle)
  {
  }
  bool ChannelDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::CHANNEL_DESCRIPTOR;
  }
  const ChannelDescriptor::MetricSequence& ChannelDescriptor::Metric() const
  {
    return Metric_;
  }
  ChannelDescriptor::MetricSequence& ChannelDescriptor::Metric()
  {
    return Metric_;
  }

  // AbstractComplexDeviceComponentDescriptor
  //
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
  // ScoDescriptor::
  //
  ScoDescriptor::ScoDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(DescriptorKind::SCO_DESCRIPTOR, handle)
  {
  }
  bool ScoDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::SCO_DESCRIPTOR;
  }
  const ScoDescriptor::OperationSequence& ScoDescriptor::Operation() const
  {
    return Operation_;
  }
  ScoDescriptor::OperationSequence& ScoDescriptor::Operation()
  {
    return Operation_;
  }

  // VmdDescriptor
  //
  VmdDescriptor::VmdDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(DescriptorKind::VMD_DESCRIPTOR, handle)
  {
  }
  const VmdDescriptor::ChannelSequence& VmdDescriptor::Channel() const
  {
    return Channel_;
  }
  VmdDescriptor::ChannelSequence& VmdDescriptor::Channel()
  {
    return Channel_;
  }
  const VmdDescriptor::ScoOptional& VmdDescriptor::Sco() const
  {
    return Sco_;
  }
  VmdDescriptor::ScoOptional& VmdDescriptor::Sco()
  {
    return Sco_;
  }
  bool VmdDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::VMD_DESCRIPTOR;
  }

  // MdsDescriptor
  //
  MdsDescriptor::MdsDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(DescriptorKind::MDS_DESCRIPTOR, handle)
  {
  }
  bool MdsDescriptor::classof(const AbstractDescriptor* other)
  {
    return other->getKind() == DescriptorKind::MDS_DESCRIPTOR;
  }
  const MdsDescriptor::VmdSequence& MdsDescriptor::Vmd() const
  {
    return Vmd_;
  }
  MdsDescriptor::VmdSequence& MdsDescriptor::Vmd()
  {
    return Vmd_;
  }
  const MdsDescriptor::SystemContextOptional& MdsDescriptor::SystemContext() const
  {
    return SystemContext_;
  }
  MdsDescriptor::SystemContextOptional& MdsDescriptor::SystemContext()
  {
    return SystemContext_;
  }
  const MdsDescriptor::MetaDataOptional& MdsDescriptor::MetaData() const
  {
    return MetaData_;
  }
  MdsDescriptor::MetaDataOptional& MdsDescriptor::MetaData()
  {
    return MetaData_;
  }

  // MdDescription
  //
  const MdDescription::MdsSequence& MdDescription::Mds() const
  {
    return Mds_;
  }
  MdDescription::MdsSequence& MdDescription::Mds()
  {
    return Mds_;
  }

  // MetricQuality
  //
  MetricQuality::MetricQuality(const ValidityType& validity)
    : Validity_(validity)
  {
  }
  const MetricQuality::ValidityType& MetricQuality::Validity() const
  {
    return Validity_;
  }
  MetricQuality::ValidityType& MetricQuality::Validity()
  {
    return Validity_;
  }

  // AbstractMetricValue
  //
  AbstractMetricValue::AbstractMetricValue(const MetricKind kind,
                                           const MetricQualityType& metricQuality)
    : kind_(kind)
    , MetricQuality_(metricQuality)
  {
  }
  AbstractMetricValue::MetricKind AbstractMetricValue::getKind() const
  {
    return kind_;
  }
  const AbstractMetricValue::MetricQualityType& AbstractMetricValue::Quality() const
  {
    return MetricQuality_;
  }
  AbstractMetricValue::MetricQualityType& AbstractMetricValue::Quality()
  {
    return MetricQuality_;
  }

  // NumericMetricValue
  //
  NumericMetricValue::NumericMetricValue(const MetricQuality& metricQuality)
    : AbstractMetricValue(MetricKind::NUMERIC_METRIC, metricQuality)
  {
  }
  bool NumericMetricValue::classof(const AbstractMetricValue* other)
  {
    return other->getKind() == MetricKind::NUMERIC_METRIC;
  }
  const NumericMetricValue::ValueOptional& NumericMetricValue::Value() const
  {
    return Value_;
  }
  NumericMetricValue::ValueOptional& NumericMetricValue::Value()
  {
    return Value_;
  }

  // AbstractState
  //
  const AbstractState::StateVersionOptional& AbstractState::StateVersion() const
  {
    return StateVersion_;
  }
  AbstractState::StateVersionOptional& AbstractState::StateVersion()
  {
    return StateVersion_;
  }
  AbstractState::AbstractState(const StateKind kind, DescriptorHandleType handle)
    : kind_(kind)
    , DescriptorHandle_(std::move(handle))
  {
  }
  AbstractState::StateKind AbstractState::getKind() const
  {
    return kind_;
  }
  const AbstractState::DescriptorHandleType& AbstractState::DescriptorHandle() const
  {
    return DescriptorHandle_;
  }
  AbstractState::DescriptorHandleType& AbstractState::DescriptorHandle()
  {
    return DescriptorHandle_;
  }

  // AbstractMultiState
  //
  const AbstractMultiState::HandleType& AbstractMultiState::Handle() const
  {
    return Handle_;
  }
  AbstractMultiState::HandleType& AbstractMultiState::Handle()
  {
    return Handle_;
  }
  AbstractMultiState::AbstractMultiState(const StateKind kind,
                                         const DescriptorHandleType& descriptorHandle,
                                         HandleType handle)
    : AbstractState(kind, descriptorHandle)
    , Handle_(std::move(handle))
  {
  }
  bool AbstractMultiState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::MULTI_STATE &&
           other->getKind() < StateKind::LAST_MULTI_STATE;
  }

  // InstanceIdentifier
  //
  const InstanceIdentifier::ExtensionOptional& InstanceIdentifier::Extension() const
  {
    return Extension_;
  }
  InstanceIdentifier::ExtensionOptional& InstanceIdentifier::Extension()
  {
    return Extension_;
  }
  const InstanceIdentifier::RootOptional& InstanceIdentifier::Root() const
  {
    return Root_;
  }
  InstanceIdentifier::RootOptional& InstanceIdentifier::Root()
  {
    return Root_;
  }

  // AbstractContextState
  //
  const AbstractContextState::BindingMdibVersionOptional&
  AbstractContextState::BindingMdibVersion() const
  {
    return BindingMdibVersion_;
  }
  AbstractContextState::BindingMdibVersionOptional& AbstractContextState::BindingMdibVersion()
  {
    return BindingMdibVersion_;
  }
  const AbstractContextState::ContextAssociationOptional&
  AbstractContextState::ContextAssociation() const
  {
    return ContextAssociation_;
  }
  AbstractContextState::ContextAssociationOptional& AbstractContextState::ContextAssociation()
  {
    return ContextAssociation_;
  }
  const AbstractContextState::ValidatorSequence& AbstractContextState::Validator() const
  {
    return Validator_;
  }
  AbstractContextState::ValidatorSequence& AbstractContextState::Validator()
  {
    return Validator_;
  }
  const AbstractContextState::IdentificationSequence& AbstractContextState::Identification() const
  {
    return Identification_;
  }
  AbstractContextState::IdentificationSequence& AbstractContextState::Identification()
  {
    return Identification_;
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

  // LocationDetailType
  //
  const LocationDetailType::PoCOptional& LocationDetailType::PoC() const
  {
    return PoC_;
  }
  LocationDetailType::PoCOptional& LocationDetailType::PoC()
  {
    return PoC_;
  }
  const LocationDetailType::RoomOptional& LocationDetailType::Room() const
  {
    return Room_;
  }
  LocationDetailType::RoomOptional& LocationDetailType::Room()
  {
    return Room_;
  }
  const LocationDetailType::BedOptional& LocationDetailType::Bed() const
  {
    return Bed_;
  }
  LocationDetailType::BedOptional& LocationDetailType::Bed()
  {
    return Bed_;
  }
  const LocationDetailType::FacilityOptional& LocationDetailType::Facility() const
  {
    return Facility_;
  }
  LocationDetailType::FacilityOptional& LocationDetailType::Facility()
  {
    return Facility_;
  }
  const LocationDetailType::BuildingOptional& LocationDetailType::Building() const
  {
    return Building_;
  }
  LocationDetailType::BuildingOptional& LocationDetailType::Building()
  {
    return Building_;
  }
  const LocationDetailType::FloorOptional& LocationDetailType::Floor() const
  {
    return Floor_;
  }
  LocationDetailType::FloorOptional& LocationDetailType::Floor()
  {
    return Floor_;
  }

  // LocationContextState
  //
  const LocationContextState::LocationDetailOptional& LocationContextState::LocationDetail() const
  {
    return LocationDetail_;
  }
  LocationContextState::LocationDetailOptional& LocationContextState::LocationDetail()
  {
    return LocationDetail_;
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
  // AbstractOperationState
  //
  AbstractOperationState::AbstractOperationState(const StateKind kind,
                                                 const DescriptorHandleType& descriptorHandle,
                                                 const OperatingModeType& operatingMode)
    : AbstractState(kind, descriptorHandle)
    , OperatingMode_(operatingMode)
  {
  }
  bool AbstractOperationState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::OPERATION_STATE &&
           other->getKind() < StateKind::LAST_OPERATION_STATE;
  }
  const AbstractOperationState::OperatingModeType& AbstractOperationState::OperatingMode() const
  {
    return OperatingMode_;
  }
  AbstractOperationState::OperatingModeType& AbstractOperationState::OperatingMode()
  {
    return OperatingMode_;
  }

  // SetValueOperationState
  //
  SetValueOperationState::SetValueOperationState(const DescriptorHandleType& descriptorHandle,
                                                 const OperatingModeType& operatingMode)
    : AbstractOperationState(StateKind::SET_VALUE_OPERATION_STATE, descriptorHandle, operatingMode)
  {
  }
  bool SetValueOperationState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::SET_VALUE_OPERATION_STATE;
  }

  // AbstractMetricState
  //
  AbstractMetricState::AbstractMetricState(const StateKind kind, DescriptorHandleType handle)
    : AbstractState(kind, std::move(handle))
  {
  }
  bool AbstractMetricState::classof(const AbstractState* other)
  {
    return other->getKind() >= StateKind::METRIC_STATE &&
           other->getKind() < StateKind::LAST_METRIC_STATE;
  }

  // NumericMetricState
  //
  NumericMetricState::NumericMetricState(DescriptorHandleType handle)
    : AbstractMetricState(StateKind::NUMERIC_METRIC_STATE, std::move(handle))
  {
  }
  bool NumericMetricState::classof(const AbstractState* other)
  {
    return other->getKind() == StateKind::NUMERIC_METRIC_STATE;
  }
  const NumericMetricState::MetricValueOptional& NumericMetricState::MetricValue() const
  {
    return MetricValue_;
  }
  NumericMetricState::MetricValueOptional& NumericMetricState::MetricValue()
  {
    return MetricValue_;
  }

  // MdState
  //
  const MdState::StateSequence& MdState::State() const
  {
    return State_;
  }
  MdState::StateSequence& MdState::State()
  {
    return State_;
  }

  // Mdib
  //
  Mdib::Mdib(SequenceIdType sequenceIdType)
    : SequenceId_(std::move(sequenceIdType))
  {
  }
  const Mdib::MdDescriptionOptional& Mdib::MdDescription() const
  {
    return MdDescription_;
  }
  Mdib::MdDescriptionOptional& Mdib::MdDescription()
  {
    return MdDescription_;
  }
  const Mdib::MdStateOptional& Mdib::MdState() const
  {
    return MdState_;
  }
  Mdib::MdStateOptional& Mdib::MdState()
  {
    return MdState_;
  }
  const Mdib::MdibVersionOptional& Mdib::MdibVersion() const
  {
    return MdibVersion_;
  }
  Mdib::MdibVersionOptional& Mdib::MdibVersion()
  {
    return MdibVersion_;
  }
  const Mdib::SequenceIdType& Mdib::SequenceId() const
  {
    return SequenceId_;
  }
  Mdib::SequenceIdType& Mdib::SequenceId()
  {
    return SequenceId_;
  }
  const Mdib::InstanceIdOptional& Mdib::InstanceId() const
  {
    return InstanceId_;
  }
  Mdib::InstanceIdOptional& Mdib::InstanceId()
  {
    return InstanceId_;
  }
} // namespace BICEPS::PM
