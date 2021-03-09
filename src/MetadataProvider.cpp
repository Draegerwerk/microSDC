#include "MetadataProvider.hpp"
#include "SDCConstants.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "networking/NetworkConfig.hpp"

MetadataProvider::MetadataProvider(std::shared_ptr<const NetworkConfig> network_config,
                                   DeviceCharacteristics dev_char)
  : network_config_(std::move(network_config))
  , device_characteristics_(std::move(dev_char))
{
}

std::string MetadataProvider::get_device_service_path()
{
  return std::string("/MicroSDC");
}

std::string MetadataProvider::get_get_service_path()
{
  return std::string("/MicroSDC/GetService");
}

std::string MetadataProvider::get_set_service_path()
{
  return std::string("/MicroSDC/SetService");
}

std::string MetadataProvider::get_state_event_service_path()
{
  return std::string("/MicroSDC/StateEventService");
}

WS::ADDRESSING::URIType MetadataProvider::get_set_service_uri() const
{
  // Endpoint Reference
  const std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  const std::string xaddress = protocol + "://" + network_config_->ip_address() + ":" +
                               std::to_string(network_config_->port()) + get_set_service_path();
  return WS::ADDRESSING::URIType(xaddress);
}

WS::ADDRESSING::URIType MetadataProvider::get_state_event_service_uri() const
{
  // Endpoint Reference
  const std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  const std::string xaddress = protocol + "://" + network_config_->ip_address() + ":" +
                               std::to_string(network_config_->port()) +
                               get_state_event_service_path();
  return WS::ADDRESSING::URIType(xaddress);
}

void MetadataProvider::fill_device_metadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(create_metadata_section_this_model());
  metadata->metadataSection.emplace_back(create_metadata_section_this_device());
  metadata->metadataSection.emplace_back(create_netadata_section_relationship(
      create_host_metadata(), {create_hosted_get_service(), create_hosted_set_service(),
                               create_hosted_state_event_service()}));
}

void MetadataProvider::fill_get_service_metadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(create_metadata_section_wsdl_get_service());
  metadata->metadataSection.emplace_back(
      create_netadata_section_relationship(create_host_metadata(), {create_hosted_get_service()}));
}

void MetadataProvider::fill_set_service_metadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(create_metadata_section_wsdl_set_service());
  metadata->metadataSection.emplace_back(
      create_netadata_section_relationship(create_host_metadata(), {create_hosted_set_service()}));
}

void MetadataProvider::fill_state_event_service_metadata(MESSAGEMODEL::Envelope& envelope) const
{
  auto& metadata = envelope.body.metadata = WS::MEX::Metadata();
  metadata->metadataSection.emplace_back(create_metadata_section_wsdl_state_event_service());
  metadata->metadataSection.emplace_back(create_netadata_section_relationship(
      create_host_metadata(), {create_hosted_state_event_service()}));
}

MetadataProvider::MetadataSection MetadataProvider::create_metadata_section_this_model() const
{
  MetadataSection metadata = MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_MODEL));
  auto& this_model = metadata.thisModel = WS::DPWS::ThisModelType();
  WS::DPWS::ThisModelType::ManufacturerType manufacturer(
      device_characteristics_.get_manufacturer());
  this_model->manufacturer.emplace_back(manufacturer);
  this_model->modelName.emplace_back(device_characteristics_.get_model_name());
  return metadata;
}

MetadataProvider::MetadataSection MetadataProvider::create_metadata_section_this_device() const
{
  MetadataSection metadata = MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_DEVICE));
  auto& this_device = metadata.thisDevice = WS::DPWS::ThisDeviceType();
  auto friendly_name = device_characteristics_.get_friendly_name();
  this_device->friendlyName.emplace_back(friendly_name);
  return metadata;
}

