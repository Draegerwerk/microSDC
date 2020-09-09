#include "ws-dpws.hpp"

namespace WS::DPWS
{
  HostServiceType::HostServiceType(EndpointReferenceType epr)
    : EndpointReference(std::move(epr))
  {
  }

  HostedServiceType::HostedServiceType(EndpointReferenceSequence epr, TypesType types,
                                       ServiceIdType serviceId)
    : EndpointReference(std::move(epr))
    , Types(std::move(types))
    , ServiceId(std::move(serviceId))
  {
  }

  Relationship::Relationship(HostServiceType host, HostedSequence hosted, TypeType type)
    : Host(std::move(host))
    , Hosted(std::move(hosted))
    , Type(std::move(type))
  {
  }

} // namespace WS::DPWS
