#include "ws-dpws.hpp"

namespace WS::DPWS
{
  HostServiceType::HostServiceType(EndpointReferenceType epr)
    : endpoint_reference(std::move(epr))
  {
  }

  HostedServiceType::HostedServiceType(EndpointReferenceSequence epr, TypesType types,
                                       ServiceIdType service_id)
    : endpoint_reference(std::move(epr))
    , types(std::move(types))
    , service_id(std::move(service_id))
  {
  }

  Relationship::Relationship(HostServiceType host, HostedSequence hosted, TypeType type)
    : host(std::move(host))
    , hosted(std::move(hosted))
    , type(std::move(type))
  {
  }

} // namespace WS::DPWS
