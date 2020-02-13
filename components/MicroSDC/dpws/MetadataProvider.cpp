#include "dpws/MetadataProvider.hpp"
#include "NetworkHandler.hpp"
#include "SDCConstants.hpp"
#include "datamodel/MDPWSConstants.hpp"

MetadataProvider::MetadataProvider(const DeviceCharacteristics& devChar, const bool useTLS)
  : deviceCharacteristics_(devChar)
  , useTLS(useTLS)
{
}

std::string MetadataProvider::getDeviceServicePath() const
{
  return std::string("/MicroSDC");
}

std::string MetadataProvider::getGetServicePath() const
{
  return std::string("/MicroSDC/GetService");
}

void MetadataProvider::fillDeviceMetadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.Body().Metadata() = WS::MEX::Metadata();
  metadata->MetadataSection().emplace_back(createMetadataSectionThisModel());
  metadata->MetadataSection().emplace_back(createMetadataSectionThisDevice());
  metadata->MetadataSection().emplace_back(
      createMetadataSectionRelationship(createHostMetadata(), {createHostedGetService()}));
}

void MetadataProvider::fillGetServiceMetadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.Body().Metadata() = WS::MEX::Metadata();
  metadata->MetadataSection().emplace_back(createMetadataSectionWSDLGetService());
  metadata->MetadataSection().emplace_back(
      createMetadataSectionRelationship(createHostMetadata(), {createHostedGetService()}));
}

MetadataProvider::MetadataSection MetadataProvider::createMetadataSectionThisModel() const
{
  MetadataSection metadata =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_MODEL));
  auto& thisModel = metadata.ThisModel() = WS::DPWS::ThisModelType();
  WS::DPWS::ThisModelType::ManufacturerType manufacturer(deviceCharacteristics_.getManufacturer());
  thisModel->Manufacturer().emplace_back(manufacturer);
  thisModel->ModelName().emplace_back(deviceCharacteristics_.getModelName());
  return metadata;
}

MetadataProvider::MetadataSection MetadataProvider::createMetadataSectionThisDevice() const
{
  MetadataSection metadata =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_DEVICE));
  auto& thisDevice = metadata.ThisDevice() = WS::DPWS::ThisDeviceType();
  auto friendlyName = deviceCharacteristics_.getFriendlyName();
  thisDevice->FriendlyName().emplace_back(friendlyName);
  return metadata;
}

MetadataProvider::Host MetadataProvider::createHostMetadata() const
{
  Host host(WS::ADDRESSING::EndpointReferenceType(deviceCharacteristics_.getEndpointReference()));
  host.Types() = Host::TypesType();
  host.Types()->emplace_back(MDPWS::WS_NS_DPWS, "Device");
  host.Types()->emplace_back(MDPWS::NS_MDPWS, "MedicalDevice");
  return host;
}

MetadataProvider::MetadataSection
MetadataProvider::createMetadataSectionRelationship(const Host& host,
                                                    const HostedSequence& hosted) const
{
  MetadataSection metadata =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_REL));
  metadata.Relationship() = WS::DPWS::Relationship(host, hosted, MDPWS::WS_MEX_REL_HOST);
  return metadata;
}

MetadataProvider::Hosted MetadataProvider::createHostedGetService() const
{
  // Endpoint Reference
  const std::string protocol = useTLS ? "https" : "http";
  const std::string xaddress = protocol + "://" + NetworkHandler::getInstance().address() +
                               (useTLS ? ":443" : ":80") + getGetServicePath();
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
  const std::string protocol = useTLS ? "https" : "http";
  wsdlSection.Location() =
      MetadataSection::LocationType(protocol + "://" + NetworkHandler::getInstance().address() +
                                    (useTLS ? ":443" : ":80") + getGetServicePath() + "/?wsdl");
  return wsdlSection;
}
