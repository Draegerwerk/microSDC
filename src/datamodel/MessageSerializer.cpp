#include "MessageSerializer.hpp"
#include "Casting.hpp"
#include "datamodel/BICEPS_ParticipantModel.hpp"
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
  if (header.message_id.has_value())
  {
    auto* message_id_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:MessageID");
    message_id_node->value(header.message_id->c_str());
    header_node->append_node(message_id_node);
  }
  if (header.to.has_value())
  {
    auto* to_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:To");
    to_node->value(header.to->c_str());
    header_node->append_node(to_node);
  }
  if (header.app_sequence.has_value())
  {
    serialize(header_node, header.app_sequence.value());
  }
  if (header.relates_to.has_value())
  {
    serialize(header_node, header.relates_to.value());
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
  else if (body.probe_matches.has_value())
  {
    serialize(body_node, body.probe_matches.value());
  }
  else if (body.resolve_matches.has_value())
  {
    serialize(body_node, body.resolve_matches.value());
  }
  else if (body.metadata.has_value())
  {
    serialize(body_node, body.metadata.value());
  }
  else if (body.get_mdib_response.has_value())
  {
    serialize(body_node, body.get_mdib_response.value());
  }
  else if (body.subscribe_response.has_value())
  {
    serialize(body_node, body.subscribe_response.value());
  }
  else if (body.renew_response.has_value())
  {
    serialize(body_node, body.renew_response.value());
  }
  else if (body.episodic_metric_report.has_value())
  {
    serialize(body_node, body.episodic_metric_report.value());
  }
  else if (body.episodic_component_report.has_value())
  {
    serialize(body_node, body.episodic_component_report.value());
  }
  else if (body.set_value_response.has_value())
  {
    serialize(body_node, body.set_value_response.value());
  }
  else if (body.set_string_response.has_value())
  {
    serialize(body_node, body.set_string_response.value());
  }
  parent->append_node(body_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::RelatesToType& relates_to)
{
  auto* relates_to_node = xml_document_->allocate_node(rapidxml::node_element, "wsa:RelatesTo");
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
  const auto instance_id_str = std::to_string(app_sequence.instance_id);
  auto* instance_id = xml_document_->allocate_string(instance_id_str.c_str());
  auto* instance_id_attr = xml_document_->allocate_attribute("InstanceId", instance_id);
  app_sequence_node->append_attribute(instance_id_attr);
  if (app_sequence.sequence_id.has_value())
  {
    auto* sequence_id = xml_document_->allocate_string(app_sequence.sequence_id->c_str());
    auto* sequence_id_attr = xml_document_->allocate_attribute("SequenceId", sequence_id);
    app_sequence_node->append_attribute(sequence_id_attr);
  }
  auto message_number_str = std::to_string(app_sequence.message_number);
  auto* message_number = xml_document_->allocate_string(message_number_str.c_str());
  auto* message_number_attr = xml_document_->allocate_attribute("MessageNumber", message_number);
  app_sequence_node->append_attribute(message_number_attr);
  parent->append_node(app_sequence_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ScopesType& scopes)
{
  auto* scopes_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:Scopes");
  if (scopes.match_by.has_value())
  {
    auto* match_by_attr = xml_document_->allocate_attribute("MatchBy", scopes.match_by->c_str());
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
  serialize(hello_node, hello.endpoint_reference);
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
  if (hello.x_addrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str = xml_document_->allocate_string(to_string(hello.x_addrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    hello_node->append_node(x_addrs_node);
  }
  auto* metadata_version_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadata_version =
      xml_document_->allocate_string(std::to_string(hello.metadata_version).c_str());
  metadata_version_node->value(metadata_version);
  parent->append_node(hello_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ByeType& bye)
{
  auto* bye_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:bye");
  serialize(bye_node, bye.endpoint_reference);
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
  if (bye.x_addrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str = xml_document_->allocate_string(to_string(bye.x_addrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    bye_node->append_node(x_addrs_node);
  }
  if (bye.metadata_version.has_value())
  {
    auto* metadata_version_node =
        xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
    auto* metadata_version =
        xml_document_->allocate_string(std::to_string(bye.metadata_version.value()).c_str());
    metadata_version_node->value(metadata_version);
  }
  parent->append_node(bye_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchType& probe_match)
{
  auto* probe_match_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:ProbeMatch");
  serialize(probe_match_node, probe_match.endpoint_reference);
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
  if (probe_match.x_addrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str =
        xml_document_->allocate_string(to_string(probe_match.x_addrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    probe_match_node->append_node(x_addrs_node);
  }
  auto* metadata_version_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadata_version =
      xml_document_->allocate_string(std::to_string(probe_match.metadata_version).c_str());
  metadata_version_node->value(metadata_version);
  probe_match_node->append_node(metadata_version_node);
  parent->append_node(probe_match_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchesType& probe_matches)
{
  auto* probe_matches_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:ProbeMatches");
  for (const auto& probe_match : probe_matches.probe_match)
  {
    serialize(probe_matches_node, probe_match);
  }
  parent->append_node(probe_matches_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchType& resolve_match)
{
  auto* resolve_match_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:ResolveMatch");
  serialize(resolve_match_node, resolve_match.endpoint_reference);
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
  if (resolve_match.x_addrs.has_value())
  {
    auto* x_addrs_node = xml_document_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* x_addrs_str =
        xml_document_->allocate_string(to_string(resolve_match.x_addrs.value()).c_str());
    x_addrs_node->value(x_addrs_str);
    resolve_match_node->append_node(x_addrs_node);
  }
  auto* metadata_version_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadata_version =
      xml_document_->allocate_string(std::to_string(resolve_match.metadata_version).c_str());
  metadata_version_node->value(metadata_version);
  resolve_match_node->append_node(metadata_version_node);
  parent->append_node(resolve_match_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchesType& resolve_matches)
{
  auto* resolve_matches_node =
      xml_document_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  for (const auto& resolve_match : resolve_matches.resolve_match)
  {
    serialize(resolve_matches_node, resolve_match);
  }
  parent->append_node(resolve_matches_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::MEX::Metadata& metadata)
{
  auto* metadata_node = xml_document_->allocate_node(rapidxml::node_element, "mex:Metadata");
  for (const auto& metadata_section : metadata.metadata_section)
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
  if (metadata_section.this_model.has_value())
  {
    serialize(metadata_section_node, metadata_section.this_model.value());
  }
  else if (metadata_section.this_device.has_value())
  {
    serialize(metadata_section_node, metadata_section.this_device.value());
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
  for (const auto& model_name : this_model.model_name)
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
  for (const auto& friendly_name : this_device.friendly_name)
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
  serialize(host_node, host.endpoint_reference);
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
  for (const auto& epr : hosted.endpoint_reference)
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
  service_id_node->value(hosted.service_id.c_str());
  hosted_node->append_node(service_id_node);
  parent->append_node(hosted_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdibVersionGroup& version_group)
{
  append_attribute(parent, "SequenceId", version_group.sequence_id);
  if (version_group.mdib_version.has_value())
  {
    append_to_string_attribute(parent, "MdibVersion", version_group.mdib_version.value());
  }
  if (version_group.instance_id.has_value())
  {
    append_to_string_attribute(parent, "InstanceId", version_group.instance_id.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::GetMdibResponse& get_mdib_response)
{
  auto* get_mdib_response_node =
      xml_document_->allocate_node(rapidxml::node_element, "mm:GetMdibResponse");
  serialize(get_mdib_response_node, get_mdib_response.mdib_version_group);
  serialize(get_mdib_response_node, get_mdib_response.mdib);
  parent->append_node(get_mdib_response_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Mdib& mdib)
{
  auto* mdib_node = xml_document_->allocate_node(rapidxml::node_element, "mm:Mdib");
  serialize(mdib_node, mdib.mdib_version_group);
  if (mdib.md_description.has_value())
  {
    serialize(mdib_node, mdib.md_description.value());
  }
  if (mdib.md_state.has_value())
  {
    serialize(mdib_node, mdib.md_state.value());
  }
  parent->append_node(mdib_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdDescription& md_description)
{
  auto* md_description_node =
      xml_document_->allocate_node(rapidxml::node_element, "pm:MdDescription");
  if (md_description.description_version.has_value())
  {
    append_to_string_attribute(md_description_node, "DescriptionVersion",
                               md_description.description_version.value());
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
  if (localized_text.text_width.has_value())
  {
    append_attribute(parent, "TextWidth", to_string(localized_text.text_width.value()));
  }
  auto* content_str = xml_document_->allocate_string(localized_text.content.c_str());
  parent->value(content_str);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::CodedValue& coded_value)
{
  append_attribute(parent, "Code", coded_value.code);
  if (coded_value.concept_description.has_value())
  {
    auto* concept_description_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:ConceptDescription");
    serialize(concept_description_node, coded_value.concept_description.value());
    parent->append_node(concept_description_node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractDescriptor& abstract_descriptor)
{
  append_attribute(parent, "Handle", abstract_descriptor.handle);

  if (abstract_descriptor.descriptor_version.has_value())
  {
    append_to_string_attribute(parent, "DescriptorVersion",
                               abstract_descriptor.descriptor_version.value());
  }
  if (abstract_descriptor.safety_classification.has_value())
  {
    append_attribute(parent, "SafetyClassification",
                     to_string(abstract_descriptor.safety_classification.value()));
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

  if (mds_descriptor.meta_data.has_value())
  {
    serialize(mds_descriptor_node, mds_descriptor.meta_data.value());
  }
  if (mds_descriptor.system_context.has_value())
  {
    serialize(mds_descriptor_node, mds_descriptor.system_context.value());
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

  for (const auto& model_name : metadata.model_name)
  {
    auto* model_name_node = xml_document_->allocate_node(rapidxml::node_element, "pm:ModelName");
    serialize(model_name_node, model_name);
    meta_data_node->append_node(model_name_node);
  }
  if (metadata.model_number.has_value())
  {
    auto* model_number_node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:ModelNumber");
    model_number_node->value(metadata.model_number.value().c_str());
    meta_data_node->append_node(model_number_node);
  }
  for (const auto& serial_number : metadata.serial_number)
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
  if (system_context.patient_context.has_value())
  {
    serialize(system_context_node, system_context.patient_context.value());
  }
  if (system_context.location_context.has_value())
  {
    serialize(system_context_node, system_context.location_context.value());
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
                   to_string(abstract_metric_descriptor.metric_category));
  append_attribute(metric_node, "MetricAvailability",
                   to_string(abstract_metric_descriptor.metric_availability));

  if (const auto* const numeric_descriptor =
          dyn_cast<BICEPS::PM::NumericMetricDescriptor>(&abstract_metric_descriptor);
      numeric_descriptor != nullptr)
  {
    auto* type_attr = xml_document_->allocate_attribute("xsi:type", "pm:NumericMetricDescriptor");
    metric_node->append_attribute(type_attr);

    for (const auto& range : numeric_descriptor->technical_range)
    {
      auto* technical_range_node =
          xml_document_->allocate_node(rapidxml::node_element, "TechnicalRange");
      serialize(technical_range_node, range);
      metric_node->append_node(technical_range_node);
    }

    append_to_string_attribute(metric_node, "Resolution", numeric_descriptor->resolution);

    if (numeric_descriptor->averaging_period.has_value())
    {
      append_attribute(metric_node, "AveragingPeriod",
                       numeric_descriptor->averaging_period.value());
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
    for (const auto& value : enum_string_descriptor->allowed_value)
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
  if (range.step_width.has_value())
  {
    append_to_string_attribute(parent, "StepWidth", range.step_width.value());
  }
  if (range.relative_accuracy.has_value())
  {
    append_to_string_attribute(parent, "RelativeAccuracy", range.relative_accuracy.value());
  }
  if (range.absolute_accuracy.has_value())
  {
    append_to_string_attribute(parent, "AbsoluteAccuracy", range.absolute_accuracy.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdState& md_state)
{
  auto* md_state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MdState");
  if (md_state.state_version.has_value())
  {
    append_to_string_attribute(md_state_node, "StateVersion", md_state.state_version.value());
  }
  for (const auto& state : md_state.state)
  {
    auto* state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:State");
    if (const auto mds_state = dyn_cast<BICEPS::PM::MdsState>(state); mds_state != nullptr)
    {
      serialize(state_node, *mds_state);
    }
    else if (const auto system_context_state = dyn_cast<BICEPS::PM::SystemContextState>(state);
             system_context_state != nullptr)
    {
      serialize(state_node, *system_context_state);
    }
    else if (const auto channel_state = dyn_cast<BICEPS::PM::ChannelState>(state);
             channel_state != nullptr)
    {
      serialize(state_node, *channel_state);
    }
    else if (const auto sco_state = dyn_cast<BICEPS::PM::ScoState>(state); sco_state != nullptr)
    {
      serialize(state_node, *sco_state);
    }
    else if (const auto vmd_state = dyn_cast<BICEPS::PM::VmdState>(state); vmd_state != nullptr)
    {
      serialize(state_node, *vmd_state);
    }
    else if (const auto numeric_metric_state = dyn_cast<BICEPS::PM::NumericMetricState>(state);
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
  append_attribute(parent, "DescriptorHandle", state.descriptor_handle);

  if (state.state_version.has_value())
  {
    append_to_string_attribute(parent, "StateVersion", state.state_version.value());
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractDeviceComponentState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractState&>(state));

  if (state.activation_state.has_value())
  {
    append_attribute(parent, "ActivationState", to_string(state.activation_state.value()));
  }
  if (state.operating_hours.has_value())
  {
    append_to_string_attribute(parent, "OperatingHours", state.operating_hours.value());
  }
  if (state.operating_cycles.has_value())
  {
    append_to_string_attribute(parent, "OperatingCycles", state.operating_cycles.value());
  }
  if (state.calibration_info.has_value())
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:CalibrationInfo");
    serialize(node, state.calibration_info.value());
    parent->append_node(node);
  }
  if (state.next_calibration.has_value())
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:NextCalibration");
    serialize(node, state.next_calibration.value());
    parent->append_node(node);
  }
  if (state.physical_connector.has_value())
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:PhysicalConnector");
    serialize(node, state.physical_connector.value());
    parent->append_node(node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::CalibrationInfo& info)
{
  if (info.component_calibration_state.has_value())
  {
    append_attribute(parent, "ComponentCalibrationState",
                     to_string(info.component_calibration_state.value()));
  }
  if (info.type.has_value())
  {
    append_attribute(parent, "Type", to_string(info.type.value()));
  }
  if (info.time.has_value())
  {
    append_to_string_attribute(parent, "Time", info.time.value());
  }
  if (info.calibration_documentation.has_value())
  {
    auto* node =
        xml_document_->allocate_node(rapidxml::node_element, "pm:CalibrationDocumentation");
    serialize(node, info.calibration_documentation.value());
    parent->append_node(node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::CalibrationDocumentation& documentation)
{
  if (documentation.documentation.has_value())
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Documentation");
    serialize(node, documentation.documentation.value());
    parent->append_node(node);
  }
  if (documentation.calibration_result.has_value())
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:CalibrationResult");
    serialize(node, documentation.calibration_result.value());
    parent->append_node(node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::PhysicalConnectorInfo& info)
{
  if (info.number.has_value())
  {
    append_to_string_attribute(parent, "Number", info.number.value());
  }
  for (const auto& label : info.label)
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Label");
    serialize(node, label);
    parent->append_node(node);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SystemContextState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDeviceComponentState&>(state));
  append_attribute(parent, "xsi:type", "pm:SystemContextState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ChannelState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDeviceComponentState&>(state));
  append_attribute(parent, "xsi:type", "pm:ChannelState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::OperationGroup& operation_group)
{
  if (operation_group.operating_mode.has_value())
  {
    // TODO
    throw std::runtime_error("Not Implemented!");
  }
  if (operation_group.operations.has_value())
  {
    // TODO
    throw std::runtime_error("Not Implemented!");
  }
  auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:Type");
  serialize(node, operation_group.type);
  parent->append_node(node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::ScoState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDeviceComponentState&>(state));

  if (state.invocation_requested.has_value())
  {
    // TODO
    throw std::runtime_error("Not Implemented!");
  }
  if (state.invocation_required.has_value())
  {
    // TODO
    throw std::runtime_error("Not Implemented!");
  }
  if (state.operation_group.has_value())
  {
    auto* node = xml_document_->allocate_node(rapidxml::node_element, "pm:OperatingJurisdiction");
    serialize(node, state.operation_group.value());
    parent->append_node(node);
  }

  append_attribute(parent, "xsi:type", "pm:ScoState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractComplexDeviceComponentState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractDeviceComponentState&>(state));
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdsState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractComplexDeviceComponentState&>(state));
  if (state.lang.has_value())
  {
    append_attribute(parent, "Lang", state.lang.value());
  }
  if (state.operating_mode.has_value())
  {
    append_attribute(parent, "OperatingMode", to_string(state.operating_mode.value()));
  }

  if (state.operating_jurisdiction.has_value())
  {
    auto* operating_jurisdiction_parent =
        xml_document_->allocate_node(rapidxml::node_element, "pm:OperatingJurisdiction");
    serialize(operating_jurisdiction_parent, state.operating_jurisdiction.value());
    parent->append_node(operating_jurisdiction_parent);
  }
  append_attribute(parent, "xsi:type", "pm:MdsState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::VmdState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractComplexDeviceComponentState&>(state));

  if (state.operating_jurisdiction.has_value())
  {
    auto* operating_jurisdiction_parent =
        xml_document_->allocate_node(rapidxml::node_element, "pm:OperatingJurisdiction");
    serialize(operating_jurisdiction_parent, state.operating_jurisdiction.value());
    parent->append_node(operating_jurisdiction_parent);
  }
  append_attribute(parent, "xsi:type", "pm:VmdState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractMetricState& metric_state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractState&>(metric_state));
  if (metric_state.activation_state.has_value())
  {
    append_attribute(parent, "ActivationState", to_string(metric_state.activation_state.value()));
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::NumericMetricState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricState&>(state));
  if (state.active_averaging_period.has_value())
  {
    append_attribute(parent, "ActiveAveragingPeriod", state.active_averaging_period.value());
  }
  if (state.metric_value.has_value())
  {
    auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
    serialize(value_node, state.metric_value.value());
    parent->append_node(value_node);
  }
  for (const auto& range : state.physiological_range)
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
  if (state.metric_value.has_value())
  {
    auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
    serialize(value_node, state.metric_value.value());
    parent->append_node(value_node);
  }
  append_attribute(parent, "xsi:type", "pm:StringMetricState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::EnumStringMetricState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractMetricState&>(state));
  if (state.metric_value.has_value())
  {
    auto* value_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MetricValue");
    serialize(value_node, state.metric_value.value());
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
  if (state.binding_mdib_version.has_value())
  {
    append_to_string_attribute(parent, "BindingMdibVersion", state.binding_mdib_version.value());
  }
  if (state.context_association.has_value())
  {
    append_attribute(parent, "ContextAssociation", to_string(state.context_association.value()));
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
  if (state.location_detail.has_value())
  {
    serialize(parent, state.location_detail.value());
  }
  append_attribute(parent, "xsi:type", "pm:LocationContextState");
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractOperationState& state)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractState&>(state));
  append_attribute(parent, "OperatingMode", to_string(state.operating_mode));
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

  for (const auto& allowed : state.allowed_values)
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
  if (location_detail.poc.has_value())
  {
    append_attribute(location_detail_node, "PoC", location_detail.poc.value());
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
  serialize(parent, value.metric_quality);
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
  address_node->value(subscribe_response.subscription_manager.address.c_str());
  subscription_manager_node->append_node(address_node);

  serialize(subscription_manager_node,
            subscribe_response.subscription_manager.reference_parameters.value());

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
  if (set_response.mdib_version_group.mdib_version.has_value())
  {
    append_to_string_attribute(set_value_response_node, "MdibVersion",
                               set_response.mdib_version_group.mdib_version.value());
  }
  append_attribute(set_value_response_node, "SequenceId",
                   set_response.mdib_version_group.sequence_id);
  if (set_response.mdib_version_group.instance_id.has_value())
  {
    append_to_string_attribute(set_value_response_node, "InstanceId",
                               set_response.mdib_version_group.instance_id.value());
  }
  serialize(set_value_response_node, set_response.invocation_info);
  parent->append_node(set_value_response_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::InvocationInfo& invocation_info)
{
  auto* invocation_info_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:InvocationInfo");

  auto* transaction_id =
      xml_document_->allocate_string(std::to_string(invocation_info.transaction_id).c_str());
  auto* transaction_id_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:TransactionId", transaction_id);
  invocation_info_node->append_node(transaction_id_node);

  auto* invocation_state =
      xml_document_->allocate_string(to_string(invocation_info.invocation_state).c_str());
  auto* invocation_state_node =
      xml_document_->allocate_node(rapidxml::node_element, "msg:InvocationState", invocation_state);
  invocation_info_node->append_node(invocation_state_node);

  if (invocation_info.invocation_error.has_value())
  {
    auto* invocation_error =
        xml_document_->allocate_string(to_string(invocation_info.invocation_error.value()).c_str());
    auto* invocation_error_node = xml_document_->allocate_node(
        rapidxml::node_element, "msg:InvocationError", invocation_error);
    invocation_info_node->append_node(invocation_error_node);
  }

  if (invocation_info.invocation_error_message.has_value())
  {
    auto* invocation_error_message =
        xml_document_->allocate_string(invocation_info.invocation_error_message.value().c_str());
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
  if (report.mdib_version_group.mdib_version.has_value())
  {
    append_to_string_attribute(report_node, "MdibVersion",
                               report.mdib_version_group.mdib_version.value());
  }
  for (const auto& part : report.report_part)
  {
    serialize(report_node, part);
  }
  parent->append_node(report_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::EpisodicComponentReport& report)
{
  auto* report_node =
      xml_document_->allocate_node(rapidxml::node_element, "mm:EpisodicComponentReport");
  if (report.mdib_version_group.mdib_version.has_value())
  {
    append_to_string_attribute(report_node, "MdibVersion",
                               report.mdib_version_group.mdib_version.value());
  }
  for (const auto& part : report.report_part)
  {
    serialize(report_node, part);
  }
  parent->append_node(report_node);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::MetricReportPart& part)
{
  auto* report_part_node = xml_document_->allocate_node(rapidxml::node_element, "mm:ReportPart");
  for (const auto& state : part.metric_state)
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
                                  const BICEPS::MM::ComponentReportPart& part)
{
  auto* report_part_node = xml_document_->allocate_node(rapidxml::node_element, "mm:ReportPart");
  for (const auto& state : part.component_state)
  {
    auto* state_node = xml_document_->allocate_node(rapidxml::node_element, "pm:State");
    if (const auto system_context_state = dyn_cast<const BICEPS::PM::SystemContextState>(state);
        system_context_state != nullptr)
    {
      serialize(state_node, *system_context_state);
    }
    else if (const auto channel_state = dyn_cast<const BICEPS::PM::ChannelState>(state);
             channel_state != nullptr)
    {
      serialize(state_node, *channel_state);
    }
    else if (const auto sco_state = dyn_cast<const BICEPS::PM::ScoState>(state);
             sco_state != nullptr)
    {
      serialize(state_node, *sco_state);
    }
    else if (const auto mds_state = dyn_cast<const BICEPS::PM::MdsState>(state);
             mds_state != nullptr)
    {
      serialize(state_node, *mds_state);
    }
    else if (const auto vmd_state = dyn_cast<const BICEPS::PM::VmdState>(state);
             vmd_state != nullptr)
    {
      serialize(state_node, *vmd_state);
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
  append_attribute(parent, "OperationTarget", operation.operation_target);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SetStringOperationDescriptor& operation)
{
  serialize(parent, static_cast<const BICEPS::PM::AbstractOperationDescriptor&>(operation));
  append_attribute(parent, "xsi:type", "pm:SetStringOperationDescriptor");
  if (operation.max_length.has_value())
  {
    append_to_string_attribute(parent, "MaxLength", operation.max_length.value());
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
  append_to_string_attribute(parent, "MeasuredValue", measurement.measured_value);
  auto* unit_node = xml_document_->allocate_node(rapidxml::node_element, "pm:MeasurementUnit");
  serialize(unit_node, measurement.measurement_unit);
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
    case BICEPS::PM::SafetyClassification::INF:
      return "Inf";
    case BICEPS::PM::SafetyClassification::MED_A:
      return "MedA";
    case BICEPS::PM::SafetyClassification::MED_B:
      return "MedB";
    case BICEPS::PM::SafetyClassification::MED_C:
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
    case BICEPS::PM::LocalizedTextWidth::XS:
      return "xs";
    case BICEPS::PM::LocalizedTextWidth::S:
      return "s";
    case BICEPS::PM::LocalizedTextWidth::M:
      return "m";
    case BICEPS::PM::LocalizedTextWidth::L:
      return "l";
    case BICEPS::PM::LocalizedTextWidth::XL:
      return "xl";
    case BICEPS::PM::LocalizedTextWidth::XXL:
      return "xxl";
  }
  assert(false && "Uncatched value in LocalizedTextWidth");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::MetricCategory category)
{
  switch (category)
  {
    case BICEPS::PM::MetricCategory::UNSPEC:
      return "Unspec";
    case BICEPS::PM::MetricCategory::MSRMT:
      return "Msrmt";
    case BICEPS::PM::MetricCategory::CLC:
      return "Clc";
    case BICEPS::PM::MetricCategory::SET:
      return "Set";
    case BICEPS::PM::MetricCategory::PRESET:
      return "Preset";
    case BICEPS::PM::MetricCategory::RCMM:
      return "Rcmm";
  }
  assert(false && "Uncatched value in MetricCategory");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::MetricAvailability availability)
{
  switch (availability)
  {
    case BICEPS::PM::MetricAvailability::INTR:
      return "Intr";
    case BICEPS::PM::MetricAvailability::CONT:
      return "Cont";
  }
  assert(false && "Uncatched value in MetricAvailability");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::MeasurementValidity validity)
{
  switch (validity)
  {
    case BICEPS::PM::MeasurementValidity::CALIB:
      return "Calib";
    case BICEPS::PM::MeasurementValidity::INV:
      return "Inv";
    case BICEPS::PM::MeasurementValidity::NA:
      return "NA";
    case BICEPS::PM::MeasurementValidity::OFLW:
      return "Oflw";
    case BICEPS::PM::MeasurementValidity::ONG:
      return "Ong";
    case BICEPS::PM::MeasurementValidity::QST:
      return "Qst";
    case BICEPS::PM::MeasurementValidity::UFLW:
      return "Uflw";
    case BICEPS::PM::MeasurementValidity::VLD:
      return "Vld";
    case BICEPS::PM::MeasurementValidity::VLDATED:
      return "Vldated";
  }
  assert(false && "Uncatched value in MeasurementValidity");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::MM::InvocationState invocation_state)
{
  switch (invocation_state)
  {
    case BICEPS::MM::InvocationState::WAIT:
      return "Wait";
    case BICEPS::MM::InvocationState::START:
      return "Start";
    case BICEPS::MM::InvocationState::CNCLLD:
      return "Cnclld";
    case BICEPS::MM::InvocationState::CNCLLD_MAN:
      return "CnclldMan";
    case BICEPS::MM::InvocationState::FIN:
      return "Fin";
    case BICEPS::MM::InvocationState::FIN_MOD:
      return "FinMod";
    case BICEPS::MM::InvocationState::FAIL:
      return "Fail";
  }
  assert(false && "Uncatched value in InvocationState");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::MM::InvocationError invocation_error)
{
  switch (invocation_error)
  {
    case BICEPS::MM::InvocationError::UNSPEC:
      return "Unspec";
    case BICEPS::MM::InvocationError::UNKN:
      return "Unkn";
    case BICEPS::MM::InvocationError::INV:
      return "Inv";
    case BICEPS::MM::InvocationError::OTH:
      return "Oth";
  }
  assert(false && "Uncatched value in InvocationError");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::ContextAssociation context_association)
{
  switch (context_association)
  {
    case BICEPS::PM::ContextAssociation::NO:
      return "No";
    case BICEPS::PM::ContextAssociation::PRE:
      return "Pre";
    case BICEPS::PM::ContextAssociation::ASSOC:
      return "Assoc";
    case BICEPS::PM::ContextAssociation::DIS:
      return "Dis";
  }
  assert(false && "Uncatched value in ContextAssociation");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::OperatingMode operating_mode)
{
  switch (operating_mode)
  {
    case BICEPS::PM::OperatingMode::DIS:
      return "Dis";
    case BICEPS::PM::OperatingMode::EN:
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
    case BICEPS::PM::ComponentActivation::ON:
      return "On";
    case BICEPS::PM::ComponentActivation::NOT_RDY:
      return "NotRdy";
    case BICEPS::PM::ComponentActivation::STND_BY:
      return "StndBy";
    case BICEPS::PM::ComponentActivation::OFF:
      return "Off";
    case BICEPS::PM::ComponentActivation::SHTDN:
      return "Shtdn";
    case BICEPS::PM::ComponentActivation::FAIL:
      return "Fail";
  }
  assert(false && "Uncatched value in ComponentActivation");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::MdsOperatingMode operating_mode)
{
  switch (operating_mode)
  {
    case BICEPS::PM::MdsOperatingMode::NML:
      return "Nml";
    case BICEPS::PM::MdsOperatingMode::DMO:
      return "Dmo";
    case BICEPS::PM::MdsOperatingMode::SRV:
      return "Srv";
    case BICEPS::PM::MdsOperatingMode::MTN:
      return "Mtn";
  }
  assert(false && "Uncatched value in MdsOperatingMode");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::CalibrationState calib_state)
{
  switch (calib_state)
  {
    case BICEPS::PM::CalibrationState::NO:
      return "No";
    case BICEPS::PM::CalibrationState::REQ:
      return "Req";
    case BICEPS::PM::CalibrationState::RUN:
      return "Run";
    case BICEPS::PM::CalibrationState::CAL:
      return "Cal";
    case BICEPS::PM::CalibrationState::OTH:
      return "Oth";
  }
  assert(false && "Uncatched value in MdsOperatingMode");
  return "";
}

std::string MessageSerializer::to_string(BICEPS::PM::CalibrationType calib_type)
{
  switch (calib_type)
  {
    case BICEPS::PM::CalibrationType::OFFSET:
      return "Offset";
    case BICEPS::PM::CalibrationType::GAIN:
      return "Gain";
    case BICEPS::PM::CalibrationType::TP:
      return "Tp";
    case BICEPS::PM::CalibrationType::UNSPEC:
      return "Unspec";
  }
  assert(false && "Uncatched value in MdsOperatingMode");
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
