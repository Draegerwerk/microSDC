#pragma once

#include "ws-addressing.hpp"
#include <optional>
#include <string>
#include <vector>

namespace BICEPS::PM
{
  enum class SafetyClassification
  {
    Inf,
    MedA,
    MedB,
    MedC
  };
  enum class MetricCategory
  {
    Unspec,
    Msrmt,
    Clc,
    Set,
    Preset,
    Rcmm
  };
  enum class MetricAvailability
  {
    Intr,
    Cont
  };
  enum class MeasurementValidity
  {
    Vld,
    Vldated,
    Ong,
    Qst,
    Calib,
    Inv,
    Oflw,
    Uflw,
    NA
  };
  enum class ContextAssociation
  {
    No,
    Pre,
    Assoc,
    Dis
  };
  enum class GenerationMode
  {
    Real,
    Test,
    Demo
  };
  enum class ComponentActivation
  {
    On,
    NotRdy,
    StndBy,
    Off,
    Shtdn,
    Fail
  };
  enum class OperatingMode
  {
    Dis,
    En,
    NA
  };

  struct CodedValue
  {
    using CodeType = std::string;
    CodeType code;

    explicit CodedValue(CodeType code);
  };

  struct AbstractDescriptor
  {
    enum class DescriptorKind
    {
      METRIC_DESCRIPTOR,
      NUMERIC_METRIC_DESCRIPTOR,
      LAST_METRIC_DESCRIPTOR,

      OPERATION_DESCRIPTOR,
      SET_VALUE_OPERATION_DESCRIPTOR,
      LAST_OPERATION_DESCRIPTOR,

      DEVICE_COMPONENT_DESCRIPTOR,
      COMPLEX_DEVICE_COMPONENT_DESCRIPTOR,
      VMD_DESCRIPTOR,
      MDS_DESCRIPTOR,
      LAST_COMPLEX_DEVICE_COMPONENT_DESCRIPTOR,
      CHANNEL_DESCRIPTOR,
      SCO_DESCRIPTOR,
      LAST_DEVICE_COMPONENT_DESCRIPTOR,

      CONTEXT_DESCRIPTOR,
      PATIENT_CONTEXT_DESCRIPTOR,
      LOCATION_CONTEXT_DESCRIPTOR,
      SYSTEM_CONTEXT_DESCRIPTOR,
      LAST_CONTEXT_DESCRIPTOR,
    };
    DescriptorKind getKind() const;

    using TypeType = CodedValue;
    using TypeOptional = std::optional<TypeType>;
    TypeOptional type;

    using HandleType = std::string;
    HandleType handle;

    using DescriptorVersionType = unsigned int;
    using DescriptorVersionOptional = std::optional<DescriptorVersionType>;
    DescriptorVersionOptional descriptorVersion;

    using SafetyClassificationType = ::BICEPS::PM::SafetyClassification;
    using SafetyClassificationOptional = std::optional<SafetyClassificationType>;
    SafetyClassificationOptional safetyClassification;

    virtual ~AbstractDescriptor() = default;

  protected:
    AbstractDescriptor(DescriptorKind kind, HandleType);
    AbstractDescriptor(const AbstractDescriptor&) = default;
    AbstractDescriptor(AbstractDescriptor&&) = default;
    AbstractDescriptor& operator=(const AbstractDescriptor&) = default;
    AbstractDescriptor& operator=(AbstractDescriptor&&) = default;

  private:
    DescriptorKind kind_;
  };

  struct AbstractOperationDescriptor : public AbstractDescriptor
  {
    using OperationTargetType = std::string;
    OperationTargetType operationTarget;

    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractOperationDescriptor(DescriptorKind kind, const HandleType& handle,
                                OperationTargetType operationTarget);
  };

