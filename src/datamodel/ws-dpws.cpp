#include "ws-dpws.hpp"

namespace WS::DPWS
{
  HostServiceType::HostServiceType(EndpointReferenceType epr)
    : endpointReference(std::move(epr))
  {
  }

  HostedServiceType::HostedServiceType(EndpointReferenceSequence epr, TypesType types,
                                       ServiceIdType serviceId)
    : endpointReference(std::move(epr))
    , types(std::move(types))
    , serviceId(std::move(serviceId))
  {
  }

  Relationship::Relationship(HostServiceType host, HostedSequence hosted, TypeType type)
    : host(std::move(host))
    , hosted(std::move(hosted))
    , type(std::move(type))
  {
  }

} // namespace WS::DPWS
