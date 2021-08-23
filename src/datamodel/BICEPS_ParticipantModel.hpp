#pragma once

#include "ws-addressing.hpp"
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace BICEPS::PM
{
  enum class SafetyClassification
  {
    INF,
    MED_A,
    MED_B,
    MED_C
  };

  enum class MetricCategory
  {
    UNSPEC,
    MSRMT,
    CLC,
    SET,
    PRESET,
    RCMM
  };

  enum class MetricAvailability
  {
    INTR,
    CONT
  };

  enum class MeasurementValidity
  {
    VLD,
    VLDATED,
    ONG,
    QST,
    CALIB,
    INV,
    OFLW,
    UFLW,
    NA
  };

  enum class ContextAssociation
  {
    NO,
    PRE,
    ASSOC,
    DIS
  };

  enum class GenerationMode
  {
    REAL,
    TEST,
    DEMO
  };

  enum class ComponentActivation
  {
    ON,
    NOT_RDY,
    STND_BY,
    OFF,
    SHTDN,
    FAIL
  };

  enum class OperatingMode
  {
    DIS,
    EN,
    NA
  };

  enum class CalibrationState
  {
    NO,
    REQ,
    RUN,
    CAL,
    OTH
  };

  enum class CalibrationType
  {
    OFFSET,
    GAIN,
    TP,
    UNSPEC
  };

  enum class LocalizedTextWidth
  {
    XS,
    S,
    M,
    L,
    XL,
    XXL
  };

  enum class MdsOperatingMode
  {
    /// Normal
    NML,
    /// Demo
    DMO,
    /// Service
    SRV,
    /// Maintenance
    MTN
  };

  using LocalizedTextRef = std::string;

  struct LocalizedText
  {
    using RefType = LocalizedTextRef;
    using RefOptional = boost::optional<RefType>;
    RefOptional ref;

    using LangType = std::string;
    using LangOptional = boost::optional<LangType>;
    LangOptional lang;

    using VersionType = unsigned int;
    using VersionOptional = boost::optional<VersionType>;
    VersionOptional version;

    using TextWidthType = LocalizedTextWidth;
    using TextWidthOptional = boost::optional<TextWidthType>;
    TextWidthOptional text_width;

    using ContentType = std::string;
    ContentType content;

    LocalizedText() = default;
    explicit LocalizedText(std::string content);
  };

  struct CodedValue
  {
    using CodeType = std::string;
    CodeType code;

    using ConceptDescriptionType = LocalizedText;
    using ConceptDescriptionOptional = boost::optional<ConceptDescriptionType>;
    ConceptDescriptionOptional concept_description;

    explicit CodedValue(CodeType code);
  };

  struct AbstractDescriptor
  {
    enum class DescriptorKind
    {
      METRIC_DESCRIPTOR,
      NUMERIC_METRIC_DESCRIPTOR,
      STRING_METRIC_DESCRIPTOR,
      ENUM_STRING_METRIC_DESCRIPTOR,
      LAST_METRIC_DESCRIPTOR,

      OPERATION_DESCRIPTOR,
      SET_VALUE_OPERATION_DESCRIPTOR,
      SET_STRING_OPERATION_DESCRIPTOR,
      LAST_OPERATION_DESCRIPTOR,

      DEVICE_COMPONENT_DESCRIPTOR,
      COMPLEX_DEVICE_COMPONENT_DESCRIPTOR,
      VMD_DESCRIPTOR,
      MDS_DESCRIPTOR,
      LAST_COMPLEX_DEVICE_COMPONENT_DESCRIPTOR,
      CHANNEL_DESCRIPTOR,
      SCO_DESCRIPTOR,
      SYSTEM_CONTEXT_DESCRIPTOR,
      LAST_DEVICE_COMPONENT_DESCRIPTOR,

      CONTEXT_DESCRIPTOR,
      PATIENT_CONTEXT_DESCRIPTOR,
      LOCATION_CONTEXT_DESCRIPTOR,
      LAST_CONTEXT_DESCRIPTOR,
    };
    DescriptorKind get_kind() const;

    using TypeType = CodedValue;
    using TypeOptional = boost::optional<TypeType>;
    TypeOptional type;

    using HandleType = std::string;
    HandleType handle;

    using DescriptorVersionType = unsigned int;
    using DescriptorVersionOptional = boost::optional<DescriptorVersionType>;
    DescriptorVersionOptional descriptor_version;

    using SafetyClassificationType = ::BICEPS::PM::SafetyClassification;
    using SafetyClassificationOptional = boost::optional<SafetyClassificationType>;
    SafetyClassificationOptional safety_classification;

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
    OperationTargetType operation_target;

    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractOperationDescriptor(DescriptorKind kind, const HandleType& handle,
                                OperationTargetType operation_target);
  };

  struct SetStringOperationDescriptor : public AbstractOperationDescriptor
  {
    using MaxLengthType = std::uint64_t;
    using MaxLengthOptional = boost::optional<MaxLengthType>;
    MaxLengthOptional max_length;

    static bool classof(const AbstractDescriptor* other);

    SetStringOperationDescriptor(const HandleType& handle,
                                 const OperationTargetType& operation_target);
  };

  struct SetValueOperationDescriptor : public AbstractOperationDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    SetValueOperationDescriptor(const HandleType& handle,
                                const OperationTargetType& operation_target);
  };

  struct AbstractDeviceComponentDescriptor : public AbstractDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractDeviceComponentDescriptor(DescriptorKind kind, const HandleType&);
  };

  struct ScoDescriptor : public AbstractDeviceComponentDescriptor
  {
    using OperationType = ::BICEPS::PM::AbstractOperationDescriptor;
    using OperationSequence = std::vector<std::shared_ptr<OperationType>>;
    OperationSequence operation;

    static bool classof(const AbstractDescriptor* other);

    explicit ScoDescriptor(const HandleType& handle);
  };

  struct AbstractComplexDeviceComponentDescriptor : public AbstractDeviceComponentDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    using ScoType = ::BICEPS::PM::ScoDescriptor;
    using ScoOptional = boost::optional<ScoType>;
    ScoOptional sco;

  protected:
    AbstractComplexDeviceComponentDescriptor(DescriptorKind kind, const HandleType&);
  };

  struct Metadata
  {
  public:
    using ManufacturerType = LocalizedText;
    using ManufacturerSequence = std::vector<ManufacturerType>;
    ManufacturerSequence manufacturer;

    using ModelNameType = LocalizedText;
    using ModelNameSequence = std::vector<ModelNameType>;
    ModelNameSequence model_name;

    using ModelNumberType = std::string;
    using ModelNumberOptional = boost::optional<ModelNumberType>;
    ModelNumberOptional model_number;

    using SerialNumberType = std::string;
    using SerialNumberSequence = std::vector<SerialNumberType>;
    SerialNumberSequence serial_number;
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

  struct SystemContextDescriptor : public AbstractDeviceComponentDescriptor
  {
    using PatientContextType = PatientContextDescriptor;
    using PatientContextOptional = boost::optional<PatientContextType>;
    PatientContextOptional patient_context;

    using LocationContextType = LocationContextDescriptor;
    using LocationContextOptional = boost::optional<LocationContextType>;
    LocationContextOptional location_context;

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
    using LowerOptional = boost::optional<LowerType>;
    LowerOptional lower;

    using UpperType = int;
    using UpperOptional = boost::optional<UpperType>;
    UpperOptional upper;

    using StepWidthType = int;
    using StepWidthOptional = boost::optional<StepWidthType>;
    StepWidthOptional step_width;

    using RelativeAccuracyType = int;
    using RelativeAccuracyOptional = boost::optional<RelativeAccuracyType>;
    RelativeAccuracyOptional relative_accuracy;

    using AbsoluteAccuracyType = int;
    using AbsoluteAccuracyOptional = boost::optional<AbsoluteAccuracyType>;
    AbsoluteAccuracyOptional absolute_accuracy;
  };

  struct InstanceIdentifier
  {
    using ExtensionType = std::string;
    using ExtensionOptional = boost::optional<ExtensionType>;
    ExtensionOptional extension;

    using RootType = WS::ADDRESSING::URIType;
    using RootOptional = boost::optional<RootType>;
    RootOptional root;
  };

  struct AbstractMetricDescriptor : public AbstractDescriptor
  {
    using UnitType = CodedValue;
    UnitType unit;

    using MetricCategoryType = ::BICEPS::PM::MetricCategory;
    MetricCategoryType metric_category;

    using MetricAvailabilityType = ::BICEPS::PM::MetricAvailability;
    MetricAvailabilityType metric_availability;

    static bool classof(const AbstractDescriptor* other);

  protected:
    AbstractMetricDescriptor(DescriptorKind kind, const HandleType&, UnitType,
                             const MetricCategoryType&, const MetricAvailabilityType&);
  };

  struct NumericMetricDescriptor : public AbstractMetricDescriptor
  {
    using TechnicalRangeType = Range;
    using TechnicalRangeSequence = std::vector<TechnicalRangeType>;
    TechnicalRangeSequence technical_range;

    using ResolutionType = int;
    ResolutionType resolution;

    using AveragingPeriodType = std::string;
    using AveragingPeriodOptional = boost::optional<AveragingPeriodType>;
    AveragingPeriodOptional averaging_period;

    static bool classof(const AbstractDescriptor* other);

    NumericMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                            const MetricAvailabilityType&, const ResolutionType&);
  };

  struct StringMetricDescriptor : public AbstractMetricDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    StringMetricDescriptor(const HandleType& handle, const UnitType& unit,
                           const MetricCategoryType& metric_category,
                           const MetricAvailabilityType& metric_availability);
  };

  struct Measurement
  {
    using MeasuredValueType = double;
    MeasuredValueType measured_value;

    using MeasurementUnitType = CodedValue;
    MeasurementUnitType measurement_unit;

    Measurement(MeasuredValueType measured_value, MeasurementUnitType measurement_unit);
  };

  struct AllowedValue
  {
    using ValueType = std::string;
    ValueType value;

    using TypeType = CodedValue;
    using TypeOptional = boost::optional<TypeType>;
    TypeOptional type;

    using IdentificationType = InstanceIdentifier;
    using IdentificationOptional = boost::optional<IdentificationType>;
    IdentificationOptional identification;

    using CharacteristicType = Measurement;
    using CharacteristicOptional = boost::optional<CharacteristicType>;
    CharacteristicOptional characteristic;

    explicit AllowedValue(ValueType value);
  };

  struct EnumStringMetricDescriptor : public AbstractMetricDescriptor
  {
    static bool classof(const AbstractDescriptor* other);

    using AllowedValueType = AllowedValue;
    using AllowedValueSequence = std::vector<AllowedValueType>;
    AllowedValueSequence allowed_value;

    EnumStringMetricDescriptor(const HandleType& handle, const UnitType& unit,
                               const MetricCategoryType& metric_category,
                               const MetricAvailabilityType& metric_availability,
                               AllowedValueSequence allowed_value);
  };

  struct ChannelDescriptor : public AbstractDeviceComponentDescriptor
  {
    using MetricType = std::shared_ptr<AbstractMetricDescriptor>;
    using MetricSequence = std::vector<MetricType>;
    MetricSequence metric;

    static bool classof(const AbstractDescriptor* other);

    explicit ChannelDescriptor(const HandleType&);
  };

  struct VmdDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
    using ChannelType = ChannelDescriptor;
    using ChannelSequence = std::vector<ChannelType>;
    ChannelSequence channel;

    static bool classof(const AbstractDescriptor* other);

    explicit VmdDescriptor(const HandleType&);
  };

  struct MdsDescriptor : public AbstractComplexDeviceComponentDescriptor
  {
    using MetaDataType = Metadata;
    using MetaDataOptional = boost::optional<MetaDataType>;
    MetaDataOptional meta_data;

    using SystemContextType = SystemContextDescriptor;
    using SystemContextOptional = boost::optional<SystemContextType>;
    SystemContextOptional system_context;

    using ClockType = ClockDescriptor;
    using ClockOptional = boost::optional<ClockType>;
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
    using DescriptionVersionOptional = boost::optional<DescriptionVersionType>;
    DescriptionVersionOptional description_version;
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

      DEVICE_COMPONENT_STATE,
      SYSTEM_CONTEXT_STATE,
      CHANNEL_STATE,
      SCO_STATE,
      COMPLEX_DEVICE_COMPONENT_STATE,
      MDS_STATE,
      VMD_STATE,
      LAST_COMPLEX_DEVICE_COMPONENT_STATE,
      LAST_DEVICE_COMPONENT_STATE,

      OPERATION_STATE,
      SET_VALUE_OPERATION_STATE,
      SET_STRING_OPERATION_STATE,
      LAST_OPERATION_STATE,

      METRIC_STATE,
      NUMERIC_METRIC_STATE,
      STRING_METRIC_STATE,
      ENUM_STRING_METRIC_STATE,
      LAST_METRIC_STATE,
    };
    StateKind get_kind() const;

    using StateVersionType = unsigned int;
    using StateVersionOptional = boost::optional<StateVersionType>;
    StateVersionOptional state_version;

    using DescriptorHandleType = std::string;
    DescriptorHandleType descriptor_handle;

    using DescriptorVersionType = unsigned int;
    using DescriptorVersionOptional = boost::optional<DescriptorVersionType>;
    DescriptorVersionOptional descriptor_version;

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
    using CategoryOptional = boost::optional<CategoryType>;
    CategoryOptional category;

    using HandleType = std::string;
    HandleType handle;

    static bool classof(const AbstractState* other);

    AbstractMultiState(StateKind kind, const DescriptorHandleType&, HandleType);
  };

  struct AbstractContextState : public AbstractMultiState
  {
    using BindingMdibVersionType = unsigned int;
    using BindingMdibVersionOptional = boost::optional<BindingMdibVersionType>;
    BindingMdibVersionOptional binding_mdib_version;

    using ContextAssociationType = ::BICEPS::PM::ContextAssociation;
    using ContextAssociationOptional = boost::optional<ContextAssociationType>;
    ContextAssociationOptional context_association;

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
    using PoCOptional = boost::optional<PoCType>;
    PoCOptional poc;

    using RoomType = std::string;
    using RoomOptional = boost::optional<RoomType>;
    RoomOptional room;

    using BedType = std::string;
    using BedOptional = boost::optional<PoCType>;
    BedOptional bed;

    using FacilityType = std::string;
    using FacilityOptional = boost::optional<FacilityType>;
    FacilityOptional facility;

    using BuildingType = std::string;
    using BuildingOptional = boost::optional<PoCType>;
    BuildingOptional building;

    using FloorType = std::string;
    using FloorOptional = boost::optional<PoCType>;
    FloorOptional floor;
  };

  struct LocationContextState : public AbstractContextState
  {
    using LocationDetailOptional = boost::optional<LocationDetail>;
    LocationDetailOptional location_detail;

    static bool classof(const AbstractState* other);

    LocationContextState(const DescriptorHandleType&, const HandleType&);
  };

  using Timestamp = unsigned int;

  struct PhysicalConnectorInfo
  {
    using NumberType = int;
    using NumberOptional = boost::optional<NumberType>;
    NumberOptional number;

    using LabelSequence = std::vector<LocalizedText>;
    LabelSequence label;
  };

  struct CalibrationDocumentation
  {
    using DocumentationType = LocalizedText;
    using DocumentationOptional = boost::optional<DocumentationType>;
    DocumentationOptional documentation;

    using CalibrationResultType = CodedValue;
    using CalibrationResultOptional = boost::optional<CalibrationResultType>;
    CalibrationResultOptional calibration_result;
  };

  struct CalibrationInfo
  {
    using ComponentCalibrationStateType = CalibrationState;
    using ComponentCalibrationStateOptional = boost::optional<ComponentCalibrationStateType>;
    ComponentCalibrationStateOptional component_calibration_state;

    using TypeType = CalibrationType;
    using TypeOptional = boost::optional<TypeType>;
    TypeOptional type;

    using TimeType = Timestamp;
    using TimeOptional = boost::optional<TimeType>;
    TimeOptional time;

    using CalibrationDocumentationType = CalibrationDocumentation;
    using CalibrationDocumentationOptional = boost::optional<CalibrationDocumentationType>;
    CalibrationDocumentationOptional calibration_documentation;
  };

  struct AbstractDeviceComponentState : public AbstractState
  {
    using ActivationStateType = ComponentActivation;
    using ActivationStateOptional = boost::optional<ActivationStateType>;
    ActivationStateOptional activation_state;

    using OperatingHoursType = unsigned int;
    using OperatingHoursOptional = boost::optional<OperatingHoursType>;
    OperatingHoursOptional operating_hours;

    using OperatingCyclesType = int;
    using OperatingCyclesOptional = boost::optional<OperatingCyclesType>;
    OperatingCyclesOptional operating_cycles;

    using CalibrationInfoType = struct CalibrationInfo;
    using CalibrationInfoOptional = boost::optional<CalibrationInfoType>;
    CalibrationInfoOptional calibration_info;

    using NextCalibrationType = struct CalibrationInfo;
    using NextCalibrationOptional = boost::optional<NextCalibrationType>;
    NextCalibrationOptional next_calibration;

    using PhysicalConnectorType = PhysicalConnectorInfo;
    using PhysicalConnectorOptional = boost::optional<PhysicalConnectorType>;
    PhysicalConnectorOptional physical_connector;

    static bool classof(const AbstractState* other);

  protected:
    AbstractDeviceComponentState(StateKind kind, DescriptorHandleType handle);
  };

  struct SystemContextState : public AbstractDeviceComponentState
  {
    static bool classof(const AbstractState* other);
    explicit SystemContextState(const DescriptorHandleType& descriptor_handle);
  };

  struct ChannelState : public AbstractDeviceComponentState
  {
    static bool classof(const AbstractState* other);
    explicit ChannelState(const DescriptorHandleType& descriptor_handle);
  };

  using HandleRef = std::string;

  using OperationRef = std::vector<HandleRef>;

  struct OperationGroup
  {
    using OperatingModeType = OperatingMode;
    using OperatingModeOptional = boost::optional<OperatingModeType>;
    OperatingModeOptional operating_mode;

    using OperationsType = OperationRef;
    using OperationsOptional = boost::optional<OperationsType>;
    OperationsOptional operations;

    using TypeType = CodedValue;
    TypeType type;

    explicit OperationGroup(TypeType type);
  };

  struct ScoState : public AbstractDeviceComponentState
  {
    using InvocationRequestedType = OperationRef;
    using InvocationRequestedOptional = boost::optional<InvocationRequestedType>;
    InvocationRequestedOptional invocation_requested;

    using InvocationRequiredType = OperationRef;
    using InvocationRequiredOptional = boost::optional<InvocationRequiredType>;
    InvocationRequiredOptional invocation_required;

    using OperationGroupType = OperationGroup;
    using OperationGroupOptional = boost::optional<OperationGroupType>;
    OperationGroupOptional operation_group;

    static bool classof(const AbstractState* other);
    explicit ScoState(const DescriptorHandleType& descriptor_handle);
  };

  struct AbstractComplexDeviceComponentState : public AbstractDeviceComponentState
  {
  protected:
    AbstractComplexDeviceComponentState(StateKind kind, DescriptorHandleType handle);
  };

  struct OperatingJurisdiction : public InstanceIdentifier
  {
  };

  struct MdsState : public AbstractComplexDeviceComponentState
  {
    using LangType = std::string;
    using LangOptional = boost::optional<LangType>;
    LangOptional lang;

    using OperatingModeType = MdsOperatingMode;
    using OperatingModeOptional = boost::optional<OperatingModeType>;
    OperatingModeOptional operating_mode;

    using OperatingJurisdictionType = OperatingJurisdiction;
    using OperatingJurisdictionOptional = boost::optional<OperatingJurisdictionType>;
    OperatingJurisdictionOptional operating_jurisdiction;

    static bool classof(const AbstractState* other);
    explicit MdsState(DescriptorHandleType handle);
  };

  struct VmdState : public AbstractComplexDeviceComponentState
  {
    using OperatingJurisdictionType = OperatingJurisdiction;
    using OperatingJurisdictionOptional = boost::optional<OperatingJurisdictionType>;
    OperatingJurisdictionOptional operating_jurisdiction;

    static bool classof(const AbstractState* other);
    explicit VmdState(DescriptorHandleType handle);
  };

  struct AbstractOperationState : public AbstractState
  {
    using OperatingModeType = ::BICEPS::PM::OperatingMode;
    OperatingModeType operating_mode;

    static bool classof(const AbstractState* other);

  protected:
    AbstractOperationState(StateKind kind, const DescriptorHandleType& descriptor_handle,
                           const OperatingModeType& operating_mode);
  };

  struct SetStringOperationState : public AbstractOperationState
  {
    using AllowedValuesType = std::string;
    using AllowedValuesSequence = std::vector<AllowedValuesType>;
    AllowedValuesSequence allowed_values;

    static bool classof(const AbstractState* other);

    SetStringOperationState(const DescriptorHandleType& descriptor_handle,
                            const OperatingModeType& operating_mode);
  };

  struct SetValueOperationState : public AbstractOperationState
  {
    static bool classof(const AbstractState* other);

    SetValueOperationState(const DescriptorHandleType& descriptor_handle,
                           const OperatingModeType& operating_mode);
  };

  struct MetricQuality
  {
    using ValidityType = MeasurementValidity;
    ValidityType validity;

    using ModeType = GenerationMode;
    using ModeOptional = boost::optional<ModeType>;
    ModeOptional mode;

    using QiType = int;
    using QiOptional = boost::optional<QiType>;
    QiOptional qi;

    explicit MetricQuality(const ValidityType& validity);
  };

  struct Annotation
  {
    using TypeType = CodedValue;
    TypeType type;
  };

  struct AbstractMetricValue
  {
    enum class MetricKind
    {
      NUMERIC_METRIC,
      STRING_METRIC
    };
    MetricKind get_kind() const;

    MetricQuality metric_quality;

    using AnnotationSequence = std::vector<Annotation>;
    AnnotationSequence annotation;

    using StartTimeType = Timestamp;
    using StartTimeOptional = boost::optional<StartTimeType>;
    StartTimeOptional start_time;

    using StopTimeType = Timestamp;
    using StopTimeOptional = boost::optional<StopTimeType>;
    StopTimeOptional stop_time;

    using DeterminationTimeType = Timestamp;
    using DeterminationTimeOptional = boost::optional<DeterminationTimeType>;
    DeterminationTimeOptional determination_time;

    virtual ~AbstractMetricValue() = default;

  protected:
    explicit AbstractMetricValue(MetricKind kind, MetricQuality metric_quality);
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
    using ValueOptional = boost::optional<ValueType>;
    ValueOptional value;

    static bool classof(const AbstractMetricValue* other);

    explicit NumericMetricValue(const MetricQuality& metric_quality);
  };

  struct AbstractMetricState : public AbstractState
  {
    using ActivationStateType = ComponentActivation;
    using ActivationStateOptional = boost::optional<ActivationStateType>;
    ActivationStateOptional activation_state;

    static bool classof(const AbstractState* other);

  protected:
    AbstractMetricState(StateKind kind, DescriptorHandleType handle);
  };

  struct NumericMetricState : public AbstractMetricState
  {
    using MetricValueType = NumericMetricValue;
    using MetricValueOptional = boost::optional<MetricValueType>;
    MetricValueOptional metric_value;

    using PhysiologicalRangeType = Range;
    using PhysiologicalRangeSequence = std::vector<PhysiologicalRangeType>;
    PhysiologicalRangeSequence physiological_range;

    using ActiveAveragingPeriodType = std::string;
    using ActiveAveragingPeriodOptional = boost::optional<ActiveAveragingPeriodType>;
    ActiveAveragingPeriodOptional active_averaging_period;

    static bool classof(const AbstractState* other);

    explicit NumericMetricState(DescriptorHandleType handle);
  };

  struct StringMetricValue : public AbstractMetricValue
  {
    using ValueType = std::string;
    using ValueOptional = boost::optional<ValueType>;
    ValueOptional value;

    static bool classof(const AbstractMetricValue* other);

    explicit StringMetricValue(const MetricQuality& metric_quality);
  };

  struct StringMetricState : public AbstractMetricState
  {
    using MetricValueType = StringMetricValue;
    using MetricValueOptional = boost::optional<MetricValueType>;
    MetricValueOptional metric_value;

    static bool classof(const AbstractState* other);

    explicit StringMetricState(DescriptorHandleType handle);
    StringMetricState(StateKind kind, DescriptorHandleType handle);
  };

  struct EnumStringMetricState : public StringMetricState
  {
    static bool classof(const AbstractState* other);
    explicit EnumStringMetricState(DescriptorHandleType handle);
  };

  struct MdState
  {
    using StateType = std::shared_ptr<AbstractState>;
    using StateSequence = std::vector<StateType>;
    StateSequence state;

    using StateVersionType = unsigned int;
    using StateVersionOptional = boost::optional<StateVersionType>;
    StateVersionOptional state_version;
  };

  struct MdibVersionGroup
  {
    using MdibVersionType = unsigned long;
    using MdibVersionOptional = boost::optional<MdibVersionType>;
    MdibVersionOptional mdib_version;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType sequence_id;

    using InstanceIdType = unsigned long;
    using InstanceIdOptional = boost::optional<InstanceIdType>;
    InstanceIdOptional instance_id;

    explicit MdibVersionGroup(SequenceIdType sequence_id);
  };

  struct Mdib
  {
    using MdDescriptionType = ::BICEPS::PM::MdDescription;
    using MdDescriptionOptional = boost::optional<MdDescriptionType>;
    MdDescriptionOptional md_description;

    using MdStateType = ::BICEPS::PM::MdState;
    using MdStateOptional = boost::optional<MdStateType>;
    MdStateOptional md_state;

    MdibVersionGroup mdib_version_group;

    explicit Mdib(MdibVersionGroup mdib_version_group);
  };
} // namespace BICEPS::PM
