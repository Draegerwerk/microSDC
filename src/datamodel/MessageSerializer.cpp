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
  append_attribute(mdib_node, "SequenceId", mdib.sequenceId);
  append_to_string_attribute(mdib_node, "MdibVersion", mdib.mdibVersion.value_or(0));
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
  if (md_description.descriptionVersion.has_value())
  {
    append_to_string_attribute(md_description_node, "DescriptionVersion",
                               md_description.descriptionVersion.value());
  }
  for (const auto& md : md_description.mds)
  {
    serialize(md_description_node, md);
  }
  parent->append_node(md_description_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocalizedText& localized_text)
{
  if (localized_text.ref.has_value())
  {
    append_attribute(parent, "Ref", localized_text.ref.value());
  }
  if (localized_text.lang.has_value())
  {
    append_attribute(parent, "Lang", localized_text.lang.value());
  }
  if (localized_text.version.has_value())
  {
    append_to_string_attribute(parent, "Version", localized_text.version.value());
  }
  if (localized_text.textWidth.has_value())
  {
    append_attribute(parent, "TextWidth", to_string(localized_text.textWidth.value()));
  }
  auto* content_str = xml_document_->allocate_string(localized_text.content.c_str());
  parent->value(content_str);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::CodedValue& coded_value)
{
  append_attribute(parent, "Code", coded_value.code);
  if (coded_value.conceptDescription.has_value())
  {
    auto* concept_description_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:ConceptDescription");
    serialize(concept_description_node, coded_value.conceptDescription.value());
    parent->append_node(concept_description_node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractDescriptor& abstract_descriptor)
{
  append_attribute(parent, "Handle", abstract_descriptor.handle);

  if (abstract_descriptor.descriptorVersion.has_value())
  {
    append_to_string_attribute(parent, "DescriptorVersion",
                               abstract_descriptor.descriptorVersion.value());
  }
  if (abstract_descriptor.safetyClassification.has_value())
  {
    append_attribute(parent, "SafetyClassification",
                     to_string(abstract_descriptor.safetyClassification.value()));
  }
  if (abstract_descriptor.type.has_value())
  {
    auto* type_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Type");
    serialize(type_node, abstract_descriptor.type.value());
    parent->append_node(type_node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractDeviceComponentDescriptor& device)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDescriptor&>(device));
}

void MessageSerializer::serialize(
    rapidxml::xml_node<>* parent,
    const BICEPS::PM::AbstractComplexDeviceComponentDescriptor& complex_device)
{
  serialize(parent,
            static_cast<const BICEPS::PM::AbstractDeviceComponentDescriptor&>(complex_device));
  if (complex_device.sco.has_value())
  {
    serialize(parent, complex_device.sco.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdsDescriptor& mds_descriptor)
{
  auto* mds_descriptor_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Mds");

  serialize(
      mds_descriptor_node,
      static_cast<const BICEPS::PM::AbstractComplexDeviceComponentDescriptor&>(mds_descriptor));

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
    serialize(model_name_node, model_name);
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
    serialize(manufacturer_node, manufacturer);
    meta_data_node->append_node(manufacturer_node);
  }
  parent->append_node(meta_data_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SystemContextDescriptor& system_context)
{
  auto* system_context_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:SystemContext");
  serialize(system_context_node,
            static_cast<const BICEPS::PM::AbstractDeviceComponentDescriptor&>(system_context));
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
                                  const BICEPS::PM::AbstractContextDescriptor& context)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDescriptor&>(context));
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::PatientContextDescriptor& patient_context)
{
  auto* patient_context_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:PatientContext");
  serialize(patient_context_node,
            static_cast<const BICEPS::PM::AbstractContextDescriptor&>(patient_context));
  parent->append_node(patient_context_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationContextDescriptor& location_context)
{
  auto* patient_context_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:LocationContext");
  serialize(patient_context_node,
            static_cast<const BICEPS::PM::AbstractContextDescriptor&>(location_context));
  parent->append_node(patient_context_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::VmdDescriptor& vmd)
{
  auto* vmd_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Vmd");
  serialize(vmd_node,
            static_cast<const BICEPS::PM::AbstractComplexDeviceComponentDescriptor&>(vmd));
  for (const auto& channel : vmd.channel)
  {
    serialize(vmd_node, channel);
  }
  parent->append_node(vmd_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ChannelDescriptor& channel)
{
  auto* channel_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Channel");
  serialize(channel_node,
            static_cast<const BICEPS::PM::AbstractDeviceComponentDescriptor&>(channel));
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
  serialize(metric_node,
            static_cast<const BICEPS::PM::AbstractDescriptor&>(abstract_metric_descriptor));

  auto* unit_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Unit");
  serialize(unit_node, abstract_metric_descriptor.unit);
  metric_node->append_node(unit_node);

  append_attribute(metric_node, "MetricCategory",
                   to_string(abstract_metric_descriptor.metricCategory));
  append_attribute(metric_node, "MetricAvailability",
                   to_string(abstract_metric_descriptor.metricAvailability));

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

    append_to_string_attribute(metric_node, "Resolution", numeric_descriptor->resolution);

    if (numeric_descriptor->averagingPeriod.has_value())
    {
      append_attribute(metric_node, "AveragingPeriod", numeric_descriptor->averagingPeriod.value());
    }
  }
  else if (const auto* const string_descriptor =
               dyn_cast<BICEPS::PM::StringMetricDescriptor>(&abstract_metric_descriptor);
           string_descriptor != nullptr)
  {
    append_attribute(metric_node, "xsi:type", "pm:StringMetricDescriptor");
  }
  else if (const auto* const enum_string_descriptor =
               dyn_cast<BICEPS::PM::EnumStringMetricDescriptor>(&abstract_metric_descriptor);
           enum_string_descriptor != nullptr)
  {
    append_attribute(metric_node, "xsi:type", "pm:EnumStringMetricDescriptor");
    for (const auto& value : enum_string_descriptor->allowedValue)
    {
      serialize(metric_node, value);
    }
  }

  parent->append_node(metric_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Range& range)
{
  if (range.lower.has_value())
  {
    append_to_string_attribute(parent, "Lower", range.lower.value());
  }
  if (range.upper.has_value())
  {
    append_to_string_attribute(parent, "Upper", range.upper.value());
  }
  if (range.stepWidth.has_value())
  {
    append_to_string_attribute(parent, "StepWidth", range.stepWidth.value());
  }
  if (range.relativeAccuracy.has_value())
  {
    append_to_string_attribute(parent, "RelativeAccuracy", range.relativeAccuracy.value());
  }
  if (range.absoluteAccuracy.has_value())
  {
    append_to_string_attribute(parent, "AbsoluteAccuracy", range.absoluteAccuracy.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdState& md_state)
{
  auto* md_state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MdState");
  if (md_state.stateVersion.has_value())
  {
    append_to_string_attribute(md_state_node, "StateVersion", md_state.stateVersion.value());
  }
  for (const auto& state : md_state.state)
  {
    auto* state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:State");
    if (const auto numeric_metric_state = dyn_cast<BICEPS::PM::NumericMetricState>(state);
        numeric_metric_state != nullptr)
    {
      serialize(state_node, *numeric_metric_state);
    }
    else if (const auto string_metric_state = dyn_cast<BICEPS::PM::StringMetricState>(state);
             string_metric_state != nullptr)
    {
      serialize(state_node, *string_metric_state);
    }
    else if (const auto string_metric_state = dyn_cast<BICEPS::PM::EnumStringMetricState>(state);
             string_metric_state != nullptr)
    {
      serialize(state_node, *string_metric_state);
    }
    else if (const auto location_context_state = dyn_cast<BICEPS::PM::LocationContextState>(state);
             location_context_state != nullptr)
    {
      serialize(state_node, *location_context_state);
    }
    else if (const auto operation_state = dyn_cast<BICEPS::PM::SetStringOperationState>(state);
             operation_state != nullptr)
    {
      serialize(state_node, *operation_state);
    }
    else if (const auto operation_state = dyn_cast<BICEPS::PM::SetValueOperationState>(state);
             operation_state != nullptr)
    {
      serialize(state_node, *operation_state);
    }
    md_state_node->append_node(state_node);
  }
  parent->append_node(md_state_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractState& state)
{
  append_attribute(parent, "DescriptorHandle", state.descriptorHandle);

  if (state.stateVersion.has_value())
  {
    append_to_string_attribute(parent, "StateVersion", state.stateVersion.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractMetricState& metric_state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractState&>(metric_state));
  if (metric_state.activationState.has_value())
  {
    append_attribute(parent, "ActivationState", to_string(metric_state.activationState.value()));
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::NumericMetricState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricState&>(state));
  if (state.activeAveragingPeriod.has_value())
  {
    append_attribute(parent, "ActiveAveragingPeriod", state.activeAveragingPeriod.value());
  }
  if (state.metricValue.has_value())
  {
    auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
    serialize(value_node, state.metricValue.value());
    parent->append_node(value_node);
  }
  for (const auto& range : state.physiologicalRange)
  {
    auto* range_node = xml_document_->allocate_node(rapidxml::node_element, "PhysiologicalRange");
    serialize(range_node, range);
    parent->append_node(range_node);
  }
  append_attribute(parent, "xsi:type", "pm:NumericMetricState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::StringMetricState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricState&>(state));
  if (state.metricValue.has_value())
  {
    auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
    serialize(value_node, state.metricValue.value());
    parent->append_node(value_node);
  }
  append_attribute(parent, "xsi:type", "pm:StringMetricState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::EnumStringMetricState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricState&>(state));
  if (state.metricValue.has_value())
  {
    auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
    serialize(value_node, state.metricValue.value());
    parent->append_node(value_node);
  }
  append_attribute(parent, "xsi:type", "pm:EnumStringMetricState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractMultiState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractState&>(state));
  append_attribute(parent, "Handle", state.handle);
  if (state.category.has_value())
  {
    auto* category_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Category");
    serialize(category_node, state.category.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractContextState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMultiState&>(state));
  if (state.bindingMdibVersion.has_value())
  {
    append_to_string_attribute(parent, "BindingMdibVersion", state.bindingMdibVersion.value());
  }
  if (state.contextAssociation.has_value())
  {
    append_attribute(parent, "ContextAssociation", to_string(state.contextAssociation.value()));
  }
  for (const auto& validator : state.validator)
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Validator");
    serialize(node, validator);
    parent->append_node(node);
  }
  for (const auto& identifier : state.identification)
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Identification");
    serialize(node, identifier);
    parent->append_node(node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationContextState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractContextState&>(state));
  if (state.locationDetail.has_value())
  {
    serialize(parent, state.locationDetail.value());
  }
  append_attribute(parent, "xsi:type", "pm:LocationContextState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractOperationState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractState&>(state));
  append_attribute(parent, "OperatingMode", to_string(state.operatingMode));
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SetValueOperationState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractOperationState&>(state));
  append_attribute(parent, "xsi:type", "pm:SetValueOperationState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SetStringOperationState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractOperationState&>(state));
  append_attribute(parent, "xsi:type", "pm:SetStringOperationState");

  for (const auto& allowed : state.allowedValues)
  {
    auto* allowed_value_str = xml_document_->allocate_string(allowed.c_str());
    auto* allowed_value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Value");
    allowed_value_node->value(allowed_value_str);
    parent->append_node(allowed_value_node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::InstanceIdentifier& identifier)
{
  if (identifier.root.has_value())
  {
    append_attribute(parent, "Root", identifier.root.value());
  }
  if (identifier.extension.has_value())
  {
    append_attribute(parent, "Extension", identifier.extension.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationDetail& location_detail)
{
  auto* location_detail_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:LocationDetail");
  if (location_detail.poC.has_value())
  {
    append_attribute(location_detail_node, "PoC", location_detail.poC.value());
  }
  if (location_detail.room.has_value())
  {
    append_attribute(location_detail_node, "Room", location_detail.room.value());
  }
  if (location_detail.bed.has_value())
  {
    append_attribute(location_detail_node, "Bed", location_detail.bed.value());
  }
  if (location_detail.facility.has_value())
  {
    append_attribute(location_detail_node, "Facility", location_detail.facility.value());
  }
  if (location_detail.building.has_value())
  {
    append_attribute(location_detail_node, "Building", location_detail.building.value());
  }
  if (location_detail.floor.has_value())
  {
    append_attribute(location_detail_node, "Floor", location_detail.floor.value());
  }
  parent->append_node(location_detail_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractMetricValue& value)
{
  serialize(parent, value.metricQuality);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::NumericMetricValue& value)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricValue&>(value));
  if (value.value.has_value())
  {
    append_to_string_attribute(parent, "Value", value.value.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::StringMetricValue& value)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricValue&>(value));
  if (value.value.has_value())
  {
    append_attribute(parent, "Value", value.value.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MetricQuality& quality)
{
  auto* metric_quality_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:MetricQuality");
  append_attribute(metric_quality_node, "Validity", to_string(quality.validity));
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
    append_to_string_attribute(set_value_response_node, "MdibVersion",
                               set_response.mdibVersion.value());
  }
  append_attribute(set_value_response_node, "SequenceId", set_response.sequenceId);
  if (set_response.instanceId.has_value())
  {
    append_to_string_attribute(set_value_response_node, "InstanceId",
                               set_response.instanceId.value());
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
    append_to_string_attribute(report_node, "MdibVersion", report.mdibVersion.value());
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
    auto* state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:State");
    if (const auto numeric_metric_state = dyn_cast<const BICEPS::PM::NumericMetricState>(state);
        numeric_metric_state != nullptr)
    {
      serialize(state_node, *numeric_metric_state);
    }
    else if (const auto string_metric_state = dyn_cast<const BICEPS::PM::StringMetricState>(state);
             string_metric_state != nullptr)
    {
      serialize(state_node, *string_metric_state);
    }
    else if (const auto string_metric_state =
                 dyn_cast<const BICEPS::PM::EnumStringMetricState>(state);
             string_metric_state != nullptr)
    {
      serialize(state_node, *string_metric_state);
    }
    report_part_node->append_node(state_node);
  }
  parent->append_node(report_part_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ScoDescriptor& sco)
{
  auto* sco_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Sco");
  serialize(sco_node, static_cast<const BICEPS::PM::AbstractDeviceComponentDescriptor&>(sco));
  append_attribute(sco_node, "xsi:type", "pm:ScoDescriptor");
  for (const auto& operation : sco.operation)
  {
    auto* operation_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Operation");
    if (const auto set_string = dyn_cast<BICEPS::PM::SetStringOperationDescriptor>(operation);
        set_string != nullptr)
    {
      serialize(operation_node, *set_string);
    }
    else if (const auto set_value = dyn_cast<BICEPS::PM::SetValueOperationDescriptor>(operation);
             set_value != nullptr)
    {
      serialize(operation_node, *set_value);
    }
    parent->append_node(operation_node);
  }
  parent->append_node(sco_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractOperationDescriptor& operation)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDescriptor&>(operation));
  append_attribute(parent, "OperationTarget", operation.operationTarget);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SetStringOperationDescriptor& operation)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractOperationDescriptor&>(operation));
  append_attribute(parent, "xsi:type", "pm:SetStringOperationDescriptor");
  if (operation.maxLength.has_value())
  {
    append_to_string_attribute(parent, "MaxLength", operation.maxLength.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SetValueOperationDescriptor& operation)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractOperationDescriptor&>(operation));
  append_attribute(parent, "xsi:type", "pm:SetValueOperationDescriptor");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AllowedValue& value)
{
  auto* allowed_value_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:AllowedValue");
  auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Value");
  auto* value_str = xml_document_->allocate_string(value.value.c_str());
  value_node->value(value_str);
  allowed_value_node->append_node(value_node);
  if (value.type.has_value())
  {
    auto* type_node = xml_document_->allocate_node(rapidxml::node_element, "pm:Type");
    serialize(type_node, value.type.value());
    allowed_value_node->append_node(type_node);
  }
  if (value.identification.has_value())
  {
    auto* identification_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:Identification");
    serialize(identification_node, value.identification.value());
    allowed_value_node->append_node(identification_node);
  }
  if (value.characteristic.has_value())
  {
    auto* characteristic_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:Characteristic");
    serialize(characteristic_node, value.characteristic.value());
    allowed_value_node->append_node(characteristic_node);
  }
  parent->append_node(allowed_value_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::Measurement& measurement)
{
  append_to_string_attribute(parent, "MeasuredValue", measurement.measuredValue);
  auto* unit_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MeasurementUnit");
  serialize(unit_node, measurement.measurementUnit);
  parent->append_node(unit_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::ExpirationType& expiration)
{
  const auto* expires_duration = xml_document_->allocate_string(to_string(expiration).c_str());
  auto* expires_node =
      xml_document_->allocate_node(rapidxml::node_element, "wse:Expires", expires_duration);
  parent->append_node(expires_node);
}

void MessageSerializer::append_attribute(rapidxml::xml_node<>* parent, const char* const name,
                                         const std::string& value)
{
  auto* str = xml_document_->allocate_string(value.c_str());
  auto* attr = xml_document_->allocate_attribute(name, str);
  parent->append_attribute(attr);
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

std::string MessageSerializer::to_string(BICEPS::PM::LocalizedTextWidth width)
{
  switch (width)
  {
    case BICEPS::PM::LocalizedTextWidth::xs:
      return "xs";
    case BICEPS::PM::LocalizedTextWidth::s:
      return "s";
    case BICEPS::PM::LocalizedTextWidth::m:
      return "m";
    case BICEPS::PM::LocalizedTextWidth::l:
      return "l";
    case BICEPS::PM::LocalizedTextWidth::xl:
      return "xl";
    case BICEPS::PM::LocalizedTextWidth::xxl:
      return "xxl";
  }
  assert(false && "Uncatched value in LocalizedTextWidth");
  return "";
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

std::string MessageSerializer::to_string(BICEPS::PM::ComponentActivation activation)
{
  switch (activation)
  {
    case BICEPS::PM::ComponentActivation::On:
      return "On";
    case BICEPS::PM::ComponentActivation::NotRdy:
      return "NotRdy";
    case BICEPS::PM::ComponentActivation::StndBy:
      return "StndBy";
    case BICEPS::PM::ComponentActivation::Off:
      return "Off";
    case BICEPS::PM::ComponentActivation::Shtdn:
      return "Shtdn";
    case BICEPS::PM::ComponentActivation::Fail:
      return "Fail";
  }
  assert(false && "Uncatched value in ComponentActivation");
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
