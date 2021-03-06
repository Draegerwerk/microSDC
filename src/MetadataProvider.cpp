#include "MetadataProvider.hpp"
#include "SDCConstants.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "networking/NetworkConfig.hpp"

MetadataProvider::MetadataProvider(std::shared_ptr<const NetworkConfig> networkConfig,
                                   DeviceCharacteristics devChar)
  : networkConfig_(std::move(networkConfig))
  , deviceCharacteristics_(std::move(devChar))
{
}

std::string MetadataProvider::getDeviceServicePath()
{
  return std::string("/MicroSDC");
}

std::string MetadataProvider::getGetServicePath()
{
  return std::string("/MicroSDC/GetService");
}

std::string MetadataProvider::getSetServicePath()
{
  return std::string("/MicroSDC/SetService");
}

std::string MetadataProvider::getStateEventServicePath()
{
  return std::string("/MicroSDC/StateEventService");
}

WS::ADDRESSING::URIType MetadataProvider::getSetServiceURI() const
{
  // Endpoint Reference
  const std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
  const std::string xaddress = protocol + "://" + networkConfig_->ipAddress() + ":" +
                               std::to_string(networkConfig_->port()) + getSetServicePath();
  return WS::ADDRESSING::URIType(xaddress);
}

WS::ADDRESSING::URIType MetadataProvider::getStateEventServiceURI() const
{
  // Endpoint Reference
  const std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
  const std::string xaddress = protocol + "://" + networkConfig_->ipAddress() + ":" +
                               std::to_string(networkConfig_->port()) + getStateEventServicePath();
  return WS::ADDRESSING::URIType(xaddress);
}

void MetadataProvider::fillDeviceMetadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(createMetadataSectionThisModel());
  metadata->metadataSection.emplace_back(createMetadataSectionThisDevice());
  metadata->metadataSection.emplace_back(createMetadataSectionRelationship(
      createHostMetadata(),
      {createHostedGetService(), createHostedSetService(), createHostedStateEventService()}));
}

void MetadataProvider::fillGetServiceMetadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(createMetadataSectionWSDLGetService());
  metadata->metadataSection.emplace_back(
      createMetadataSectionRelationship(createHostMetadata(), {createHostedGetService()}));
}

void MetadataProvider::fillSetServiceMetadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(createMetadataSectionWSDLSetService());
  metadata->metadataSection.emplace_back(
      createMetadataSectionRelationship(createHostMetadata(), {createHostedSetService()}));
}

void MetadataProvider::fillStateEventServiceMetadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(createMetadataSectionWSDLStateEventService());
  metadata->metadataSection.emplace_back(
      createMetadataSectionRelationship(createHostMetadata(), {createHostedStateEventService()}));
}

MetadataProvider::MetadataSection MetadataProvider::createMetadataSectionThisModel() const
{
  MetadataSection metadata = MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_MODEL));
  auto& thisModel = metadata.thisModel = WS::DPWS::ThisModelType();
  WS::DPWS::ThisModelType::ManufacturerType manufacturer(deviceCharacteristics_.getManufacturer());
  thisModel->manufacturer.emplace_back(manufacturer);
  thisModel->modelName.emplace_back(deviceCharacteristics_.getModelName());
  return metadata;
}

MetadataProvider::MetadataSection MetadataProvider::createMetadataSectionThisDevice() const
{
  MetadataSection metadata = MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_DEVICE));
  auto& thisDevice = metadata.thisDevice = WS::DPWS::ThisDeviceType();
  auto friendlyName = deviceCharacteristics_.getFriendlyName();
  thisDevice->friendlyName.emplace_back(friendlyName);
  return metadata;
}

