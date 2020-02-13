#pragma once

#include "ws-addressing.hpp"
#include <optional>
#include <string>
#include <vector>

namespace BICEPS::PM
{
  class CodedValue
  {
  public:
    using CodeType = std::string;
    const CodeType& Code() const;
    CodeType& Code();

    CodedValue(const CodeType& code);

  private:
    CodeType Code_;
  };
  enum class SafetyClassification
  {
    Inf,
    MedA,
    MedB,
    MedC
  };
  class AbstractDescriptor
  {
  public:
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

  protected:
    // Constructors.
    //
    AbstractDescriptor(const HandleType&);

  protected:
    TypeOptional Type_;
    HandleType Handle_;
    DescriptorVersionOptional DescriptorVersion_;
    SafetyClassificationOptional SafetyClassification_;
  };
  class AbstractDeviceComponentDescriptor : public AbstractDescriptor
  {
  protected:
    AbstractDeviceComponentDescriptor(const HandleType&);
  };
  class AbstractComplexDeviceComponentDescriptor : public AbstractDeviceComponentDescriptor
  {
  protected:
    AbstractComplexDeviceComponentDescriptor(const HandleType&);
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

  protected:
    ManufacturerSequence Manufacturer_;
    ModelNameSequence ModelName_;
    ModelNumberOptional ModelNumber_;
    SerialNumberSequence SerialNumber_;
  };
  class AbstractContextDescriptor : public AbstractDescriptor
  {
  protected:
    AbstractContextDescriptor(const HandleType&);
  };
  class PatientContextDescriptor : public AbstractContextDescriptor
  {
  public:
    // Constructors
    //
    PatientContextDescriptor(const HandleType&);
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

    // Constructors
    //
    SystemContextDescriptor(const HandleType&);

  protected:
    PatientContextOptional PatientContext_;
  };
  class ClockDescriptor
  {
  };
  class BatteryDescriptor
  {
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

  protected:
    LowerOptional Lower_;
    UpperOptional Upper_;
    StepWidthOptional StepWidth_;
    RelativeAccuracyOptional RelativeAccuracy_;
    AbsoluteAccuracyOptional AbsoluteAccuracy_;
  };
  enum class MetricType
  {
    NUMERIC,
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

    [[nodiscard]] virtual MetricType getMetricType() const = 0;

  protected:
    // Constructors.
    //
    AbstractMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                             const MetricAvailabilityType&);
    virtual ~AbstractMetricDescriptor() = default;

  protected:
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

    [[nodiscard]] MetricType getMetricType() const override;

    // Constructors
    //
    NumericMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                            const MetricAvailabilityType&, const ResolutionType&);

  protected:
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
    ChannelDescriptor(const HandleType&);

  protected:
    MetricSequence Metric_;
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

    VmdDescriptor(const HandleType&);

  protected:
    ChannelSequence Channel_;
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
    MdsDescriptor(const HandleType&);

  protected:
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

  protected:
    MdsSequence Mds_;
    DescriptionVersionOptional DescriptionVersion_;
  };
  enum class StateType
  {
    NUMERIC_METRIC_STATE,
  };
  class AbstractState
  {
  public:
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

    virtual StateType getStateType() const = 0;

  protected:
    // Constructors.
    //
    AbstractState(DescriptorHandleType);

  protected:
    StateVersionOptional StateVersion_;
    DescriptorHandleType DescriptorHandle_;
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
  enum class GenerationMode
  {
    Real,
    Test,
    Demo
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

  protected:
    ValidityType Validity_;
    ModeOptional Mode_;
    QiOptional Qi_;
  };
  class Annotation
  {
  public:
    using TypeType = CodedValue;

  protected:
    TypeType Type_;
  };

  using Timestamp = unsigned int;

  class AbstractMetricValue
  {
  public:
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

    [[nodiscard]] virtual MetricType getMetricType() const = 0;

  protected:
    MetricQualityType MetricQuality_;
    AnnotationSequence Annotation_;
    StartTimeOptional StartTime_;
    StopTimeOptional StopTime_;
    DeterminationTimeOptional DeterminationTime_;
  };
  class NumericMetricValue : public AbstractMetricValue
  {
  public:
    using ValueType = int;
    using ValueOptional = std::optional<ValueType>;
    const ValueOptional& Value() const;
    ValueOptional& Value();

    [[nodiscard]] MetricType getMetricType() const override;

  protected:
    ValueOptional Value_;
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
  class AbstractMetricState : public AbstractState
  {
  public:
    using ActivationStateType = ComponentActivation;
    using ActivationStateOptional = std::optional<ActivationStateType>;

  protected:
    // Constructors
    //
    AbstractMetricState(DescriptorHandleType handle);

  protected:
    ActivationStateOptional ActivationState_;
    /* TODO
  BodySiteSequence BodySite_;
  PhysicalConnectorOptional PhysicalConnector_;
  ActiveDeterminationPeriodOptional ActiveDeterminationPeriod_;
  LifeTimePeriodOptional LifeTimePeriod_;
  */
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

    StateType getStateType() const override;

    // Constructors
    //
    NumericMetricState(DescriptorHandleType handle);

  protected:
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

  protected:
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
    Mdib(const SequenceIdType&);

  protected:
    MdDescriptionOptional MdDescription_;
    MdStateOptional MdState_;
    MdibVersionOptional MdibVersion_;
    SequenceIdType SequenceId_;
    InstanceIdOptional InstanceId_;
  };
} // namespace BICEPS::PM