  struct SetValueOperationDescriptor : public AbstractOperationDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    SetValueOperationDescriptor(const HandleType& handle,
                                const OperationTargetType& operationTarget);
  };

  struct AbstractDeviceComponentDescriptor : public AbstractDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractDeviceComponentDescriptor(DescriptorKind kind, const HandleType&);
  };

  struct AbstractComplexDeviceComponentDescriptor : public AbstractDeviceComponentDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractComplexDeviceComponentDescriptor(DescriptorKind kind, const HandleType&);
  };

  struct Metadata
  {
  public:
    using ManufacturerType = std::string;
    using ManufacturerSequence = std::vector<ManufacturerType>;
    ManufacturerSequence manufacturer;

    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    ModelNameSequence modelName;

    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;
    ModelNumberOptional modelNumber;

    using SerialNumberType = std::string;
    using SerialNumberSequence = std::vector<SerialNumberType>;
    SerialNumberSequence serialNumber;
  };

  struct AbstractContextDescriptor : public AbstractDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractContextDescriptor(DescriptorKind kind, const HandleType&);
  };

  struct PatientContextDescriptor : public AbstractContextDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    explicit PatientContextDescriptor(const HandleType&);
  };

  struct LocationContextDescriptor : public AbstractContextDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    explicit LocationContextDescriptor(const HandleType&);
  };

  struct SystemContextDescriptor : public AbstractContextDescriptor
  {
    using PatientContextType = PatientContextDescriptor;
    using PatientContextOptional = std::optional<PatientContextType>;
    PatientContextOptional patientContext;

    using LocationContextType = LocationContextDescriptor;
    using LocationContextOptional = std::optional<LocationContextType>;
    LocationContextOptional locationContext;

    static bool classof(const AbstractDescriptor* other);

    explicit SystemContextDescriptor(const HandleType&);
  };

  struct ClockDescriptor
  {
  };

  struct BatteryDescriptor
  {
  };

  struct Range
  {
    using LowerType = int;
    using LowerOptional = std::optional<LowerType>;
    LowerOptional lower;

    using UpperType = int;
    using UpperOptional = std::optional<UpperType>;
    UpperOptional upper;

    using StepWidthType = int;
    using StepWidthOptional = std::optional<StepWidthType>;
    StepWidthOptional stepWidth;

    using RelativeAccuracyType = int;
    using RelativeAccuracyOptional = std::optional<RelativeAccuracyType>;
    RelativeAccuracyOptional relativeAccuracy;

    using AbsoluteAccuracyType = int;
    using AbsoluteAccuracyOptional = std::optional<AbsoluteAccuracyType>;
    AbsoluteAccuracyOptional absoluteAccuracy;
  };

  struct AbstractMetricDescriptor : public AbstractDescriptor
  {
    using UnitType = CodedValue;
    UnitType unit;

    using MetricCategoryType = ::BICEPS::PM::MetricCategory;
    MetricCategoryType metricCategory;

    using MetricAvailabilityType = ::BICEPS::PM::MetricAvailability;
    MetricAvailabilityType metricAvailability;

    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractMetricDescriptor(DescriptorKind kind, const HandleType&, UnitType,
                             const MetricCategoryType&, const MetricAvailabilityType&);
  };

  struct NumericMetricDescriptor : public AbstractMetricDescriptor
  {
    using TechnicalRangeType = Range;
    using TechnicalRangeSequence = std::vector<TechnicalRangeType>;
    TechnicalRangeSequence technicalRange;

    using ResolutionType = int;
    ResolutionType resolution;

    using AveragingPeriodType = std::string;
    using AveragingPeriodOptional = std::optional<AveragingPeriodType>;
    AveragingPeriodOptional averagingPeriod;

    static bool classof(const AbstractDescriptor* other);

    NumericMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                            const MetricAvailabilityType&, const ResolutionType&);
  };

  struct ChannelDescriptor : public AbstractDeviceComponentDescriptor
  {
    using MetricType = std::shared_ptr<AbstractMetricDescriptor>;
    using MetricSequence = std::vector<MetricType>;
    MetricSequence metric;

    static bool classof(const AbstractDescriptor* other);

    explicit ChannelDescriptor(const HandleType&);
  };

  struct ScoDescriptor : public AbstractDeviceComponentDescriptor
  {
    using OperationType = ::BICEPS::PM::AbstractOperationDescriptor;
    using OperationSequence = std::vector<std::shared_ptr<OperationType>>;
    OperationSequence operation;

    static bool classof(const AbstractDescriptor* other);

    explicit ScoDescriptor(const HandleType& handle);
  };

  struct VmdDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
    using ChannelType = ChannelDescriptor;
    using ChannelSequence = std::vector<ChannelType>;
    ChannelSequence channel;

    using ScoType = ::BICEPS::PM::ScoDescriptor;
    using ScoOptional = std::optional<ScoType>;
    ScoOptional sco;

    static bool classof(const AbstractDescriptor* other);

    explicit VmdDescriptor(const HandleType&);
  };

  struct MdsDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
    using MetaDataType = Metadata;
    using MetaDataOptional = std::optional<MetaDataType>;
    MetaDataOptional metaData;

    using SystemContextType = SystemContextDescriptor;
    using SystemContextOptional = std::optional<SystemContextType>;
    SystemContextOptional systemContext;

    using ClockType = ClockDescriptor;
    using ClockOptional = std::optional<ClockType>;
    ClockOptional clock;

    using BatteryType = BatteryDescriptor;
    using BatterySequence = std::vector<BatteryType>;
    BatterySequence battery;

    using VmdType = VmdDescriptor;
    using VmdSequence = std::vector<VmdType>;
    VmdSequence vmd;

    static bool classof(const AbstractDescriptor* other);

    explicit MdsDescriptor(const HandleType&);
  };

  struct MdDescription
  {
    using MdsType = MdsDescriptor;
    using MdsSequence = std::vector<MdsType>;
    MdsSequence mds;

    using DescriptionVersionType = unsigned int;
    using DescriptionVersionOptional = std::optional<DescriptionVersionType>;
    DescriptionVersionOptional descriptionVersion;
  };

  struct AbstractState
  {
    enum class StateKind
    {
      MULTI_STATE,
      CONTEXT_STATE,
      LOCATION_CONTEXT_STATE,
      LAST_CONTEXT_STATE,
      LAST_MULTI_STATE,

      OPERATION_STATE,
      SET_VALUE_OPERATION_STATE,
      LAST_OPERATION_STATE,

      METRIC_STATE,
      NUMERIC_METRIC_STATE,
      LAST_METRIC_STATE,
    };
    StateKind getKind() const;

    using StateVersionType = unsigned int;
    using StateVersionOptional = std::optional<StateVersionType>;
    StateVersionOptional stateVersion;

    using DescriptorHandleType = std::string;
    DescriptorHandleType descriptorHandle;

    virtual ~AbstractState() = default;

  protected:
    AbstractState(StateKind kind, DescriptorHandleType);
    AbstractState(const AbstractState&) = default;
    AbstractState(AbstractState&&) = default;
    AbstractState& operator=(const AbstractState&) = default;
    AbstractState& operator=(AbstractState&&) = default;

  private:
    StateKind kind_;
  };

  struct AbstractMultiState : public AbstractState
  {
    using CategoryType = CodedValue;
    using CategoryOptional = std::optional<CategoryType>;
    CategoryOptional category;

    using HandleType = std::string;
    HandleType handle;

    static bool classof(const AbstractState* other);

    AbstractMultiState(StateKind kind, const DescriptorHandleType&, HandleType);
  };

  struct InstanceIdentifier
  {
    using ExtensionType = std::string;
    using ExtensionOptional = std::optional<ExtensionType>;
    ExtensionOptional extension;

    using RootType = WS::ADDRESSING::URIType;
    using RootOptional = std::optional<RootType>;
    RootOptional root;
  };

  struct AbstractContextState : public AbstractMultiState
  {
    using BindingMdibVersionType = unsigned int;
    using BindingMdibVersionOptional = std::optional<BindingMdibVersionType>;
    BindingMdibVersionOptional bindingMdibVersion;

    using ContextAssociationType = ::BICEPS::PM::ContextAssociation;
    using ContextAssociationOptional = std::optional<ContextAssociationType>;
    ContextAssociationOptional contextAssociation;

    using ValidatorType = InstanceIdentifier;
    using ValidatorSequence = std::vector<ValidatorType>;
    ValidatorSequence validator;

    using IdentificationType = InstanceIdentifier;
    using IdentificationSequence = std::vector<IdentificationType>;
    IdentificationSequence identification;

    static bool classof(const AbstractState* other);

    AbstractContextState(StateKind kind, const DescriptorHandleType&, const HandleType&);
  };

  struct LocationDetail
  {
    using PoCType = std::string;
    using PoCOptional = std::optional<PoCType>;
    PoCOptional poC;

    using RoomType = std::string;
    using RoomOptional = std::optional<RoomType>;
    RoomOptional room;

    using BedType = std::string;
    using BedOptional = std::optional<PoCType>;
    BedOptional bed;

    using FacilityType = std::string;
    using FacilityOptional = std::optional<FacilityType>;
    FacilityOptional facility;

    using BuildingType = std::string;
    using BuildingOptional = std::optional<PoCType>;
    BuildingOptional building;

    using FloorType = std::string;
    using FloorOptional = std::optional<PoCType>;
    FloorOptional floor;
  };

  struct LocationContextState : public AbstractContextState
  {
    using LocationDetailOptional = std::optional<LocationDetail>;
    LocationDetailOptional locationDetail;

    static bool classof(const AbstractState* other);

    LocationContextState(const DescriptorHandleType&, const HandleType&);
  };

  struct AbstractOperationState : public AbstractState
  {
    using OperatingModeType = ::BICEPS::PM::OperatingMode;
    OperatingModeType operatingMode;

    static bool classof(const AbstractState* other);

  protected:
    AbstractOperationState(StateKind kind, const DescriptorHandleType& descriptorHandle,
                           const OperatingModeType& operatingMode);
  };

  struct SetValueOperationState : public AbstractOperationState
  {
    static bool classof(const AbstractState* other);

    SetValueOperationState(const DescriptorHandleType& descriptorHandle,
                           const OperatingModeType& operatingMode);
  };

  struct MetricQuality
  {
    using ValidityType = MeasurementValidity;
    ValidityType validity;

    using ModeType = GenerationMode;
    using ModeOptional = std::optional<ModeType>;
    ModeOptional mode;

    using QiType = int;
    using QiOptional = std::optional<QiType>;
    QiOptional qi;

    explicit MetricQuality(const ValidityType& validity);
  };

  struct Annotation
  {
    using TypeType = CodedValue;
    TypeType type;
  };

  using Timestamp = unsigned int;

  struct AbstractMetricValue
  {
    enum class MetricKind
    {
      NUMERIC_METRIC,
    };
    MetricKind getKind() const;

    MetricQuality metricQuality;

    using AnnotationSequence = std::vector<Annotation>;
    AnnotationSequence annotation;

    using StartTimeType = Timestamp;
    using StartTimeOptional = std::optional<StartTimeType>;
    StartTimeOptional startTime;

    using StopTimeType = Timestamp;
    using StopTimeOptional = std::optional<StopTimeType>;
    StopTimeOptional stopTime;

    using DeterminationTimeType = Timestamp;
    using DeterminationTimeOptional = std::optional<DeterminationTimeType>;
    DeterminationTimeOptional determinationTime;

    virtual ~AbstractMetricValue() = default;

  protected:
    explicit AbstractMetricValue(MetricKind kind, const MetricQuality& metricQuality);
    AbstractMetricValue(const AbstractMetricValue&) = default;
    AbstractMetricValue(AbstractMetricValue&&) = default;
    AbstractMetricValue& operator=(const AbstractMetricValue&) = default;
    AbstractMetricValue& operator=(AbstractMetricValue&&) = default;

  private:
    MetricKind kind_;
  };

  struct NumericMetricValue : public AbstractMetricValue
  {
    using ValueType = double;
    using ValueOptional = std::optional<ValueType>;
    ValueOptional value;

    static bool classof(const AbstractMetricValue* other);

    explicit NumericMetricValue(const MetricQuality& metricQuality);
  };

  struct AbstractMetricState : public AbstractState
  {
    using ActivationStateType = ComponentActivation;
    using ActivationStateOptional = std::optional<ActivationStateType>;
    ActivationStateOptional activationState;

    static bool classof(const AbstractState* other);

  protected:
    AbstractMetricState(StateKind kind, DescriptorHandleType handle);
  };

  struct NumericMetricState : public AbstractMetricState
  {
    using MetricValueType = NumericMetricValue;
    using MetricValueOptional = std::optional<MetricValueType>;
    MetricValueOptional metricValue;

    using PhysiologicalRangeType = Range;
    using PhysiologicalRangeSequence = std::vector<PhysiologicalRangeType>;
    PhysiologicalRangeSequence physiologicalRange;

    using ActiveAveragingPeriodType = std::string;
    using ActiveAveragingPeriodOptional = std::optional<ActiveAveragingPeriodType>;
    ActiveAveragingPeriodOptional activeAveragingPeriod;

    static bool classof(const AbstractState* other);

    explicit NumericMetricState(DescriptorHandleType handle);
  };

  struct MdState
  {
    using StateType = std::shared_ptr<AbstractState>;
    using StateSequence = std::vector<StateType>;
    StateSequence state;

    using StateVersionType = unsigned int;
    using StateVersionOptional = std::optional<StateVersionType>;
    StateVersionOptional stateVersion;
  };

  struct Mdib
  {
    using MdDescriptionType = ::BICEPS::PM::MdDescription;
    using MdDescriptionOptional = std::optional<MdDescriptionType>;
    MdDescriptionOptional mdDescription;

    using MdStateType = ::BICEPS::PM::MdState;
    using MdStateOptional = std::optional<MdStateType>;
    MdStateOptional mdState;

    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    MdibVersionOptional mdibVersion;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType sequenceId;

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    InstanceIdOptional instanceId;

    explicit Mdib(SequenceIdType sequenceIdType);
  };
} // namespace BICEPS::PM