MetadataProvider::Host MetadataProvider::create_host_metadata() const
{
  Host host(WS::ADDRESSING::EndpointReferenceType(
      WS::ADDRESSING::URIType(device_characteristics_.get_endpoint_reference())));
  host.types = Host::TypesType();
  host.types->emplace_back(MDPWS::WS_NS_DPWS_PREFIX, "Device");
  host.types->emplace_back(MDPWS::NS_MDPWS_PREFIX, "MedicalDevice");
  return host;
}

MetadataProvider::MetadataSection
MetadataProvider::create_netadata_section_relationship(const Host& host,
                                                       const HostedSequence& hosted)
{
  MetadataSection metadata = MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_REL));
  metadata.relationship = WS::DPWS::Relationship(host, hosted, MDPWS::WS_MEX_REL_HOST);
  return metadata;
}

MetadataProvider::Hosted MetadataProvider::create_hosted_get_service() const
{
  // Endpoint Reference
  const std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  const std::string xaddress = protocol + "://" + network_config_->ip_address() + ":" +
                               std::to_string(network_config_->port()) + get_get_service_path();
  Hosted::EndpointReferenceSequence endpoint_reference;
  endpoint_reference.emplace_back(Hosted::EndpointReferenceType::AddressType(xaddress));
  // Types
  Hosted::TypesType types;
  types.emplace_back(SDC::NS_GLUE_PREFIX, SDC::QNAME_GETSERVICE);
  // Service Id
  Hosted::ServiceIdType service_id("GetService");
  return Hosted(endpoint_reference, types, service_id);
}

MetadataProvider::MetadataSection MetadataProvider::create_metadata_section_wsdl_get_service() const
{
  MetadataSection wsdl_section =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_WSDL));
  const std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  wsdl_section.location = MetadataSection::LocationType(
      protocol + "://" + network_config_->ip_address() + ":" +
      std::to_string(network_config_->port()) + get_get_service_path() + "/wsdl");
  return wsdl_section;
}

MetadataProvider::Hosted MetadataProvider::create_hosted_set_service() const
{
  // Endpoint Reference
  Hosted::EndpointReferenceSequence endpoint_reference;
  endpoint_reference.emplace_back(
      Hosted::EndpointReferenceType::AddressType(get_set_service_uri()));
  // Types
  Hosted::TypesType types;
  types.emplace_back(SDC::NS_GLUE_PREFIX, SDC::QNAME_SETSERVICE);
  // Service Id
  Hosted::ServiceIdType service_id("SetService");
  return Hosted(endpoint_reference, types, service_id);
}

MetadataProvider::MetadataSection MetadataProvider::create_metadata_section_wsdl_set_service() const
{
  MetadataSection wsdl_section =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_WSDL));
  const std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  wsdl_section.location = MetadataSection::LocationType(
      protocol + "://" + network_config_->ip_address() + ":" +
      std::to_string(network_config_->port()) + get_set_service_path() + "/wsdl");
  return wsdl_section;
}

MetadataProvider::Hosted MetadataProvider::create_hosted_state_event_service() const
{
  // Endpoint Reference
  Hosted::EndpointReferenceSequence endpoint_reference;
  endpoint_reference.emplace_back(
      Hosted::EndpointReferenceType::AddressType(get_state_event_service_uri()));
  // Types
  Hosted::TypesType types;
  types.emplace_back(SDC::NS_GLUE_PREFIX, SDC::QNAME_STATEEVENTSERVICE);
  // Service Id
  Hosted::ServiceIdType service_id("StateEventService");
  return Hosted(endpoint_reference, types, service_id);
}

MetadataProvider::MetadataSection
MetadataProvider::create_metadata_section_wsdl_state_event_service() const
{
  MetadataSection wsdl_section =
      MetadataSection(WS::ADDRESSING::URIType(MDPWS::WS_MEX_DIALECT_WSDL));
  const std::string protocol = network_config_->is_using_tls() ? "https" : "http";
  wsdl_section.location = MetadataSection::LocationType(
      protocol + "://" + network_config_->ip_address() + ":" +
      std::to_string(network_config_->port()) + get_state_event_service_path() + "/wsdl");
  return wsdl_section;
}
