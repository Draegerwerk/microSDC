#include "MessageSerializer.hpp"
#include "Casting.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "rapidxml_print.hpp"

MessageSerializer::MessageSerializer()
  : xml_document_(std::make_unique<rapidxml::xml_document<>>())
{
  auto* declaration = xml_document_->allocate_node(rapidxml::node_declaration);
  auto* version = xml_document_->allocate_attribute("version", "1.0");
  auto* encoding = xml_document_->allocate_attribute("encoding", "utf-8");
  declaration->append_attribute(version);
  declaration->append_attribute(encoding);
  xml_document_->append_node(declaration);
}

std::string MessageSerializer::str() const
{
  std::string out;
  rapidxml::print(std::back_inserter(out), *xml_document_, rapidxml::print_no_indenting);
  return out;
}

void MessageSerializer::serialize(const MESSAGEMODEL::Envelope& message)
{
  serialize(&*xml_document_, message);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const MESSAGEMODEL::Envelope& message)
{
  auto* envelope = xml_document_->allocate_node(rapidxml::node_element, "soap:Envelope");

  auto* xmlns_soap = xml_document_->allocate_attribute("xmlns:soap", MDPWS::WS_NS_SOAP_ENVELOPE);
  auto* xmlns_wsd = xml_document_->allocate_attribute("xmlns:wsd", MDPWS::WS_NS_DISCOVERY);
  auto* xmlns_wsa = xml_document_->allocate_attribute("xmlns:wsa", MDPWS::WS_NS_ADDRESSING);
  auto* xmlns_wse = xml_document_->allocate_attribute("xmlns:wse", MDPWS::WS_NS_EVENTING);
  auto* xmlns_dpws = xml_document_->allocate_attribute("xmlns:dpws", MDPWS::WS_NS_DPWS);
  auto* xmlns_mdpws = xml_document_->allocate_attribute("xmlns:mdpws", MDPWS::NS_MDPWS);
  auto* xmlns_mex = xml_document_->allocate_attribute("xmlns:mex", MDPWS::WS_NS_METADATA_EXCHANGE);
  auto* xmlns_glue = xml_document_->allocate_attribute("xmlns:glue", SDC::NS_GLUE);
  auto* xmlns_mm = xml_document_->allocate_attribute("xmlns:mm", SDC::NS_BICEPS_MESSAGE_MODEL);
  auto* xmlns_pm = xml_document_->allocate_attribute("xmlns:pm", SDC::NS_BICEPS_PARTICIPANT_MODEL);
  auto* xmlns_ext = xml_document_->allocate_attribute("xmlns:ext", SDC::NS_BICEPS_EXTENSION);
  auto* xmlns_xsi =
      xml_document_->allocate_attribute("xmlns:xsi", MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE);

  envelope->append_attribute(xmlns_soap);
  envelope->append_attribute(xmlns_wsd);
  envelope->append_attribute(xmlns_wsa);
  envelope->append_attribute(xmlns_wse);
  envelope->append_attribute(xmlns_dpws);
  envelope->append_attribute(xmlns_mdpws);
  envelope->append_attribute(xmlns_mex);
  envelope->append_attribute(xmlns_glue);
  envelope->append_attribute(xmlns_mm);
  envelope->append_attribute(xmlns_pm);
  envelope->append_attribute(xmlns_ext);
  envelope->append_attribute(xmlns_xsi);

  serialize(envelope, message.header);
  serialize(envelope, message.body);

  parent->append_node(envelope);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Header& header)
{
  auto* header_node = xml_document_->allocate_node(rapidxml::node_element, "soap:Header");
  // Mandatory action element
  auto* action_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:Action");
  action_node->value(header.action.c_str());
  header_node->append_node(action_node);
  // optionals
  if (header.messageID.has_value())
  {
    auto* message_id_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:MessageID");
    message_id_node->value(header.messageID->c_str());
    header_node->append_node(message_id_node);
  }
  if (header.to.has_value())
  {
    auto* to_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:To");
    to_node->value(header.to->c_str());
    header_node->append_node(to_node);
  }
  if (header.appSequence.has_value())
  {
    serialize(header_node, header.appSequence.value());
  }
  if (header.relatesTo.has_value())
  {
    serialize(header_node, header.relatesTo.value());
  }
  parent->append_node(header_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Body& body)
{
  auto* body_node = xml_document_->allocate_node(rapidxml::node_element, "soap:Body");
  if (body.hello.has_value())
  {
    serialize(body_node, body.hello.value());
  }
  else if (body.bye.has_value())
  {
    serialize(body_node, body.bye.value());
  }
  else if (body.probeMatches.has_value())
  {
    serialize(body_node, body.probeMatches.value());
  }
  else if (body.resolveMatches.has_value())
  {
    serialize(body_node, body.resolveMatches.value());
  }
  else if (body.metadata.has_value())
  {
    serialize(body_node, body.metadata.value());
  }
  else if (body.getMdibResponse.has_value())
  {
    serialize(body_node, body.getMdibResponse.value());
  }
  else if (body.subscribeResponse.has_value())
  {
    serialize(body_node, body.subscribeResponse.value());
  }
  else if (body.renewResponse.has_value())
  {
    serialize(body_node, body.renewResponse.value());
  }
  else if (body.episodicMetricReport.has_value())
  {
    serialize(body_node, body.episodicMetricReport.value());
  }
  else if (body.setValueResponse.has_value())
  {
    serialize(body_node, body.setValueResponse.value());
  }
  else if (body.setStringResponse.has_value())
  {
    serialize(body_node, body.setStringResponse.value());
  }
  parent->append_node(body_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::RelatesToType& relates_to)
{
  auto* relates_to_node = xml_document_->allocate_node(rapidxml::node_element, "mdpws:RelatesTo");
  relates_to_node->value(relates_to.c_str());
  parent->append_node(relates_to_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::EndpointReferenceType& endpoint_reference)
{
  auto* epr_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:EndpointReference");
  auto* address_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:Address");
  address_node->value(endpoint_reference.address.c_str());
  epr_node->append_node(address_node);
  parent->append_node(epr_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::AppSequenceType& app_sequence)
{
  auto* app_sequence_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:AppSequence");
  const auto instance_id_str = std::to_string(app_sequence.instanceId);
  auto* instance_id = xml_document_->allocate_string(instance_id_str.c_str());
  auto* instance_id_attr = xml_document_->allocate_attribute("InstanceId", instance_id);
  app_sequence_node->append_attribute(instance_id_attr);
  if (app_sequence.sequenceId.has_value())
  {
    auto* sequence_id = xml_document_->allocate_string(app_sequence.sequenceId->c_str());
    auto* sequence_id_attr = xml_document_->allocate_attribute("SequenceId", sequence_id);
    app_sequence_node->append_attribute(sequence_id_attr);
  }
  auto message_number_str = std::to_string(app_sequence.messageNumber);
  auto* message_number = xml_document_->allocate_string(message_number_str.c_str());
  auto* message_number_attr = xml_document_->allocate_attribute("MessageNumber", message_number);
  app_sequence_node->append_attribute(message_number_attr);
  parent->append_node(app_sequence_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ScopesType& scopes)
{
  auto* scopes_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Scopes");
  if (scopes.matchBy.has_value())
  {
    auto* match_by_attr = xml_document_->allocate_attribute("MatchBy", scopes.matchBy->c_str());
    scopes_node->append_attribute(match_by_attr);
  }
  const auto scopes_str = to_string(scopes);
  auto* uri_list = xml_document_->allocate_string(scopes_str.c_str());
  scopes_node->value(uri_list);
  parent->append_node(scopes_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::HelloType& hello)
{
  auto* hello_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Hello");
  serialize(hello_node, hello.endpointReference);
  if (hello.types.has_value())
  {
    auto* types_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* types_str = xml_document_->allocate_string(to_string(hello.types.value()).c_str());
    types_node->value(types_str);
    hello_node->append_node(types_node);
  }
  if (hello.scopes.has_value())
  {
    serialize(hello_node, hello.scopes.value());
  }
  if (hello.xAddrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str = xml_document_->allocate_string(to_string(hello.xAddrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    hello_node->append_node(x_addrs_node);
  }
  auto* metadata_version_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadata_version =
      xml_document_->allocate_string(std::to_string(hello.metadataVersion).c_str());
  metadata_version_node->value(metadata_version);
  parent->append_node(hello_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ByeType& bye)
{
  auto* bye_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:bye");
  serialize(bye_node, bye.endpointReference);
  if (bye.types.has_value())
  {
    auto* types_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* types_str = xml_document_->allocate_string(to_string(bye.types.value()).c_str());
    types_node->value(types_str);
    bye_node->append_node(types_node);
  }
  if (bye.scopes.has_value())
  {
    serialize(bye_node, bye.scopes.value());
  }
  if (bye.xAddrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str = xml_document_->allocate_string(to_string(bye.xAddrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    bye_node->append_node(x_addrs_node);
  }
  if (bye.metadataVersion.has_value())
  {
    auto* metadata_version_node =
        xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
    auto* metadata_version =
        xml_document_->allocate_string(std::to_string(bye.metadataVersion.value()).c_str());
    metadata_version_node->value(metadata_version);
  }
  parent->append_node(bye_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchType& probe_match)
{
  auto* probe_match_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:ProbeMatch");
  serialize(probe_match_node, probe_match.endpointReference);
  if (probe_match.types.has_value())
  {
    auto* types_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* types_str = xml_document_->allocate_string(to_string(probe_match.types.value()).c_str());
    types_node->value(types_str);
    probe_match_node->append_node(types_node);
  }
  if (probe_match.scopes.has_value())
  {
    serialize(probe_match_node, probe_match.scopes.value());
  }
  if (probe_match.xAddrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str =
        xml_document_->allocate_string(to_string(probe_match.xAddrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    probe_match_node->append_node(x_addrs_node);
  }
  auto* metadata_version_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadata_version =
      xml_document_->allocate_string(std::to_string(probe_match.metadataVersion).c_str());
  metadata_version_node->value(metadata_version);
  probe_match_node->append_node(metadata_version_node);
  parent->append_node(probe_match_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchesType& probe_matches)
{
  auto* probe_matches_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:ProbeMatches");
  for (const auto& probe_match : probe_matches.probeMatch)
  {
    serialize(probe_matches_node, probe_match);
  }
  parent->append_node(probe_matches_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchType& resolve_match)
{
  auto* resolve_match_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  serialize(resolve_match_node, resolve_match.endpointReference);
  if (resolve_match.types.has_value())
  {
    auto* types_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* types_str =
        xml_document_->allocate_string(to_string(resolve_match.types.value()).c_str());
    types_node->value(types_str);
    resolve_match_node->append_node(types_node);
  }
  if (resolve_match.scopes.has_value())
  {
    serialize(resolve_match_node, resolve_match.scopes.value());
  }
  if (resolve_match.xAddrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str =
        xml_document_->allocate_string(to_string(resolve_match.xAddrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    resolve_match_node->append_node(x_addrs_node);
  }
  auto* metadata_version_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadata_version =
      xml_document_->allocate_string(std::to_string(resolve_match.metadataVersion).c_str());
  metadata_version_node->value(metadata_version);
  resolve_match_node->append_node(metadata_version_node);
  parent->append_node(resolve_match_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchesType& resolve_matches)
{
  auto* resolve_matches_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  for (const auto& resolve_match : resolve_matches.resolveMatch)
  {
    serialize(resolve_matches_node, resolve_match);
  }
  parent->append_node(resolve_matches_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::MEX::Metadata& metadata)
{
  auto* metadata_node = xml_document_->allocate_node(rapidxml::node_element, "mex:Metadata");
  for (const auto& metadata_section : metadata.metadataSection)
  {
    serialize(metadata_node, metadata_section);
  }
  parent->append_node(metadata_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::MEX::MetadataSection& metadata_section)
{
  auto* metadata_section_node =
      xml_document_->allocate_node(rapidxml::node_element, "mex:MetadataSection");
  auto* dialect_attr =
      xml_document_->allocate_attribute("Dialect", metadata_section.dialect.c_str());
  metadata_section_node->append_attribute(dialect_attr);
  if (metadata_section.thisModel.has_value())
  {
    serialize(metadata_section_node, metadata_section.thisModel.value());
  }
  else if (metadata_section.thisDevice.has_value())
  {
    serialize(metadata_section_node, metadata_section.thisDevice.value());
  }
  else if (metadata_section.relationship.has_value())
  {
    serialize(metadata_section_node, metadata_section.relationship.value());
  }
  else if (metadata_section.location.has_value())
  {
    auto* location_node = xml_document_->allocate_node(rapidxml::node_element, "mex:Location");
    location_node->value(metadata_section.location->c_str());
    metadata_section_node->append_node(location_node);
  }
  parent->append_node(metadata_section_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::ThisModelType& this_model)
{
  auto* this_model_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:ThisModel");
  // Manufacturer
  for (const auto& manufacturer : this_model.manufacturer)
  {
    auto* manufacturer_node =
        xml_document_->allocate_node(rapidxml::node_element, "dpws:Manufacturer");
    manufacturer_node->value(manufacturer.c_str());
    this_model_node->append_node(manufacturer_node);
  }
  // ModelName
  for (const auto& model_name : this_model.modelName)
  {
    auto* model_name_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:ModelName");
    model_name_node->value(model_name.c_str());
    this_model_node->append_node(model_name_node);
  }
  parent->append_node(this_model_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::ThisDeviceType& this_device)
{
  auto* this_device_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:ThisDevice");
  // FriendlyName
  for (const auto& friendly_name : this_device.friendlyName)
  {
    auto* friendly_name_node =
        xml_document_->allocate_node(rapidxml::node_element, "dpws:FriendlyName");
    friendly_name_node->value(friendly_name.c_str());
    this_device_node->append_node(friendly_name_node);
  }
  parent->append_node(this_device_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::Relationship& relationship)
{
  auto* relationship_node =
      xml_document_->allocate_node(rapidxml::node_element, "dpws:Relationship");
  auto* type_attr = xml_document_->allocate_attribute("Type", relationship.type.c_str());
  relationship_node->append_attribute(type_attr);

  serialize(relationship_node, relationship.host);
  for (const auto& hosted : relationship.hosted)
  {
    serialize(relationship_node, hosted);
  }
  parent->append_node(relationship_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::HostServiceType& host)
{
  auto* host_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:Host");
  serialize(host_node, host.endpointReference);
  if (host.types.has_value())
  {
    auto* types_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:Types");
    auto* types_str = xml_document_->allocate_string(to_string(host.types.value()).c_str());
    types_node->value(types_str);
    host_node->append_node(types_node);
  }
  parent->append_node(host_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::HostedServiceType& hosted)
{
  auto* hosted_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:Hosted");
  for (const auto& epr : hosted.endpointReference)
  {
    serialize(hosted_node, epr);
  }
  // Types
  auto* types_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:Types");
  auto* types_str = xml_document_->allocate_string(to_string(hosted.types).c_str());
  types_node->value(types_str);
  hosted_node->append_node(types_node);
  // ServiceId
  auto* service_id_node = xml_document_->allocate_node(rapidxml::node_element, "dpws:ServiceId");
  service_id_node->value(hosted.serviceId.c_str());
  hosted_node->append_node(service_id_node);
  parent->append_node(hosted_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::GetMdibResponse& get_mdib_response)
{
  auto* get_mdib_response_node =
      xml_document_->allocate_node(rapidxml::node_element, "mm:GetMdibResponse");
  auto* sequence_id_attr = xml_document_->allocate_attribute("SequenceId", "0");
  get_mdib_response_node->append_attribute(sequence_id_attr);
  serialize(get_mdib_response_node, get_mdib_response.mdib);
  parent->append_node(get_mdib_response_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Mdib& mdib)
{
  auto* mdib_node = xml_document_->allocate_node(rapidxml::node_element, "mm:Mdib");
  auto* sequence_id = xml_document_->allocate_string(mdib.sequenceId.c_str());
  auto* sequence_id_attr = xml_document_->allocate_attribute("SequenceId", sequence_id);
  mdib_node->append_attribute(sequence_id_attr);
  auto* mdib_version =
      xml_document_->allocate_string(std::to_string(mdib.mdibVersion.value_or(0)).c_str());
  auto* mdib_version_attr = xml_document_->allocate_attribute(mdib_version);
  mdib_node->append_attribute(mdib_version_attr);
  if (mdib.mdDescription.has_value())
  {
    serialize(mdib_node, mdib.mdDescription.value());
  }
  if (mdib.mdState.has_value())
  {
    serialize(mdib_node, mdib.mdState.value());
  }
  parent->append_node(mdib_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdDescription& md_description)
{
  auto* md_description_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:MdDescription");
  for (const auto& md : md_description.mds)
  {
    serialize(md_description_node, md);
  }
  parent->append_node(md_description_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdsDescriptor& mds_descriptor)
{
  auto* mds_descriptor_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Mds");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", mds_descriptor.handle.c_str());
  mds_descriptor_node->append_attribute(handle_attr);
  if (mds_descriptor.metaData.has_value())
  {
    serialize(mds_descriptor_node, mds_descriptor.metaData.value());
  }
  if (mds_descriptor.systemContext.has_value())
  {
    serialize(mds_descriptor_node, mds_descriptor.systemContext.value());
  }
  for (const auto& vmd : mds_descriptor.vmd)
  {
    serialize(mds_descriptor_node, vmd);
  }
  parent->append_node(mds_descriptor_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::Metadata& metadata)
{
  auto* meta_data_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetaData");

  for (const auto& model_name : metadata.modelName)
  {
    auto* model_name_node = xml_document_->allocate_node(rapidxml::node_element, "pm:ModelName");
    auto* ref_attr = xml_document_->allocate_attribute("Ref", model_name.c_str());
    model_name_node->append_attribute(ref_attr);
    meta_data_node->append_node(model_name_node);
  }
  if (metadata.modelNumber.has_value())
  {
    auto* model_number_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:ModelNumber");
    model_number_node->value(metadata.modelNumber.value().c_str());
    meta_data_node->append_node(model_number_node);
  }
  for (const auto& serial_number : metadata.serialNumber)
  {
    auto* serial_number_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:SerialNumber");
    serial_number_node->value(serial_number.c_str());
    meta_data_node->append_node(serial_number_node);
  }
  for (const auto& manufacturer : metadata.manufacturer)
  {
    auto* manufacturer_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:Manufacturer");
    auto* ref_attr = xml_document_->allocate_attribute("Ref", manufacturer.c_str());
    manufacturer_node->append_attribute(ref_attr);
    meta_data_node->append_node(manufacturer_node);
  }
  parent->append_node(meta_data_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SystemContextDescriptor& system_context)
{
  auto* system_context_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:SystemContext");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", system_context.handle.c_str());
  system_context_node->append_attribute(handle_attr);
  if (system_context.patientContext.has_value())
  {
    serialize(system_context_node, system_context.patientContext.value());
  }
  if (system_context.locationContext.has_value())
  {
    serialize(system_context_node, system_context.locationContext.value());
  }
  parent->append_node(system_context_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::PatientContextDescriptor& patient_context)
{
  auto* patient_context_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:PatientContext");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", patient_context.handle.c_str());
  patient_context_node->append_attribute(handle_attr);
  if (patient_context.safetyClassification.has_value())
  {
    auto* safety_classification = xml_document_->allocate_string(
        to_string(patient_context.safetyClassification.value()).c_str());
    auto* safety_classification_attr =
        xml_document_->allocate_attribute("SafetyClassification", safety_classification);
    patient_context_node->append_attribute(safety_classification_attr);
  }
  parent->append_node(patient_context_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationContextDescriptor& location_context)
{
  auto* patient_context_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:LocationContext");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", location_context.handle.c_str());
  patient_context_node->append_attribute(handle_attr);
  if (location_context.safetyClassification.has_value())
  {
    auto* safety_classification = xml_document_->allocate_string(
        to_string(location_context.safetyClassification.value()).c_str());
    auto* safety_classification_attr =
        xml_document_->allocate_attribute("SafetyClassification", safety_classification);
    patient_context_node->append_attribute(safety_classification_attr);
  }
  parent->append_node(patient_context_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::VmdDescriptor& vmd)
{
  auto* vmd_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Vmd");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", vmd.handle.c_str());
  vmd_node->append_attribute(handle_attr);
  for (const auto& channel : vmd.channel)
  {
    serialize(vmd_node, channel);
  }
  if (vmd.sco.has_value())
  {
    serialize(vmd_node, vmd.sco.value());
  }
  parent->append_node(vmd_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ChannelDescriptor& channel)
{
  auto* channel_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Channel");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", channel.handle.c_str());
  channel_node->append_attribute(handle_attr);
  if (channel.descriptorVersion.has_value())
  {
    auto* descriptor_version =
        xml_document_->allocate_string(std::to_string(channel.descriptorVersion.value()).c_str());
    auto* descriptor_version_attr =
        xml_document_->allocate_attribute("DescriptorVersion", descriptor_version);
    channel_node->append_attribute(descriptor_version_attr);
  }
  if (channel.safetyClassification.has_value())
  {
    auto* safety_classification =
        xml_document_->allocate_string(to_string(channel.safetyClassification.value()).c_str());
    auto* safety_classification_attr =
        xml_document_->allocate_attribute("SafetyClassification", safety_classification);
    channel_node->append_attribute(safety_classification_attr);
  }
  if (channel.type.has_value())
  {
    auto* type_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Type");
    auto* code_attr = xml_document_->allocate_attribute("Code", channel.type.value().code.c_str());
    type_node->append_attribute(code_attr);
    channel_node->append_node(type_node);
  }
  for (const auto& metric : channel.metric)
  {
    serialize(channel_node, *metric);
  }
  parent->append_node(channel_node);
}

void MessageSerializer::serialize(
    rapidxml::xml_node<>* parent,
    const BICEPS::PM::AbstractMetricDescriptor& abstract_metric_descriptor)
{
  auto* metric_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Metric");
  auto* handle_attr =
      xml_document_->allocate_attribute("Handle", abstract_metric_descriptor.handle.c_str());
  metric_node->append_attribute(handle_attr);

  auto* descriptor_version = xml_document_->allocate_string(
      std::to_string(abstract_metric_descriptor.descriptorVersion.value_or(0)).c_str());
  auto* descriptor_version_attr =
      xml_document_->allocate_attribute("DescriptorVersion", descriptor_version);
  metric_node->append_attribute(descriptor_version_attr);

  if (abstract_metric_descriptor.safetyClassification.has_value())
  {
    auto* safety_classification = xml_document_->allocate_string(
        to_string(abstract_metric_descriptor.safetyClassification.value()).c_str());
    auto* safety_classification_attr =
        xml_document_->allocate_attribute("SafetyClassification", safety_classification);
    metric_node->append_attribute(safety_classification_attr);
  }

  auto* unit_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Unit");
  auto* unit_code_attr =
      xml_document_->allocate_attribute("Code", abstract_metric_descriptor.unit.code.c_str());
  unit_node->append_attribute(unit_code_attr);
  metric_node->append_node(unit_node);

  auto* metric_category =
      xml_document_->allocate_string(to_string(abstract_metric_descriptor.metricCategory).c_str());
  auto* metric_category_attr = xml_document_->allocate_attribute("MetricCategory", metric_category);
  metric_node->append_attribute(metric_category_attr);

  auto* metric_availability = xml_document_->allocate_string(
      to_string(abstract_metric_descriptor.metricAvailability).c_str());
  auto* metric_availability_attr =
      xml_document_->allocate_attribute("MetricAvailability", metric_availability);
  metric_node->append_attribute(metric_availability_attr);

  if (const auto* const numeric_descriptor =
          dyn_cast<BICEPS::PM::NumericMetricDescriptor>(&abstract_metric_descriptor);
      numeric_descriptor != nullptr)
  {
    auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:NumericMetricDescriptor");
    metric_node->append_attribute(type_attr);

    for (const auto& range : numeric_descriptor->technicalRange)
    {
      auto* technical_range_node =
          xml_document_->allocate_node(rapidxml::node_element, "TechnicalRange");
      serialize(technical_range_node, range);
      metric_node->append_node(technical_range_node);
    }

    auto* resolution =
        xml_document_->allocate_string(std::to_string(numeric_descriptor->resolution).c_str());
    auto* resolution_attr = xml_document_->allocate_attribute("Resolution", resolution);
    metric_node->append_attribute(resolution_attr);

    if (numeric_descriptor->averagingPeriod.has_value())
    {
      auto* averaging_period_attr = xml_document_->allocate_attribute(
          "AveragingPeriod", numeric_descriptor->averagingPeriod->c_str());
      metric_node->append_attribute(averaging_period_attr);
    }
  }
  else if (const auto* const string_descriptor =
               dyn_cast<BICEPS::PM::StringMetricDescriptor>(&abstract_metric_descriptor);
           string_descriptor != nullptr)
  {
    auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:StringMetricDescriptor");
    metric_node->append_attribute(type_attr);
  }

  parent->append_node(metric_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Range& range)
{
  if (range.lower.has_value())
  {
    auto* lower = xml_document_->allocate_string(std::to_string(range.lower.value()).c_str());
    auto* lower_attr = xml_document_->allocate_attribute("Lower", lower);
    parent->append_attribute(lower_attr);
  }
  if (range.upper.has_value())
  {
    auto* upper = xml_document_->allocate_string(std::to_string(range.upper.value()).c_str());
    auto* upper_attr = xml_document_->allocate_attribute("Upper", upper);
    parent->append_attribute(upper_attr);
  }
  if (range.stepWidth.has_value())
  {
    auto* step_width =
        xml_document_->allocate_string(std::to_string(range.stepWidth.value()).c_str());
    auto* step_width_attr = xml_document_->allocate_attribute("StepWidth", step_width);
    parent->append_attribute(step_width_attr);
  }
  if (range.relativeAccuracy.has_value())
  {
    auto* relative_accuracy =
        xml_document_->allocate_string(std::to_string(range.relativeAccuracy.value()).c_str());
    auto* relative_accuracy_attr =
        xml_document_->allocate_attribute("RelativeAccuracy", relative_accuracy);
    parent->append_attribute(relative_accuracy_attr);
  }
  if (range.absoluteAccuracy.has_value())
  {
    auto* absolute_accuracy =
        xml_document_->allocate_string(std::to_string(range.absoluteAccuracy.value()).c_str());
    auto* absolute_accuracy_attr =
        xml_document_->allocate_attribute("AbsoluteAccuracy", absolute_accuracy);
    parent->append_attribute(absolute_accuracy_attr);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdState& md_state)
{
  auto* md_state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MdState");
  for (const auto& state : md_state.state)
  {
    serialize(md_state_node, *state);
  }
  parent->append_node(md_state_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractState& state)
{
  auto* state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:State");
  auto* descriptor = xml_document_->allocate_string(state.descriptorHandle.c_str());
  auto* descriptor_handle_attr = xml_document_->allocate_attribute("DescriptorHandle", descriptor);
  state_node->append_attribute(descriptor_handle_attr);

  if (state.stateVersion.has_value())
  {
    auto* version =
        xml_document_->allocate_string(std::to_string(state.stateVersion.value()).c_str());
    auto* version_attr = xml_document_->allocate_attribute("StateVersion", version);
    state_node->append_attribute(version_attr);
  }

  if (const auto* numeric_metric_state = dyn_cast<BICEPS::PM::NumericMetricState>(&state);
      numeric_metric_state != nullptr)
  {
    if (numeric_metric_state->metricValue.has_value())
    {
      serialize(state_node, numeric_metric_state->metricValue.value());
    }
    auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:NumericMetricState");
    state_node->append_attribute(type_attr);
  }
  else if (const auto* string_metric_state = dyn_cast<BICEPS::PM::StringMetricState>(&state);
           string_metric_state != nullptr)
  {
    if (string_metric_state->metricValue.has_value())
    {
      serialize(state_node, string_metric_state->metricValue.value());
    }
    auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:StringMetricState");
    state_node->append_attribute(type_attr);
  }
  else if (const auto* location_context_state = dyn_cast<BICEPS::PM::LocationContextState>(&state);
           location_context_state != nullptr)
  {
    for (const auto& validator : location_context_state->validator)
    {
      auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Validator");
      serialize(node, validator);
      state_node->append_node(node);
    }
    for (const auto& identifier : location_context_state->identification)
    {
      auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Identification");
      serialize(node, identifier);
      state_node->append_node(node);
    }
    if (location_context_state->locationDetail.has_value())
    {
      serialize(state_node, location_context_state->locationDetail.value());
    }
    if (location_context_state->bindingMdibVersion.has_value())
    {
      auto* version = xml_document_->allocate_string(
          std::to_string(location_context_state->bindingMdibVersion.value()).c_str());
      auto* attr = xml_document_->allocate_attribute("BindingMdibVersion", version);
      state_node->append_attribute(attr);
    }
    if (location_context_state->contextAssociation.has_value())
    {
      auto* assoc = xml_document_->allocate_string(
          to_string(location_context_state->contextAssociation.value()).c_str());
      auto* attr = xml_document_->allocate_attribute("ContextAssociation", assoc);
      state_node->append_attribute(attr);
    }
    auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:LocationContextState");
    state_node->append_attribute(type_attr);
    auto* handle_attr =
        xml_document_->allocate_attribute("Handle", location_context_state->handle.c_str());
    state_node->append_attribute(handle_attr);
  }
  else if (const auto* operation_state = dyn_cast<BICEPS::PM::AbstractOperationState>(&state);
           operation_state != nullptr)
  {
    auto* operating_mode_str =
        xml_document_->allocate_string(to_string(operation_state->operatingMode).c_str());
    auto* operating_mode_attr =
        xml_document_->allocate_attribute("OperatingMode", operating_mode_str);
    state_node->append_attribute(operating_mode_attr);

    if (const auto* set_value_operation_state =
            dyn_cast<BICEPS::PM::SetValueOperationState>(&state);
        set_value_operation_state != nullptr)
    {
      auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:SetValueOperationState");
      state_node->append_attribute(type_attr);
    }
    else if (const auto* set_string_operation_state =
                 dyn_cast<BICEPS::PM::SetStringOperationState>(&state);
             set_string_operation_state != nullptr)
    {
      auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:SetStringOperationState");
      state_node->append_attribute(type_attr);
      for (const auto& allowed : set_string_operation_state->allowedValues)
      {
        auto* allowed_value_str = xml_document_->allocate_string(allowed.c_str());
        auto* allowed_value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Value");
        allowed_value_node->value(allowed_value_str);
        state_node->append_node(allowed_value_node);
      }
    }
  }
  parent->append_node(state_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::InstanceIdentifier& identifier)
{
  if (identifier.root.has_value())
  {
    auto* root_attr = xml_document_->allocate_attribute("Root", identifier.root.value().c_str());
    parent->append_attribute(root_attr);
  }
  if (identifier.extension.has_value())
  {
    auto* extension_attr =
        xml_document_->allocate_attribute("Extension", identifier.extension.value().c_str());
    parent->append_attribute(extension_attr);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationDetail& location_detail)
{
  auto* location_detail_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:LocationDetail");
  if (location_detail.poC.has_value())
  {
    auto* poc_attr = xml_document_->allocate_attribute("PoC", location_detail.poC.value().c_str());
    location_detail_node->append_attribute(poc_attr);
  }
  if (location_detail.room.has_value())
  {
    auto* poc_attr =
        xml_document_->allocate_attribute("Room", location_detail.room.value().c_str());
    location_detail_node->append_attribute(poc_attr);
  }
  if (location_detail.bed.has_value())
  {
    auto* poc_attr = xml_document_->allocate_attribute("Bed", location_detail.bed.value().c_str());
    location_detail_node->append_attribute(poc_attr);
  }
  if (location_detail.facility.has_value())
  {
    auto* poc_attr =
        xml_document_->allocate_attribute("Facility", location_detail.facility.value().c_str());
    location_detail_node->append_attribute(poc_attr);
  }
  if (location_detail.building.has_value())
  {
    auto* poc_attr =
        xml_document_->allocate_attribute("Building", location_detail.building.value().c_str());
    location_detail_node->append_attribute(poc_attr);
  }
  if (location_detail.floor.has_value())
  {
    auto* poc_attr =
        xml_document_->allocate_attribute("Floor", location_detail.floor.value().c_str());
    location_detail_node->append_attribute(poc_attr);
  }
  parent->append_node(location_detail_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractMetricValue& value)
{
  auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
  serialize(value_node, value.metricQuality);

  if (const auto* numeric_value = dyn_cast<BICEPS::PM::NumericMetricValue>(&value);
      numeric_value != nullptr)
  {
    if (numeric_value->value.has_value())
    {
      auto* num =
          xml_document_->allocate_string(std::to_string(numeric_value->value.value()).c_str());
      auto* value_attr = xml_document_->allocate_attribute("Value", num);
      value_node->append_attribute(value_attr);
    }
  }
  else if (const auto* string_value = dyn_cast<BICEPS::PM::StringMetricValue>(&value);
           string_value != nullptr)
  {
    if (string_value->value.has_value())
    {
      auto* val = xml_document_->allocate_string(string_value->value->c_str());
      auto* value_attr = xml_document_->allocate_attribute("Value", val);
      value_node->append_attribute(value_attr);
    }
  }

  parent->append_node(value_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MetricQuality& quality)
{
  auto* metric_quality_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:MetricQuality");
  auto* validity = xml_document_->allocate_string(to_string(quality.validity).c_str());
  auto* validity_attr = xml_document_->allocate_attribute("Validity", validity);
  metric_quality_node->append_attribute(validity_attr);
  parent->append_node(metric_quality_node);
}


void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::SubscribeResponse& subscribe_response)
{
  auto* subscribe_response_node =
      xml_document_->allocate_node(rapidxml::node_element, "wse:SubscribeResponse");
  auto* subscription_manager_node =
      xml_document_->allocate_node(rapidxml::node_element, "wse:SubscriptionManager");

  auto* address_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:Address");
  address_node->value(subscribe_response.subscriptionManager.address.c_str());
  subscription_manager_node->append_node(address_node);

  serialize(subscription_manager_node,
            subscribe_response.subscriptionManager.referenceParameters.value());

  subscribe_response_node->append_node(subscription_manager_node);

  serialize(subscribe_response_node, subscribe_response.expires);

  parent->append_node(subscribe_response_node);
}

void MessageSerializer::serialize(
    rapidxml::xml_node<>* parent,
    const WS::ADDRESSING::ReferenceParametersType& reference_parameters)
{
  auto* reference_parameters_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsa:ReferenceParameters");
  if (reference_parameters.identifier.has_value())
  {
    auto* identifier_node = xml_document_->allocate_node(rapidxml::node_element, "wse:Identifier");
    identifier_node->value(reference_parameters.identifier.value().c_str());
    reference_parameters_node->append_node(identifier_node);
  }
  parent->append_node(reference_parameters_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::RenewResponse& renew_response)
{
  auto* renew_response_node =
      xml_document_->allocate_node(rapidxml::node_element, "wse:RenewResponse");
  if (renew_response.expires.has_value())
  {
    serialize(renew_response_node, renew_response.expires.value());
  }
  parent->append_node(renew_response_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::AbstractSetResponse& set_response)
{
  auto* set_value_response_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:SetValueResponse");
  auto* xmlns_biceps_message =
      xml_document_->allocate_attribute("xmlns:msg", SDC::NS_BICEPS_MESSAGE_MODEL);
  set_value_response_node->append_attribute(xmlns_biceps_message);
  if (set_response.mdibVersion.has_value())
  {
    auto* mdib_version =
        xml_document_->allocate_string(std::to_string(set_response.mdibVersion.value()).c_str());
    auto* mdib_version_attr = xml_document_->allocate_attribute("MdibVersion", mdib_version);
    set_value_response_node->append_attribute(mdib_version_attr);
  }
  auto* sequence_id = xml_document_->allocate_string(set_response.sequenceId.c_str());
  auto* sequence_id_attr = xml_document_->allocate_attribute("SequenceId", sequence_id);
  set_value_response_node->append_attribute(sequence_id_attr);
  if (set_response.instanceId.has_value())
  {
    auto* instance_id =
        xml_document_->allocate_string(std::to_string(set_response.instanceId.value()).c_str());
    auto* instance_id_attr = xml_document_->allocate_attribute("SequenceId", instance_id);
    set_value_response_node->append_attribute(instance_id_attr);
  }
  serialize(set_value_response_node, set_response.invocationInfo);
  parent->append_node(set_value_response_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::InvocationInfo& invocation_info)
{
  auto* invocation_info_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:InvocationInfo");

  auto* transaction_id =
      xml_document_->allocate_string(std::to_string(invocation_info.transactionId).c_str());
  auto* transaction_id_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:TransactionId", transaction_id);
  invocation_info_node->append_node(transaction_id_node);

  auto* invocation_state =
      xml_document_->allocate_string(to_string(invocation_info.invocationState).c_str());
  auto* invocation_state_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:InvocationState", invocation_state);
  invocation_info_node->append_node(invocation_state_node);

  if (invocation_info.invocationError.has_value())
  {
    auto* invocation_error =
        xml_document_->allocate_string(to_string(invocation_info.invocationError.value()).c_str());
    auto* invocation_error_node = xml_document_->allocate_node(
        rapidxml::node_element, "msg:InvocationError", invocation_error);
    invocation_info_node->append_node(invocation_error_node);
  }

  if (invocation_info.invocationErrorMessage.has_value())
  {
    auto* invocation_error_message =
        xml_document_->allocate_string(invocation_info.invocationErrorMessage.value().c_str());
    auto* invocation_error_message_node = xml_document_->allocate_node(
        rapidxml::node_element, "msg:InvocationErrorMessage", invocation_error_message);
    invocation_info_node->append_node(invocation_error_message_node);
  }
  parent->append_node(invocation_info_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::EpisodicMetricReport& report)
{
  auto* report_node =
      xml_document_->allocate_node(rapidxml::node_element, "mm:EpisodicMetricReport");
  if (report.mdibVersion.has_value())
  {
    auto* version =
        xml_document_->allocate_string(std::to_string(report.mdibVersion.value()).c_str());
    auto* version_attr = xml_document_->allocate_attribute("MdibVersion", version);
    report_node->append_attribute(version_attr);
  }
  for (const auto& part : report.reportPart)
  {
    serialize(report_node, part);
  }
  parent->append_node(report_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::MetricReportPart& part)
{
  auto* report_part_node = xml_document_->allocate_node(rapidxml::node_element, "mm:ReportPart");
  for (const auto& state : part.metricState)
  {
    serialize(report_part_node, *state);
  }
  parent->append_node(report_part_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ScoDescriptor& sco)
{
  auto* sco_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Sco");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", sco.handle.c_str());
  sco_node->append_attribute(handle_attr);
  auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:ScoDescriptor");
  sco_node->append_attribute(type_attr);
  for (const auto& operation : sco.operation)
  {
    serialize(sco_node, *operation);
  }
  parent->append_node(sco_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractOperationDescriptor& operation)
{
  auto* operation_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Operation");
  auto* handle_attr = xml_document_->allocate_attribute("Handle", operation.handle.c_str());
  operation_node->append_attribute(handle_attr);
  auto* operation_target_attr =
      xml_document_->allocate_attribute("OperationTarget", operation.operationTarget.c_str());
  operation_node->append_attribute(operation_target_attr);
  if (isa<BICEPS::PM::SetValueOperationDescriptor>(&operation))
  {
    auto* type_attr =
        xml_document_->allocate_attribute("xsi:type", "pm:SetValueOperationDescriptor");
    operation_node->append_attribute(type_attr);
  }
  else if (const auto* set_string = dyn_cast<BICEPS::PM::SetStringOperationDescriptor>(&operation);
           set_string != nullptr)
  {
    auto* type_attr =
        xml_document_->allocate_attribute("xsi:type", "pm:SetStringOperationDescriptor");
    operation_node->append_attribute(type_attr);
    if (set_string->maxLength.has_value())
    {
      auto* max_length_str =
          xml_document_->allocate_string(std::to_string(set_string->maxLength.value()).c_str());
      auto* max_length_attr = xml_document_->allocate_attribute("MaxLength", max_length_str);
      operation_node->append_attribute(max_length_attr);
    }
  }
  parent->append_node(operation_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AllowedValue& allowed_value)
{
  // TODO
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::ExpirationType& expiration)
{
  const auto* expires_duration = xml_document_->allocate_string(to_string(expiration).c_str());
  auto* expires_node =
      xml_document_->allocate_node(rapidxml::node_element, "wse:Expires", expires_duration);
  parent->append_node(expires_node);
}

/*static*/ std::string
MessageSerializer::to_string(BICEPS::PM::SafetyClassification safety_classification)
{
  switch (safety_classification)
  {
    case BICEPS::PM::SafetyClassification::Inf:
      return "Inf";
    case BICEPS::PM::SafetyClassification::MedA:
      return "MedA";
    case BICEPS::PM::SafetyClassification::MedB:
      return "MedB";
    case BICEPS::PM::SafetyClassification::MedC:
      return "MedC";
  }
  assert(false && "Uncatched value in SafetyClassification");
  return "";
}

std::string MessageSerializer::to_string(const WS::DISCOVERY::UriListType& uri_list)
{
  std::string out;
  for (auto uri = uri_list.begin(); uri != uri_list.end(); uri++)
  {
    if (uri != uri_list.begin())
    {
      out += " ";
    }
    out += *uri;
  }
  return out;
}

std::string MessageSerializer::to_string(const WS::DISCOVERY::QNameListType& q_name_list)
{
  std::string out;
  for (auto qname = q_name_list.begin(); qname != q_name_list.end(); qname++)
  {
    if (qname != q_name_list.begin())
    {
      out += " ";
    }
    out += std::string(std::string(qname->ns) + ":" + qname->name);
  }
  return out;
}

std::string MessageSerializer::to_string(BICEPS::PM::MetricCategory category)
{
  switch (category)
  {
    case BICEPS::PM::MetricCategory::Unspec:
      return "Unspec";
    case BICEPS::PM::MetricCategory::Msrmt:
      return "Msrmt";
    case BICEPS::PM::MetricCategory::Clc:
      return "Clc";
    case BICEPS::PM::MetricCategory::Set:
      return "Set";
    case BICEPS::PM::MetricCategory::Preset:
      return "Preset";
    case BICEPS::PM::MetricCategory::Rcmm:
      return "Rcmm";
  }
  assert(false && "Uncatched value in MetricCategory");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::MetricAvailability availability)
{
  switch (availability)
  {
    case BICEPS::PM::MetricAvailability::Intr:
      return "Intr";
    case BICEPS::PM::MetricAvailability::Cont:
      return "Cont";
  }
  assert(false && "Uncatched value in MetricAvailability");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::MeasurementValidity validity)
{
  switch (validity)
  {
    case BICEPS::PM::MeasurementValidity::Calib:
      return "Calib";
    case BICEPS::PM::MeasurementValidity::Inv:
      return "Inv";
    case BICEPS::PM::MeasurementValidity::NA:
      return "NA";
    case BICEPS::PM::MeasurementValidity::Oflw:
      return "Oflw";
    case BICEPS::PM::MeasurementValidity::Ong:
      return "Ong";
    case BICEPS::PM::MeasurementValidity::Qst:
      return "Qst";
    case BICEPS::PM::MeasurementValidity::Uflw:
      return "Uflw";
    case BICEPS::PM::MeasurementValidity::Vld:
      return "Vld";
    case BICEPS::PM::MeasurementValidity::Vldated:
      return "Vldated";
  }
  assert(false && "Uncatched value in MeasurementValidity");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::MM::InvocationState invocation_state)
{
  switch (invocation_state)
  {
    case BICEPS::MM::InvocationState::Wait:
      return "Wait";
    case BICEPS::MM::InvocationState::Start:
      return "Start";
    case BICEPS::MM::InvocationState::Cnclld:
      return "Cnclld";
    case BICEPS::MM::InvocationState::CnclldMan:
      return "CnclldMan";
    case BICEPS::MM::InvocationState::Fin:
      return "Fin";
    case BICEPS::MM::InvocationState::FinMod:
      return "FinMod";
    case BICEPS::MM::InvocationState::Fail:
      return "Fail";
  }
  assert(false && "Uncatched value in InvocationState");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::MM::InvocationError invocation_error)
{
  switch (invocation_error)
  {
    case BICEPS::MM::InvocationError::Unspec:
      return "Unspec";
    case BICEPS::MM::InvocationError::Unkn:
      return "Unkn";
    case BICEPS::MM::InvocationError::Inv:
      return "Inv";
    case BICEPS::MM::InvocationError::Oth:
      return "Oth";
  }
  assert(false && "Uncatched value in InvocationError");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::ContextAssociation context_association)
{
  switch (context_association)
  {
    case BICEPS::PM::ContextAssociation::No:
      return "No";
    case BICEPS::PM::ContextAssociation::Pre:
      return "Pre";
    case BICEPS::PM::ContextAssociation::Assoc:
      return "Assoc";
    case BICEPS::PM::ContextAssociation::Dis:
      return "Dis";
  }
  assert(false && "Uncatched value in ContextAssociation");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::OperatingMode operating_mode)
{
  switch (operating_mode)
  {
    case BICEPS::PM::OperatingMode::Dis:
      return "Dis";
    case BICEPS::PM::OperatingMode::En:
      return "En";
    case BICEPS::PM::OperatingMode::NA:
      return "NA";
  }
  assert(false && "Uncatched value in ContextAssociation");
  return "";
}

std::string MessageSerializer::to_string(Duration duration)
{
  std::string out;
  out += "P";
  out += std::to_string(duration.years());
  out += "Y";
  out += std::to_string(duration.months());
  out += "M";
  out += std::to_string(duration.days());
  out += "DT";
  out += std::to_string(duration.hours());
  out += "H";
  out += std::to_string(duration.minutes());
  out += "M";
  out += std::to_string(duration.seconds());
  out += "S";
  return out;
}