MetadataProvider::Host MetadataProvider::createHostMetadata() const
{
  Host host(WS::ADDRESSING::EndpointReferenceType(
      WS::ADDRESSING::URIType(deviceCharacteristics_.getEndpointReference())));
  host.types = Host::TypesType();
  host.types->emplace_back(MDPWS::WS_NS_DPWS_PREFIX, "Device");
  host.types->emplace_back(MDPWS::NS_MDPWS_PREFIX, "MedicalDevice");
  return host;
}

MetadataProvider::MetadataSection
MetadataProvider::createMetadataSectionRelationship(const Host& host, const HostedSequence& hosted)
{
  MetadataSection metadata = MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_REL));
  metadata.relationship = WS::DPWS::Relationship(host, hosted, MDPWS::WS_MEX_REL_HOST);
  return metadata;
}

MetadataProvider::Hosted MetadataProvider::createHostedGetService() const
{
  // Endpoint Reference
  const std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
  const std::string xaddress = protocol + "://" + networkConfig_->ipAddress() + ":" +
                               std::to_string(networkConfig_->port()) + getGetServicePath();
  Hosted::EndpointReferenceSequence endpointReference;
  endpointReference.emplace_back(Hosted::EndpointReferenceType::AddressType(xaddress));
  // Types
  Hosted::TypesType types;
  types.emplace_back(SDC::NS_GLUE_PREFIX, SDC::QNAME_GETSERVICE);
  // Service Id
  Hosted::ServiceIdType serviceId("GetService");
  return Hosted(endpointReference, types, serviceId);
}

MetadataProvider::MetadataSection MetadataProvider::createMetadataSectionWSDLGetService() const
{
  MetadataSection wsdlSection =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_WSDL));
  const std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
  wsdlSection.location = MetadataSection::LocationType(
      protocol + "://" + networkConfig_->ipAddress() + ":" +
      std::to_string(networkConfig_->port()) + getGetServicePath() + "/wsdl");
  return wsdlSection;
}

MetadataProvider::Hosted MetadataProvider::createHostedSetService() const
{
  // Endpoint Reference
  Hosted::EndpointReferenceSequence endpointReference;
  endpointReference.emplace_back(Hosted::EndpointReferenceType::AddressType(getSetServiceURI()));
  // Types
  Hosted::TypesType types;
  types.emplace_back(SDC::NS_GLUE_PREFIX, SDC::QNAME_SETSERVICE);
  // Service Id
  Hosted::ServiceIdType serviceId("SetService");
  return Hosted(endpointReference, types, serviceId);
}

MetadataProvider::MetadataSection MetadataProvider::createMetadataSectionWSDLSetService() const
{
  MetadataSection wsdlSection =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_WSDL));
  const std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
  wsdlSection.location = MetadataSection::LocationType(
      protocol + "://" + networkConfig_->ipAddress() + ":" +
      std::to_string(networkConfig_->port()) + getSetServicePath() + "/wsdl");
  return wsdlSection;
}

MetadataProvider::Hosted MetadataProvider::createHostedStateEventService() const
{
  // Endpoint Reference
  Hosted::EndpointReferenceSequence endpointReference;
  endpointReference.emplace_back(
      Hosted::EndpointReferenceType::AddressType(getStateEventServiceURI()));
  // Types
  Hosted::TypesType types;
  types.emplace_back(SDC::NS_GLUE_PREFIX, SDC::QNAME_STATEEVENTSERVICE);
  // Service Id
  Hosted::ServiceIdType serviceId("StateEventService");
  return Hosted(endpointReference, types, serviceId);
}

MetadataProvider::MetadataSection
MetadataProvider::createMetadataSectionWSDLStateEventService() const
{
  MetadataSection wsdlSection =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_WSDL));
  const std::string protocol = networkConfig_->isUsingTLS() ? "https" : "http";
  wsdlSection.location = MetadataSection::LocationType(
      protocol + "://" + networkConfig_->ipAddress() + ":" +
      std::to_string(networkConfig_->port()) + getStateEventServicePath() + "/wsdl");
  return wsdlSection;
}
