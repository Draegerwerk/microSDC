#include "ws-dpws.hpp"

namespace WS::DPWS
{
  const ThisModelType::ManufacturerSequence& ThisModelType::Manufacturer() const
  {
    return Manufacturer_;
  }
  ThisModelType::ManufacturerSequence& ThisModelType::Manufacturer()
  {
    return Manufacturer_;
  }

  const ThisModelType::ModelNameSequence& ThisModelType::ModelName() const
  {
    return ModelName_;
  }
  ThisModelType::ModelNameSequence& ThisModelType::ModelName()
  {
    return ModelName_;
  }

  const ThisDeviceType::FriendlyNameSequence& ThisDeviceType::FriendlyName() const
  {
    return FriendlyName_;
  }
  ThisDeviceType::FriendlyNameSequence& ThisDeviceType::FriendlyName()
  {
    return FriendlyName_;
  }

  const HostServiceType::EndpointReferenceType& HostServiceType::EndpointReference() const
  {
    return EndpointReference_;
  }
  HostServiceType::EndpointReferenceType& HostServiceType::EndpointReference()
  {
    return EndpointReference_;
  }

  const HostServiceType::TypesOptional& HostServiceType::Types() const
  {
    return Types_;
  }
  HostServiceType::TypesOptional& HostServiceType::Types()
  {
    return Types_;
  }

  HostServiceType::HostServiceType(EndpointReferenceType epr)
    : EndpointReference_(std::move(epr))
  {
  }

  const HostedServiceType::EndpointReferenceSequence& HostedServiceType::EndpointReference() const
  {
    return EndpointReference_;
  }
  HostedServiceType::EndpointReferenceSequence& HostedServiceType::EndpointReference()
  {
    return EndpointReference_;
  }

  const HostedServiceType::TypesType& HostedServiceType::Types() const
  {
    return Types_;
  }
  HostedServiceType::TypesType& HostedServiceType::Types()
  {
    return Types_;
  }

  const HostedServiceType::ServiceIdType& HostedServiceType::ServiceId() const
  {
    return ServiceId_;
  }
  HostedServiceType::ServiceIdType& HostedServiceType::ServiceId()
  {
    return ServiceId_;
  }

  HostedServiceType::HostedServiceType(EndpointReferenceSequence epr, TypesType types,
                                       ServiceIdType serviceId)
    : EndpointReference_(std::move(epr))
    , Types_(std::move(types))
    , ServiceId_(std::move(serviceId))
  {
  }


  const Relationship::HostType& Relationship::Host() const
  {
    return Host_;
  }
  Relationship::HostType& Relationship::Host()
  {
    return Host_;
  }

  const Relationship::HostedSequence& Relationship::Hosted() const
  {
    return Hosted_;
  }
  Relationship::HostedSequence& Relationship::Hosted()
  {
    return Hosted_;
  }

  const Relationship::TypeType& Relationship::Type() const
  {
    return Type_;
  }
  Relationship::TypeType& Relationship::Type()
  {
    return Type_;
  }

  Relationship::Relationship(HostServiceType host, HostedSequence hosted, TypeType type)
    : Host_(std::move(host))
    , Hosted_(std::move(hosted))
    , Type_(std::move(type))
  {
  }

} // namespace WS::DPWS
