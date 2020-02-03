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
  };
  class AbstractComplexDeviceComponentDescriptor : public AbstractDeviceComponentDescriptor
  {
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
  class SystemContextDescriptor
  {
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

    // Constructors.
    //
    AbstractMetricDescriptor(const HandleType&, const UnitType&, const MetricCategoryType&,
                             const MetricAvailabilityType&);

  protected:
    UnitType Unit_;
    MetricCategoryType MetricCategory_;
    MetricAvailabilityType MetricAvailability_;
  };
  class ChannelDescriptor : public AbstractDeviceComponentDescriptor
  {
  public:
    // Metric
    //
    using MetricType = AbstractMetricDescriptor;
    using MetricSequence = std::vector<MetricType>;
    const MetricSequence& Metric() const;
    MetricSequence& Metric();

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
    MdDescription();

  protected:
    MdsSequence Mds_;
    DescriptionVersionOptional DescriptionVersion_;
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

    // Constructors.
    //
    AbstractState(const DescriptorHandleType&);

  protected:
    StateVersionOptional StateVersion_;
    DescriptorHandleType DescriptorHandle_;
  };
  class MdState
  {
  public:
    // State
    //
    using StateType = AbstractState;
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
