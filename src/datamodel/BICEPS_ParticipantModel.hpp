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
    CodeType Code;

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
    TypeOptional Type;

    using HandleType = std::string;
    HandleType Handle;

    using DescriptorVersionType = unsigned int;
    using DescriptorVersionOptional = std::optional<DescriptorVersionType>;
    DescriptorVersionOptional DescriptorVersion;

    using SafetyClassificationType = ::BICEPS::PM::SafetyClassification;
    using SafetyClassificationOptional = std::optional<SafetyClassificationType>;
    SafetyClassificationOptional SafetyClassification;

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
    OperationTargetType OperationTarget;

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
    ManufacturerSequence Manufacturer;

    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    ModelNameSequence ModelName;

    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;
    ModelNumberOptional ModelNumber;

    using SerialNumberType = std::string;
    using SerialNumberSequence = std::vector<SerialNumberType>;
    SerialNumberSequence SerialNumber;
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
    PatientContextOptional PatientContext;

    using LocationContextType = LocationContextDescriptor;
    using LocationContextOptional = std::optional<LocationContextType>;
    LocationContextOptional LocationContext;

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
    LowerOptional Lower;

    using UpperType = int;
    using UpperOptional = std::optional<UpperType>;
    UpperOptional Upper;

    using StepWidthType = int;
    using StepWidthOptional = std::optional<StepWidthType>;
    StepWidthOptional StepWidth;

    using RelativeAccuracyType = int;
    using RelativeAccuracyOptional = std::optional<RelativeAccuracyType>;
    RelativeAccuracyOptional RelativeAccuracy;

    using AbsoluteAccuracyType = int;
    using AbsoluteAccuracyOptional = std::optional<AbsoluteAccuracyType>;
    AbsoluteAccuracyOptional AbsoluteAccuracy;
  };

  struct AbstractMetricDescriptor : public AbstractDescriptor
  {
    using UnitType = CodedValue;
    UnitType Unit;

    using MetricCategoryType = ::BICEPS::PM::MetricCategory;
    MetricCategoryType MetricCategory;

    using MetricAvailabilityType = ::BICEPS::PM::MetricAvailability;
    MetricAvailabilityType MetricAvailability;

    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractMetricDescriptor(DescriptorKind kind, const HandleType&, UnitType,
                             const MetricCategoryType&, const MetricAvailabilityType&);
  };

  struct NumericMetricDescriptor : public AbstractMetricDescriptor
  {
    using TechnicalRangeType = Range;
    using TechnicalRangeSequence = std::vector<TechnicalRangeType>;
    TechnicalRangeSequence TechnicalRange;

    using ResolutionType = int;
    ResolutionType Resolution;

    using AveragingPeriodType = std::string;
    using AveragingPeriodOptional = std::optional<AveragingPeriodType>;
    AveragingPeriodOptional AveragingPeriod;

    static bool classof(const AbstractDescriptor* other);

    NumericMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                            const MetricAvailabilityType&, const ResolutionType&);
  };

  struct ChannelDescriptor : public AbstractDeviceComponentDescriptor
  {
    using MetricType = std::shared_ptr<AbstractMetricDescriptor>;
    using MetricSequence = std::vector<MetricType>;
    MetricSequence Metric;

    static bool classof(const AbstractDescriptor* other);

    explicit ChannelDescriptor(const HandleType&);
  };

  struct ScoDescriptor : public AbstractDeviceComponentDescriptor
  {
    using OperationType = ::BICEPS::PM::AbstractOperationDescriptor;
    using OperationSequence = std::vector<std::shared_ptr<OperationType>>;
    OperationSequence Operation;

    static bool classof(const AbstractDescriptor* other);

    explicit ScoDescriptor(const HandleType& handle);
  };

  struct VmdDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
    using ChannelType = ChannelDescriptor;
    using ChannelSequence = std::vector<ChannelType>;
    ChannelSequence Channel;

    using ScoType = ::BICEPS::PM::ScoDescriptor;
    using ScoOptional = std::optional<ScoType>;
    ScoOptional Sco;

    static bool classof(const AbstractDescriptor* other);

    explicit VmdDescriptor(const HandleType&);
  };

  struct MdsDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
    using MetaDataType = Metadata;
    using MetaDataOptional = std::optional<MetaDataType>;
    MetaDataOptional MetaData;

    using SystemContextType = SystemContextDescriptor;
    using SystemContextOptional = std::optional<SystemContextType>;
    SystemContextOptional SystemContext;

    using ClockType = ClockDescriptor;
    using ClockOptional = std::optional<ClockType>;
    ClockOptional Clock;

    using BatteryType = BatteryDescriptor;
    using BatterySequence = std::vector<BatteryType>;
    BatterySequence Battery;

    using VmdType = VmdDescriptor;
    using VmdSequence = std::vector<VmdType>;
    VmdSequence Vmd;

    static bool classof(const AbstractDescriptor* other);

    explicit MdsDescriptor(const HandleType&);
  };

  struct MdDescription
  {
    using MdsType = MdsDescriptor;
    using MdsSequence = std::vector<MdsType>;
    MdsSequence Mds;

    using DescriptionVersionType = unsigned int;
    using DescriptionVersionOptional = std::optional<DescriptionVersionType>;
    DescriptionVersionOptional DescriptionVersion;
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
    StateVersionOptional StateVersion;

    using DescriptorHandleType = std::string;
    DescriptorHandleType DescriptorHandle;

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
    CategoryOptional Category;

    using HandleType = std::string;
    HandleType Handle;

    static bool classof(const AbstractState* other);

    AbstractMultiState(StateKind kind, const DescriptorHandleType&, HandleType);
  };

  struct InstanceIdentifier
  {
    using ExtensionType = std::string;
    using ExtensionOptional = std::optional<ExtensionType>;
    ExtensionOptional Extension;

    using RootType = WS::ADDRESSING::URIType;
    using RootOptional = std::optional<RootType>;
    RootOptional Root;
  };

  struct AbstractContextState : public AbstractMultiState
  {
    using BindingMdibVersionType = unsigned int;
    using BindingMdibVersionOptional = std::optional<BindingMdibVersionType>;
    BindingMdibVersionOptional BindingMdibVersion;

    using ContextAssociationType = ::BICEPS::PM::ContextAssociation;
    using ContextAssociationOptional = std::optional<ContextAssociationType>;
    ContextAssociationOptional ContextAssociation;

    using ValidatorType = InstanceIdentifier;
    using ValidatorSequence = std::vector<ValidatorType>;
    ValidatorSequence Validator;

    using IdentificationType = InstanceIdentifier;
    using IdentificationSequence = std::vector<IdentificationType>;
    IdentificationSequence Identification;

    static bool classof(const AbstractState* other);

    AbstractContextState(StateKind kind, const DescriptorHandleType&, const HandleType&);
  };

  struct LocationDetailType
  {
    using PoCType = std::string;
    using PoCOptional = std::optional<PoCType>;
    PoCOptional PoC;

    using RoomType = std::string;
    using RoomOptional = std::optional<RoomType>;
    RoomOptional Room;

    using BedType = std::string;
    using BedOptional = std::optional<PoCType>;
    BedOptional Bed;

    using FacilityType = std::string;
    using FacilityOptional = std::optional<FacilityType>;
    FacilityOptional Facility;

    using BuildingType = std::string;
    using BuildingOptional = std::optional<PoCType>;
    BuildingOptional Building;

    using FloorType = std::string;
    using FloorOptional = std::optional<PoCType>;
    FloorOptional Floor;
  };

  struct LocationContextState : public AbstractContextState
  {
    using LocationDetailOptional = std::optional<LocationDetailType>;
    LocationDetailOptional LocationDetail;

    static bool classof(const AbstractState* other);

    LocationContextState(const DescriptorHandleType&, const HandleType&);
  };

  struct AbstractOperationState : public AbstractState
  {
    using OperatingModeType = ::BICEPS::PM::OperatingMode;
    OperatingModeType OperatingMode;

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

  struct MetricQualityType
  {
    using ValidityType = MeasurementValidity;
    ValidityType Validity;

    using ModeType = GenerationMode;
    using ModeOptional = std::optional<ModeType>;
    ModeOptional Mode;

    using QiType = int;
    using QiOptional = std::optional<QiType>;
    QiOptional Qi;

    explicit MetricQualityType(const ValidityType& validity);
  };

  struct AnnotationType
  {
    using TypeType = CodedValue;
    TypeType Type;
  };

  using Timestamp = unsigned int;

  struct AbstractMetricValue
  {
    enum class MetricKind
    {
      NUMERIC_METRIC,
    };
    MetricKind getKind() const;

    MetricQualityType MetricQuality;

    using AnnotationSequence = std::vector<AnnotationType>;
    AnnotationSequence Annotation;

    using StartTimeType = Timestamp;
    using StartTimeOptional = std::optional<StartTimeType>;
    StartTimeOptional StartTime;

    using StopTimeType = Timestamp;
    using StopTimeOptional = std::optional<StopTimeType>;
    StopTimeOptional StopTime;

    using DeterminationTimeType = Timestamp;
    using DeterminationTimeOptional = std::optional<DeterminationTimeType>;
    DeterminationTimeOptional DeterminationTime;

    virtual ~AbstractMetricValue() = default;

  protected:
    explicit AbstractMetricValue(MetricKind kind, const MetricQualityType& metricQuality);
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
    ValueOptional Value;

    static bool classof(const AbstractMetricValue* other);

    explicit NumericMetricValue(const MetricQualityType& metricQuality);
  };

  struct AbstractMetricState : public AbstractState
  {
    using ActivationStateType = ComponentActivation;
    using ActivationStateOptional = std::optional<ActivationStateType>;
    ActivationStateOptional ActivationState;

    static bool classof(const AbstractState* other);

  protected:
    AbstractMetricState(StateKind kind, DescriptorHandleType handle);
  };

  struct NumericMetricState : public AbstractMetricState
  {
    using MetricValueType = NumericMetricValue;
    using MetricValueOptional = std::optional<MetricValueType>;
    MetricValueOptional MetricValue;

    using PhysiologicalRangeType = Range;
    using PhysiologicalRangeSequence = std::vector<PhysiologicalRangeType>;
    PhysiologicalRangeSequence PhysiologicalRange;

    using ActiveAveragingPeriodType = std::string;
    using ActiveAveragingPeriodOptional = std::optional<ActiveAveragingPeriodType>;
    ActiveAveragingPeriodOptional ActiveAveragingPeriod;

    static bool classof(const AbstractState* other);

    explicit NumericMetricState(DescriptorHandleType handle);
  };

  struct MdState
  {
    using StateType = std::shared_ptr<AbstractState>;
    using StateSequence = std::vector<StateType>;
    StateSequence State;

    using StateVersionType = unsigned int;
    using StateVersionOptional = std::optional<StateVersionType>;
    StateVersionOptional StateVersion;
  };

  struct Mdib
  {
    using MdDescriptionType = ::BICEPS::PM::MdDescription;
    using MdDescriptionOptional = std::optional<MdDescriptionType>;
    MdDescriptionOptional MdDescription;

    using MdStateType = ::BICEPS::PM::MdState;
    using MdStateOptional = std::optional<MdStateType>;
    MdStateOptional MdState;

    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    MdibVersionOptional MdibVersion;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType SequenceId;

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    InstanceIdOptional InstanceId;

    explicit Mdib(SequenceIdType sequenceIdType);
  };
} // namespace BICEPS::PM
