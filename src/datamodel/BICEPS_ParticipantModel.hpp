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

  class CodedValue
  {
  public:
    using CodeType = std::string;
    const CodeType& Code() const;
    CodeType& Code();

    explicit CodedValue(CodeType code);

  private:
    CodeType Code_;
  };
  class AbstractDescriptor
  {
  public:
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
    // Type
    //
    using TypeType = CodedValue;
    using TypeOptional = std::optional<TypeType>;
    const TypeOptional& Type() const;
    TypeOptional& Type();

    // Handle
    //
    using HandleType = std::string;
    const HandleType& Handle() const;
    HandleType& Handle();

    // DescriptorVersion
    //
    using DescriptorVersionType = unsigned int;
    using DescriptorVersionOptional = std::optional<DescriptorVersionType>;
    const DescriptorVersionOptional& DescriptorVersion() const;
    DescriptorVersionOptional& DescriptorVersion();

    // SafetyClassification
    //
    using SafetyClassificationType = ::BICEPS::PM::SafetyClassification;
    using SafetyClassificationOptional = std::optional<SafetyClassificationType>;
    const SafetyClassificationOptional& SafetyClassification() const;
    SafetyClassificationOptional& SafetyClassification();

    DescriptorKind getKind() const;

  protected:
    // Constructors.
    //
    AbstractDescriptor(DescriptorKind kind, HandleType);

  private:
    DescriptorKind kind_;
    TypeOptional Type_;
    HandleType Handle_;
    DescriptorVersionOptional DescriptorVersion_;
    SafetyClassificationOptional SafetyClassification_;
  };

  class AbstractOperationDescriptor : public AbstractDescriptor
  {
  public:
    using OperationTargetType = std::string;
    const OperationTargetType& OperationTarget() const;
    OperationTargetType& OperationTarget();

    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractOperationDescriptor(DescriptorKind kind, const HandleType& handle,
                                OperationTargetType operationTarget);

  private:
    OperationTargetType OperationTarget_;
  };

  class SetValueOperationDescriptor : public AbstractOperationDescriptor
  {
  public:
    SetValueOperationDescriptor(const HandleType& handle,
                                const OperationTargetType& operationTarget);
    static bool classof(const AbstractDescriptor* other);
  };
  class AbstractDeviceComponentDescriptor : public AbstractDescriptor
  {
  public:
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractDeviceComponentDescriptor(DescriptorKind kind, const HandleType&);
  };
  class AbstractComplexDeviceComponentDescriptor : public AbstractDeviceComponentDescriptor
  {
  public:
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractComplexDeviceComponentDescriptor(DescriptorKind kind, const HandleType&);
  };
  class Metadata
  {
  public:
    // Manufacturer
    //
    using ManufacturerType = std::string;
    using ManufacturerSequence = std::vector<ManufacturerType>;
    const ManufacturerSequence& Manufacturer() const;
    ManufacturerSequence& Manufacturer();

    // ModelName
    //
    using ModelNameType = std::string;
    using ModelNameSequence = std::vector<ModelNameType>;
    const ModelNameSequence& ModelName() const;
    ModelNameSequence& ModelName();

    // ModelNumber
    //
    using ModelNumberType = std::string;
    using ModelNumberOptional = std::optional<ModelNumberType>;
    const ModelNumberOptional& ModelNumber() const;
    ModelNumberOptional& ModelNumber();

    // SerialNumber
    //
    using SerialNumberType = std::string;
    using SerialNumberSequence = std::vector<SerialNumberType>;
    const SerialNumberSequence& SerialNumber() const;
    SerialNumberSequence& SerialNumber();

  private:
    ManufacturerSequence Manufacturer_;
    ModelNameSequence ModelName_;
    ModelNumberOptional ModelNumber_;
    SerialNumberSequence SerialNumber_;
  };
  class AbstractContextDescriptor : public AbstractDescriptor
  {
  public:
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractContextDescriptor(DescriptorKind kind, const HandleType&);
  };
  class PatientContextDescriptor : public AbstractContextDescriptor
  {
  public:
    // Constructors
    //
    explicit PatientContextDescriptor(const HandleType&);
    static bool classof(const AbstractDescriptor* other);
  };
  class LocationContextDescriptor : public AbstractContextDescriptor
  {
  public:
    // Constructors
    //
    explicit LocationContextDescriptor(const HandleType&);
    static bool classof(const AbstractDescriptor* other);
  };
  class SystemContextDescriptor : public AbstractContextDescriptor
  {
  public:
    // PatientContext
    //
    using PatientContextType = PatientContextDescriptor;
    using PatientContextOptional = std::optional<PatientContextType>;
    const PatientContextOptional& PatientContext() const;
    PatientContextOptional& PatientContext();

    // LocationContext
    //
    using LocationContextType = LocationContextDescriptor;
    using LocationContextOptional = std::optional<LocationContextType>;
    const LocationContextOptional& LocationContext() const;
    LocationContextOptional& LocationContext();

    // Constructors
    //
    explicit SystemContextDescriptor(const HandleType&);
    static bool classof(const AbstractDescriptor* other);

  private:
    PatientContextOptional PatientContext_;
    LocationContextOptional LocationContext_;
  };
  class ClockDescriptor
  {
  };
  class BatteryDescriptor
  {
  };
  class Range
  {
  public:
    using LowerType = int;
    using LowerOptional = std::optional<LowerType>;
    const LowerOptional& Lower() const;
    LowerOptional& Lower();

    using UpperType = int;
    using UpperOptional = std::optional<UpperType>;
    const UpperOptional& Upper() const;
    UpperOptional& Upper();

    using StepWidthType = int;
    using StepWidthOptional = std::optional<StepWidthType>;
    const StepWidthOptional& StepWidth() const;
    StepWidthOptional& StepWidth();

    using RelativeAccuracyType = int;
    using RelativeAccuracyOptional = std::optional<RelativeAccuracyType>;
    const RelativeAccuracyOptional& RelativeAccuracy() const;
    RelativeAccuracyOptional& RelativeAccuracy();

    using AbsoluteAccuracyType = int;
    using AbsoluteAccuracyOptional = std::optional<AbsoluteAccuracyType>;
    const AbsoluteAccuracyOptional& AbsoluteAccuracy() const;
    AbsoluteAccuracyOptional& AbsoluteAccuracy();

  private:
    LowerOptional Lower_;
    UpperOptional Upper_;
    StepWidthOptional StepWidth_;
    RelativeAccuracyOptional RelativeAccuracy_;
    AbsoluteAccuracyOptional AbsoluteAccuracy_;
  };
  class AbstractMetricDescriptor : public AbstractDescriptor
  {
  public:
    // Unit
    //
    using UnitType = CodedValue;
    const UnitType& Unit() const;
    UnitType& Unit();

    // MetricCategory
    //
    using MetricCategoryType = ::BICEPS::PM::MetricCategory;
    const MetricCategoryType& MetricCategory() const;
    MetricCategoryType& MetricCategory();

    // MetricAvailability
    //
    using MetricAvailabilityType = ::BICEPS::PM::MetricAvailability;
    const MetricAvailabilityType& MetricAvailability() const;
    MetricAvailabilityType& MetricAvailability();

    static bool classof(const AbstractDescriptor* other);

    AbstractMetricDescriptor(const AbstractMetricDescriptor&) = delete;
    AbstractMetricDescriptor& operator=(const AbstractMetricDescriptor&) = delete;

  protected:
    // Constructors.
    //
    AbstractMetricDescriptor(DescriptorKind kind, const HandleType&, UnitType,
                             const MetricCategoryType&, const MetricAvailabilityType&);
    AbstractMetricDescriptor(AbstractMetricDescriptor&&) = default;
    AbstractMetricDescriptor& operator=(AbstractMetricDescriptor&&) = default;
    virtual ~AbstractMetricDescriptor() = default;

  private:
    UnitType Unit_;
    MetricCategoryType MetricCategory_;
    MetricAvailabilityType MetricAvailability_;
  };
  class NumericMetricDescriptor : public AbstractMetricDescriptor
  {
  public:
    using TechnicalRangeType = Range;
    using TechnicalRangeSequence = std::vector<TechnicalRangeType>;
    const TechnicalRangeSequence& TechnicalRange() const;
    TechnicalRangeSequence& TechnicalRange();

    using ResolutionType = int;
    const ResolutionType& Resolution() const;
    ResolutionType& Resolution();

    using AveragingPeriodType = std::string;
    using AveragingPeriodOptional = std::optional<AveragingPeriodType>;
    const AveragingPeriodOptional& AveragingPeriod() const;
    AveragingPeriodOptional& AveragingPeriod();

    static bool classof(const AbstractDescriptor* other);

    // Constructors
    //
    NumericMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                            const MetricAvailabilityType&, const ResolutionType&);

  private:
    TechnicalRangeSequence TechnicalRange_;
    ResolutionType Resolution_;
    AveragingPeriodOptional AveragingPeriod_;
  };
  class ChannelDescriptor : public AbstractDeviceComponentDescriptor
  {
  public:
    // Metric
    //
    using MetricType = std::shared_ptr<AbstractMetricDescriptor>;
    using MetricSequence = std::vector<MetricType>;
    const MetricSequence& Metric() const;
    MetricSequence& Metric();
    // Constructors
    //
    explicit ChannelDescriptor(const HandleType&);
    static bool classof(const AbstractDescriptor* other);

  private:
    MetricSequence Metric_;
  };
  class ScoDescriptor : public AbstractDeviceComponentDescriptor
  {
  public:
    using OperationType = ::BICEPS::PM::AbstractOperationDescriptor;
    using OperationSequence = std::vector<std::shared_ptr<OperationType>>;
    const OperationSequence& Operation() const;
    OperationSequence& Operation();

    explicit ScoDescriptor(const HandleType& handle);
    static bool classof(const AbstractDescriptor* other);

  private:
    OperationSequence Operation_;
  };
  class VmdDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
  public:
    // Channel
    //
    using ChannelType = ChannelDescriptor;
    using ChannelSequence = std::vector<ChannelType>;
    const ChannelSequence& Channel() const;
    ChannelSequence& Channel();

    // Sco
    //
    using ScoType = ::BICEPS::PM::ScoDescriptor;
    using ScoOptional = std::optional<ScoType>;
    const ScoOptional& Sco() const;
    ScoOptional& Sco();

    explicit VmdDescriptor(const HandleType&);

    static bool classof(const AbstractDescriptor* other);

  private:
    ChannelSequence Channel_;
    ScoOptional Sco_;
  };
  class MdsDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
  public:
    // MetaData
    //
    using MetaDataType = Metadata;
    using MetaDataOptional = std::optional<MetaDataType>;
    const MetaDataOptional& MetaData() const;
    MetaDataOptional& MetaData();

    // SystemContext
    //
    using SystemContextType = SystemContextDescriptor;
    using SystemContextOptional = std::optional<SystemContextType>;
    const SystemContextOptional& SystemContext() const;
    SystemContextOptional& SystemContext();

    // Clock
    //
    using ClockType = ClockDescriptor;
    using ClockOptional = std::optional<ClockType>;
    const ClockOptional& Clock() const;
    ClockOptional& Clock();

    // Battery
    //
    using BatteryType = BatteryDescriptor;
    using BatterySequence = std::vector<BatteryType>;
    const BatterySequence& Battery() const;
    BatterySequence& Battery();

    // Vmd
    //
    using VmdType = VmdDescriptor;
    using VmdSequence = std::vector<VmdType>;
    const VmdSequence& Vmd() const;
    VmdSequence& Vmd();

    // Constructors.
    //
    explicit MdsDescriptor(const HandleType&);
    static bool classof(const AbstractDescriptor* other);

  private:
    MetaDataOptional MetaData_;
    SystemContextOptional SystemContext_;
    ClockOptional Clock_;
    BatterySequence Battery_;
    VmdSequence Vmd_;
  };
  class MdDescription
  {
  public:
    // Mds
    //
    using MdsType = MdsDescriptor;
    using MdsSequence = std::vector<MdsType>;
    const MdsSequence& Mds() const;
    MdsSequence& Mds();

    // DescriptionVersion
    //
    using DescriptionVersionType = unsigned int;
    using DescriptionVersionOptional = std::optional<DescriptionVersionType>;
    const DescriptionVersionOptional& DescriptionVersion() const;
    DescriptionVersionOptional& DescriptionVersion();

    // Constructors.
    //
    MdDescription() = default;

  private:
    MdsSequence Mds_;
    DescriptionVersionOptional DescriptionVersion_;
  };
  class AbstractState
  {
  public:
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
    // StateVersion
    //
    using StateVersionType = unsigned int;
    using StateVersionOptional = std::optional<StateVersionType>;
    const StateVersionOptional& StateVersion() const;
    StateVersionOptional& StateVersion();

    // DescriptorHandle
    //
    using DescriptorHandleType = std::string;
    const DescriptorHandleType& DescriptorHandle() const;
    DescriptorHandleType& DescriptorHandle();

    StateKind getKind() const;

  protected:
    // Constructors.
    //
    AbstractState(StateKind kind, DescriptorHandleType);

  private:
    StateKind kind_;
    StateVersionOptional StateVersion_;
    DescriptorHandleType DescriptorHandle_;
  };
  class AbstractMultiState : public AbstractState
  {
  public:
    using CategoryType = CodedValue;
    using CategoryOptional = std::optional<CategoryType>;

    using HandleType = std::string;
    const HandleType& Handle() const;
    HandleType& Handle();

    AbstractMultiState(StateKind kind, const DescriptorHandleType&, HandleType);
    static bool classof(const AbstractState* other);

  private:
    CategoryOptional Category_;
    HandleType Handle_;
  };
  class InstanceIdentifier
  {
  public:
    using ExtensionType = std::string;
    using ExtensionOptional = std::optional<ExtensionType>;
    const ExtensionOptional& Extension() const;
    ExtensionOptional& Extension();

    using RootType = WS::ADDRESSING::URIType;
    using RootOptional = std::optional<RootType>;
    const RootOptional& Root() const;
    RootOptional& Root();

  private:
    ExtensionOptional Extension_;
    RootOptional Root_;
  };
  class AbstractContextState : public AbstractMultiState
  {
  public:
    using BindingMdibVersionType = unsigned int;
    using BindingMdibVersionOptional = std::optional<BindingMdibVersionType>;
    const BindingMdibVersionOptional& BindingMdibVersion() const;
    BindingMdibVersionOptional& BindingMdibVersion();

    using ContextAssociationType = ::BICEPS::PM::ContextAssociation;
    using ContextAssociationOptional = std::optional<ContextAssociationType>;
    const ContextAssociationOptional& ContextAssociation() const;
    ContextAssociationOptional& ContextAssociation();

    using ValidatorType = InstanceIdentifier;
    using ValidatorSequence = std::vector<ValidatorType>;
    const ValidatorSequence& Validator() const;
    ValidatorSequence& Validator();

    using IdentificationType = InstanceIdentifier;
    using IdentificationSequence = std::vector<IdentificationType>;
    const IdentificationSequence& Identification() const;
    IdentificationSequence& Identification();

    AbstractContextState(StateKind kind, const DescriptorHandleType&, const HandleType&);
    static bool classof(const AbstractState* other);

  private:
    BindingMdibVersionOptional BindingMdibVersion_;
    ContextAssociationOptional ContextAssociation_;
    ValidatorSequence Validator_;
    IdentificationSequence Identification_;
  };
  class LocationDetailType
  {
  public:
    using PoCType = std::string;
    using PoCOptional = std::optional<PoCType>;
    const PoCOptional& PoC() const;
    PoCOptional& PoC();

    using RoomType = std::string;
    using RoomOptional = std::optional<RoomType>;
    const RoomOptional& Room() const;
    RoomOptional& Room();

    using BedType = std::string;
    using BedOptional = std::optional<PoCType>;
    const BedOptional& Bed() const;
    BedOptional& Bed();

    using FacilityType = std::string;
    using FacilityOptional = std::optional<FacilityType>;
    const FacilityOptional& Facility() const;
    FacilityOptional& Facility();

    using BuildingType = std::string;
    using BuildingOptional = std::optional<PoCType>;
    const BuildingOptional& Building() const;
    BuildingOptional& Building();

    using FloorType = std::string;
    using FloorOptional = std::optional<PoCType>;
    const FloorOptional& Floor() const;
    FloorOptional& Floor();

  private:
    PoCOptional PoC_;
    RoomOptional Room_;
    BedOptional Bed_;
    FacilityOptional Facility_;
    BuildingOptional Building_;
    FloorOptional Floor_;
  };
  class LocationContextState : public AbstractContextState
  {
  public:
    using LocationDetailOptional = std::optional<LocationDetailType>;
    const LocationDetailOptional& LocationDetail() const;
    LocationDetailOptional& LocationDetail();

    LocationContextState(const DescriptorHandleType&, const HandleType&);
    static bool classof(const AbstractState* other);

  private:
    LocationDetailOptional LocationDetail_;
  };
  class AbstractOperationState : public AbstractState
  {
  public:
    using OperatingModeType = ::BICEPS::PM::OperatingMode;
    const OperatingModeType& OperatingMode() const;
    OperatingModeType& OperatingMode();
    static bool classof(const AbstractState* other);

  protected:
    AbstractOperationState(StateKind kind, const DescriptorHandleType& descriptorHandle,
                           const OperatingModeType& operatingMode);

  private:
    OperatingModeType OperatingMode_;
  };
  class SetValueOperationState : public AbstractOperationState
  {
  public:
    SetValueOperationState(const DescriptorHandleType& descriptorHandle,
                           const OperatingModeType& operatingMode);
    static bool classof(const AbstractState* other);
  };
  class MetricQuality
  {
  public:
    using ValidityType = MeasurementValidity;
    const ValidityType& Validity() const;
    ValidityType& Validity();

    using ModeType = GenerationMode;
    using ModeOptional = std::optional<ModeType>;

    using QiType = int;
    using QiOptional = std::optional<QiType>;

    explicit MetricQuality(const ValidityType& validity);

  private:
    ValidityType Validity_;
    ModeOptional Mode_;
    QiOptional Qi_;
  };
  class Annotation
  {
  public:
    using TypeType = CodedValue;

  private:
    TypeType Type_;
  };

  using Timestamp = unsigned int;

  class AbstractMetricValue
  {
  public:
    enum class MetricKind
    {
      NUMERIC_METRIC,
    };
    using MetricQualityType = MetricQuality;
    const MetricQualityType& Quality() const;
    MetricQualityType& Quality();

    using AnnotationType = Annotation;
    using AnnotationSequence = std::vector<AnnotationType>;

    using StartTimeType = Timestamp;
    using StartTimeOptional = std::optional<StartTimeType>;

    using StopTimeType = Timestamp;
    using StopTimeOptional = std::optional<StopTimeType>;

    using DeterminationTimeType = Timestamp;
    using DeterminationTimeOptional = std::optional<DeterminationTimeType>;

    MetricKind getKind() const;

  private:
    MetricKind kind_;

  protected:
    explicit AbstractMetricValue(MetricKind kind, const MetricQualityType& metricQuality);

  private:
    MetricQualityType MetricQuality_;
    AnnotationSequence Annotation_;
    StartTimeOptional StartTime_;
    StopTimeOptional StopTime_;
    DeterminationTimeOptional DeterminationTime_;
  };
  class NumericMetricValue : public AbstractMetricValue
  {
  public:
    using ValueType = double;
    using ValueOptional = std::optional<ValueType>;
    const ValueOptional& Value() const;
    ValueOptional& Value();

    explicit NumericMetricValue(const MetricQualityType& metricQuality);
    static bool classof(const AbstractMetricValue* other);

  private:
    ValueOptional Value_;
  };
  class AbstractMetricState : public AbstractState
  {
  public:
    using ActivationStateType = ComponentActivation;
    using ActivationStateOptional = std::optional<ActivationStateType>;
    static bool classof(const AbstractState* other);

  protected:
    // Constructors
    //
    AbstractMetricState(StateKind kind, DescriptorHandleType handle);

  private:
    ActivationStateOptional ActivationState_;
  };
  class NumericMetricState : public AbstractMetricState
  {
  public:
    using MetricValueType = NumericMetricValue;
    using MetricValueOptional = std::optional<MetricValueType>;
    const MetricValueOptional& MetricValue() const;
    MetricValueOptional& MetricValue();

    using PhysiologicalRangeType = Range;
    using PhysiologicalRangeSequence = std::vector<PhysiologicalRangeType>;

    using ActiveAveragingPeriodType = std::string;
    using ActiveAveragingPeriodOptional = std::optional<ActiveAveragingPeriodType>;

    static bool classof(const AbstractState* other);

    // Constructors
    //
    explicit NumericMetricState(DescriptorHandleType handle);

  private:
    MetricValueOptional MetricValue_;
    PhysiologicalRangeSequence PhysiologicalRange_;
    ActiveAveragingPeriodOptional ActiveAveragingPeriod_;
  };
  class MdState
  {
  public:
    // State
    //
    using StateType = std::shared_ptr<AbstractState>;
    using StateSequence = std::vector<StateType>;
    const StateSequence& State() const;
    StateSequence& State();

    // StateVersion
    //
    using StateVersionType = unsigned int;
    using StateVersionOptional = std::optional<StateVersionType>;
    const StateVersionOptional& StateVersion() const;
    StateVersionOptional& StateVersion();

  private:
    StateSequence State_;
    StateVersionOptional StateVersion_;
  };
  class Mdib
  {
  public:
    // MdDescription
    //
    using MdDescriptionType = ::BICEPS::PM::MdDescription;
    using MdDescriptionOptional = std::optional<MdDescriptionType>;
    const MdDescriptionOptional& MdDescription() const;
    MdDescriptionOptional& MdDescription();

    // MdState
    //
    using MdStateType = ::BICEPS::PM::MdState;
    using MdStateOptional = std::optional<MdStateType>;
    const MdStateOptional& MdState() const;
    MdStateOptional& MdState();

    // MdibVersion
    //
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    const MdibVersionOptional& MdibVersion() const;
    MdibVersionOptional& MdibVersion();

    // SequenceId
    //
    using SequenceIdType = WS::ADDRESSING::URIType;
    const SequenceIdType& SequenceId() const;
    SequenceIdType& SequenceId();

    // InstanceId
    //
    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    const InstanceIdOptional& InstanceId() const;
    InstanceIdOptional& InstanceId();

    // Constructors.
    //
    explicit Mdib(SequenceIdType sequenceIdType);

  private:
    MdDescriptionOptional MdDescription_;
    MdStateOptional MdState_;
    MdibVersionOptional MdibVersion_;
    SequenceIdType SequenceId_;
    InstanceIdOptional InstanceId_;
  };
} // namespace BICEPS::PM
