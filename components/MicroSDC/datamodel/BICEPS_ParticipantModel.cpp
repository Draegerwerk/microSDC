#include "BICEPS_ParticipantModel.hpp"

namespace BICEPS::PM
{

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
  AbstractDescriptor::AbstractDescriptor(const HandleType& handle)
    : Handle_(handle)
  {
  }
  const AbstractDescriptor::HandleType& AbstractDescriptor::Handle() const
  {
    return Handle_;
  }
  AbstractDescriptor::HandleType& AbstractDescriptor::Handle()
  {
    return Handle_;
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

  // AbstractContextDescriptor
  //
  AbstractContextDescriptor::AbstractContextDescriptor(const HandleType& handle)
    : AbstractDescriptor(handle)
  {
  }

  // PatientContext
  //
  PatientContextDescriptor::PatientContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(handle)
  {
  }

  // SystemContext
  //
  SystemContextDescriptor::SystemContextDescriptor(const HandleType& handle)
    : AbstractContextDescriptor(handle)
  {
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

  // AbstractDeviceComponentDescriptor
  //
  AbstractDeviceComponentDescriptor::AbstractDeviceComponentDescriptor(const HandleType& handle)
    : AbstractDescriptor(handle)
  {
  }

  // ChannelDescriptor
  //
  ChannelDescriptor::ChannelDescriptor(const HandleType& handle)
    : AbstractDeviceComponentDescriptor(handle)
  {
  }

  // AbstractComplexDeviceComponentDescriptor
  //
  AbstractComplexDeviceComponentDescriptor::AbstractComplexDeviceComponentDescriptor(
      const HandleType& handle)
    : AbstractDeviceComponentDescriptor(handle)
  {
  }

  // VmdDescriptor
  //
  VmdDescriptor::VmdDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(handle)
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

  // MdsDescriptor
  //
  MdsDescriptor::MdsDescriptor(const HandleType& handle)
    : AbstractComplexDeviceComponentDescriptor(handle)
  {
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

  // Mdib
  //
  Mdib::Mdib(const SequenceIdType& sequenceIdType)
    : SequenceId_(sequenceIdType)
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
