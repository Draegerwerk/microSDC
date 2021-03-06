#include "MessageSerializer.hpp"
#include "Casting.hpp"
#include "datamodel/MDPWSConstants.hpp"
#include "rapidxml_print.hpp"

MessageSerializer::MessageSerializer()
  : xmlDocument_(std::make_unique<rapidxml::xml_document<>>())
{
  auto* declaration = xmlDocument_->allocate_node(rapidxml::node_declaration);
  auto* version = xmlDocument_->allocate_attribute("version", "1.0");
  auto* encoding = xmlDocument_->allocate_attribute("encoding", "utf-8");
  declaration->append_attribute(version);
  declaration->append_attribute(encoding);
  xmlDocument_->append_node(declaration);
}

std::string MessageSerializer::str() const
{
  std::string out;
  rapidxml::print(std::back_inserter(out), *xmlDocument_, rapidxml::print_no_indenting);
  return out;
}

void MessageSerializer::serialize(const MESSAGEMODEL::Envelope& message)
{
  serialize(&*xmlDocument_, message);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const MESSAGEMODEL::Envelope& message)
{
  auto* envelope = xmlDocument_->allocate_node(rapidxml::node_element, "soap:Envelope");

  auto* xmlnsSoap = xmlDocument_->allocate_attribute("xmlns:soap", MDPWS::WS_NS_SOAP_ENVELOPE);
  auto* xmlnsWsd = xmlDocument_->allocate_attribute("xmlns:wsd", MDPWS::WS_NS_DISCOVERY);
  auto* xmlnsWsa = xmlDocument_->allocate_attribute("xmlns:wsa", MDPWS::WS_NS_ADDRESSING);
  auto* xmlnsWse = xmlDocument_->allocate_attribute("xmlns:wse", MDPWS::WS_NS_EVENTING);
  auto* xmlnsDpws = xmlDocument_->allocate_attribute("xmlns:dpws", MDPWS::WS_NS_DPWS);
  auto* xmlnsMdpws = xmlDocument_->allocate_attribute("xmlns:mdpws", MDPWS::NS_MDPWS);
  auto* xmlnsMex = xmlDocument_->allocate_attribute("xmlns:mex", MDPWS::WS_NS_METADATA_EXCHANGE);
  auto* xmlnsGlue = xmlDocument_->allocate_attribute("xmlns:glue", SDC::NS_GLUE);
  auto* xmlnsMm = xmlDocument_->allocate_attribute("xmlns:mm", SDC::NS_BICEPS_MESSAGE_MODEL);
  auto* xmlnsPm = xmlDocument_->allocate_attribute("xmlns:pm", SDC::NS_BICEPS_PARTICIPANT_MODEL);
  auto* xmlnsExt = xmlDocument_->allocate_attribute("xmlns:ext", SDC::NS_BICEPS_EXTENSION);
  auto* xmlnsXsi =
      xmlDocument_->allocate_attribute("xmlns:xsi", MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE);

  envelope->append_attribute(xmlnsSoap);
  envelope->append_attribute(xmlnsWsd);
  envelope->append_attribute(xmlnsWsa);
  envelope->append_attribute(xmlnsWse);
  envelope->append_attribute(xmlnsDpws);
  envelope->append_attribute(xmlnsMdpws);
  envelope->append_attribute(xmlnsMex);
  envelope->append_attribute(xmlnsGlue);
  envelope->append_attribute(xmlnsMm);
  envelope->append_attribute(xmlnsPm);
  envelope->append_attribute(xmlnsExt);
  envelope->append_attribute(xmlnsXsi);

  serialize(envelope, message.header);
  serialize(envelope, message.body);

  parent->append_node(envelope);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Header& header)
{
  auto* headerNode = xmlDocument_->allocate_node(rapidxml::node_element, "soap:Header");
  // Mandatory action element
  auto* actionNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:Action");
  actionNode->value(header.action.c_str());
  headerNode->append_node(actionNode);
  // optionals
  if (header.messageID.has_value())
  {
    auto* messageIdNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:MessageID");
    messageIdNode->value(header.messageID->c_str());
    headerNode->append_node(messageIdNode);
  }
  if (header.to.has_value())
  {
    auto* toNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:To");
    toNode->value(header.to->c_str());
    headerNode->append_node(toNode);
  }
  if (header.appSequence.has_value())
  {
    serialize(headerNode, header.appSequence.value());
  }
  if (header.relatesTo.has_value())
  {
    serialize(headerNode, header.relatesTo.value());
  }
  parent->append_node(headerNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Body& body)
{
  auto* bodyNode = xmlDocument_->allocate_node(rapidxml::node_element, "soap:Body");
  if (body.hello.has_value())
  {
    serialize(bodyNode, body.hello.value());
  }
  else if (body.bye.has_value())
  {
    serialize(bodyNode, body.bye.value());
  }
  else if (body.probeMatches.has_value())
  {
    serialize(bodyNode, body.probeMatches.value());
  }
  else if (body.resolveMatches.has_value())
  {
    serialize(bodyNode, body.resolveMatches.value());
  }
  else if (body.metadata.has_value())
  {
    serialize(bodyNode, body.metadata.value());
  }
  else if (body.getMdibResponse.has_value())
  {
    serialize(bodyNode, body.getMdibResponse.value());
  }
  else if (body.subscribeResponse.has_value())
  {
    serialize(bodyNode, body.subscribeResponse.value());
  }
  else if (body.renewResponse.has_value())
  {
    serialize(bodyNode, body.renewResponse.value());
  }
  else if (body.episodicMetricReport.has_value())
  {
    serialize(bodyNode, body.episodicMetricReport.value());
  }
  else if (body.setValueResponse.has_value())
  {
    serialize(bodyNode, body.setValueResponse.value());
  }
  parent->append_node(bodyNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::RelatesToType& relatesTo)
{
  auto* relatesToNode = xmlDocument_->allocate_node(rapidxml::node_element, "mdpws:RelatesTo");
  relatesToNode->value(relatesTo.c_str());
  parent->append_node(relatesToNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::EndpointReferenceType& endpointReference)
{
  auto* eprNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:EndpointReference");
  auto* addressNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:Address");
  addressNode->value(endpointReference.address.c_str());
  eprNode->append_node(addressNode);
  parent->append_node(eprNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::AppSequenceType& appSequence)
{
  auto* appSequenceNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:AppSequence");
  const auto instanceIdStr = std::to_string(appSequence.instanceId);
  auto* instanceId = xmlDocument_->allocate_string(instanceIdStr.c_str());
  auto* instanceIdAttr = xmlDocument_->allocate_attribute("InstanceId", instanceId);
  appSequenceNode->append_attribute(instanceIdAttr);
  if (appSequence.sequenceId.has_value())
  {
    auto* sequenceId = xmlDocument_->allocate_string(appSequence.sequenceId->c_str());
    auto* sequenceIdAttr = xmlDocument_->allocate_attribute("SequenceId", sequenceId);
    appSequenceNode->append_attribute(sequenceIdAttr);
  }
  auto messageNumberStr = std::to_string(appSequence.messageNumber);
  auto* messageNumber = xmlDocument_->allocate_string(messageNumberStr.c_str());
  auto* messageNumberAttr = xmlDocument_->allocate_attribute("MessageNumber", messageNumber);
  appSequenceNode->append_attribute(messageNumberAttr);
  parent->append_node(appSequenceNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ScopesType& scopes)
{
  auto* scopesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Scopes");
  if (scopes.matchBy.has_value())
  {
    auto* matchByAttr = xmlDocument_->allocate_attribute("MatchBy", scopes.matchBy->c_str());
    scopesNode->append_attribute(matchByAttr);
  }
  const auto scopesStr = toString(scopes);
  auto* uriList = xmlDocument_->allocate_string(scopesStr.c_str());
  scopesNode->value(uriList);
  parent->append_node(scopesNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::HelloType& hello)
{
  auto* helloNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Hello");
  serialize(helloNode, hello.endpointReference);
  if (hello.types.has_value())
  {
    auto* typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* typesStr = xmlDocument_->allocate_string(toString(hello.types.value()).c_str());
    typesNode->value(typesStr);
    helloNode->append_node(typesNode);
  }
  if (hello.scopes.has_value())
  {
    serialize(helloNode, hello.scopes.value());
  }
  if (hello.xAddrs.has_value())
  {
    auto* xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* xAddrsStr = xmlDocument_->allocate_string(toString(hello.xAddrs.value()).c_str());
    xAddrsNode->value(xAddrsStr);
    helloNode->append_node(xAddrsNode);
  }
  auto* metadataVersionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadataVersion =
      xmlDocument_->allocate_string(std::to_string(hello.metadataVersion).c_str());
  metadataVersionNode->value(metadataVersion);
  parent->append_node(helloNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ByeType& bye)
{
  auto* byeNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:bye");
  serialize(byeNode, bye.endpointReference);
  if (bye.types.has_value())
  {
    auto* typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* typesStr = xmlDocument_->allocate_string(toString(bye.types.value()).c_str());
    typesNode->value(typesStr);
    byeNode->append_node(typesNode);
  }
  if (bye.scopes.has_value())
  {
    serialize(byeNode, bye.scopes.value());
  }
  if (bye.xAddrs.has_value())
  {
    auto* xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* xAddrsStr = xmlDocument_->allocate_string(toString(bye.xAddrs.value()).c_str());
    xAddrsNode->value(xAddrsStr);
    byeNode->append_node(xAddrsNode);
  }
  if (bye.metadataVersion.has_value())
  {
    auto* metadataVersionNode =
        xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
    auto* metadataVersion =
        xmlDocument_->allocate_string(std::to_string(bye.metadataVersion.value()).c_str());
    metadataVersionNode->value(metadataVersion);
  }
  parent->append_node(byeNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchType& probeMatch)
{
  auto* probeMatchNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ProbeMatch");
  serialize(probeMatchNode, probeMatch.endpointReference);
  if (probeMatch.types.has_value())
  {
    auto* typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* typesStr = xmlDocument_->allocate_string(toString(probeMatch.types.value()).c_str());
    typesNode->value(typesStr);
    probeMatchNode->append_node(typesNode);
  }
  if (probeMatch.scopes.has_value())
  {
    serialize(probeMatchNode, probeMatch.scopes.value());
  }
  if (probeMatch.xAddrs.has_value())
  {
    auto* xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* xAddrsStr = xmlDocument_->allocate_string(toString(probeMatch.xAddrs.value()).c_str());
    xAddrsNode->value(xAddrsStr);
    probeMatchNode->append_node(xAddrsNode);
  }
  auto* metadataVersionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadataVersion =
      xmlDocument_->allocate_string(std::to_string(probeMatch.metadataVersion).c_str());
  metadataVersionNode->value(metadataVersion);
  probeMatchNode->append_node(metadataVersionNode);
  parent->append_node(probeMatchNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchesType& probeMatches)
{
  auto* probeMatchesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ProbeMatches");
  for (const auto& probeMatch : probeMatches.probeMatch)
  {
    serialize(probeMatchesNode, probeMatch);
  }
  parent->append_node(probeMatchesNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchType& resolveMatch)
{
  auto* resolveMatchNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  serialize(resolveMatchNode, resolveMatch.endpointReference);
  if (resolveMatch.types.has_value())
  {
    auto* typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto* typesStr = xmlDocument_->allocate_string(toString(resolveMatch.types.value()).c_str());
    typesNode->value(typesStr);
    resolveMatchNode->append_node(typesNode);
  }
  if (resolveMatch.scopes.has_value())
  {
    serialize(resolveMatchNode, resolveMatch.scopes.value());
  }
  if (resolveMatch.xAddrs.has_value())
  {
    auto* xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto* xAddrsStr = xmlDocument_->allocate_string(toString(resolveMatch.xAddrs.value()).c_str());
    xAddrsNode->value(xAddrsStr);
    resolveMatchNode->append_node(xAddrsNode);
  }
  auto* metadataVersionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto* metadataVersion =
      xmlDocument_->allocate_string(std::to_string(resolveMatch.metadataVersion).c_str());
  metadataVersionNode->value(metadataVersion);
  resolveMatchNode->append_node(metadataVersionNode);
  parent->append_node(resolveMatchNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchesType& resolveMatches)
{
  auto* resolveMatchesNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  for (const auto& resolveMatch : resolveMatches.resolveMatch)
  {
    serialize(resolveMatchesNode, resolveMatch);
  }
  parent->append_node(resolveMatchesNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::MEX::Metadata& metadata)
{
  auto* metadataNode = xmlDocument_->allocate_node(rapidxml::node_element, "mex:Metadata");
  for (const auto& metadataSection : metadata.metadataSection)
  {
    serialize(metadataNode, metadataSection);
  }
  parent->append_node(metadataNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::MEX::MetadataSection& metadataSection)
{
  auto* metadataSectionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "mex:MetadataSection");
  auto* dialectAttr = xmlDocument_->allocate_attribute("Dialect", metadataSection.dialect.c_str());
  metadataSectionNode->append_attribute(dialectAttr);
  if (metadataSection.thisModel.has_value())
  {
    serialize(metadataSectionNode, metadataSection.thisModel.value());
  }
  else if (metadataSection.thisDevice.has_value())
  {
    serialize(metadataSectionNode, metadataSection.thisDevice.value());
  }
  else if (metadataSection.relationship.has_value())
  {
    serialize(metadataSectionNode, metadataSection.relationship.value());
  }
  else if (metadataSection.location.has_value())
  {
    auto* locationNode = xmlDocument_->allocate_node(rapidxml::node_element, "mex:Location");
    locationNode->value(metadataSection.location->c_str());
    metadataSectionNode->append_node(locationNode);
  }
  parent->append_node(metadataSectionNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::ThisModelType& thisModel)
{
  auto* thisModelNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ThisModel");
  // Manufacturer
  for (const auto& manufacturer : thisModel.manufacturer)
  {
    auto* manufacturerNode =
        xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Manufacturer");
    manufacturerNode->value(manufacturer.c_str());
    thisModelNode->append_node(manufacturerNode);
  }
  // ModelName
  for (const auto& modelName : thisModel.modelName)
  {
    auto* modelNameNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ModelName");
    modelNameNode->value(modelName.c_str());
    thisModelNode->append_node(modelNameNode);
  }
  parent->append_node(thisModelNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::ThisDeviceType& thisDevice)
{
  auto* thisDeviceNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ThisDevice");
  // FriendlyName
  for (const auto& friendlyName : thisDevice.friendlyName)
  {
    auto* friendlyNameNode =
        xmlDocument_->allocate_node(rapidxml::node_element, "dpws:FriendlyName");
    friendlyNameNode->value(friendlyName.c_str());
    thisDeviceNode->append_node(friendlyNameNode);
  }
  parent->append_node(thisDeviceNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::Relationship& relationship)
{
  auto* relationshipNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Relationship");
  auto* typeAttr = xmlDocument_->allocate_attribute("Type", relationship.type.c_str());
  relationshipNode->append_attribute(typeAttr);

  serialize(relationshipNode, relationship.host);
  for (const auto& hosted : relationship.hosted)
  {
    serialize(relationshipNode, hosted);
  }
  parent->append_node(relationshipNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::HostServiceType& host)
{
  auto* hostNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Host");
  serialize(hostNode, host.endpointReference);
  if (host.types.has_value())
  {
    auto* typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Types");
    auto* typesStr = xmlDocument_->allocate_string(toString(host.types.value()).c_str());
    typesNode->value(typesStr);
    hostNode->append_node(typesNode);
  }
  parent->append_node(hostNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::HostedServiceType& hosted)
{
  auto* hostedNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Hosted");
  for (const auto& epr : hosted.endpointReference)
  {
    serialize(hostedNode, epr);
  }
  // Types
  auto* typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Types");
  auto* typesStr = xmlDocument_->allocate_string(toString(hosted.types).c_str());
  typesNode->value(typesStr);
  hostedNode->append_node(typesNode);
  // ServiceId
  auto* serviceIdNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ServiceId");
  serviceIdNode->value(hosted.serviceId.c_str());
  hostedNode->append_node(serviceIdNode);
  parent->append_node(hostedNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::GetMdibResponse& getMdibResponse)
{
  auto* getMdibResponseNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "mm:GetMdibResponse");
  auto* sequenceIdAttr = xmlDocument_->allocate_attribute("SequenceId", "0");
  getMdibResponseNode->append_attribute(sequenceIdAttr);
  serialize(getMdibResponseNode, getMdibResponse.mdib);
  parent->append_node(getMdibResponseNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Mdib& mdib)
{
  auto* mdibNode = xmlDocument_->allocate_node(rapidxml::node_element, "mm:Mdib");
  auto* sequenceId = xmlDocument_->allocate_string(mdib.sequenceId.c_str());
  auto* sequenceIdAttr = xmlDocument_->allocate_attribute("SequenceId", sequenceId);
  mdibNode->append_attribute(sequenceIdAttr);
  auto* mdibVersion =
      xmlDocument_->allocate_string(std::to_string(mdib.mdibVersion.value_or(0)).c_str());
  auto* mdibVersionAttr = xmlDocument_->allocate_attribute(mdibVersion);
  mdibNode->append_attribute(mdibVersionAttr);
  if (mdib.mdDescription.has_value())
  {
    serialize(mdibNode, mdib.mdDescription.value());
  }
  if (mdib.mdState.has_value())
  {
    serialize(mdibNode, mdib.mdState.value());
  }
  parent->append_node(mdibNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdDescription& mdDescription)
{
  auto* mdDescriptionNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:MdDescription");
  for (const auto& md : mdDescription.mds)
  {
    serialize(mdDescriptionNode, md);
  }
  parent->append_node(mdDescriptionNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MdsDescriptor& mdsDescriptor)
{
  auto* mdsDescriptorNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Mds");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", mdsDescriptor.handle.c_str());
  mdsDescriptorNode->append_attribute(handleAttr);
  if (mdsDescriptor.metaData.has_value())
  {
    serialize(mdsDescriptorNode, mdsDescriptor.metaData.value());
  }
  if (mdsDescriptor.systemContext.has_value())
  {
    serialize(mdsDescriptorNode, mdsDescriptor.systemContext.value());
  }
  for (const auto& vmd : mdsDescriptor.vmd)
  {
    serialize(mdsDescriptorNode, vmd);
  }
  parent->append_node(mdsDescriptorNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::Metadata& metadata)
{
  auto* metaDataNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:MetaData");

  for (const auto& modelName : metadata.modelName)
  {
    auto* modelNameNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:ModelName");
    auto* refAttr = xmlDocument_->allocate_attribute("Ref", modelName.c_str());
    modelNameNode->append_attribute(refAttr);
    metaDataNode->append_node(modelNameNode);
  }
  if (metadata.modelNumber.has_value())
  {
    auto* modelNumberNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:ModelNumber");
    modelNumberNode->value(metadata.modelNumber.value().c_str());
    metaDataNode->append_node(modelNumberNode);
  }
  for (const auto& serialNumber : metadata.serialNumber)
  {
    auto* serialNumberNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:SerialNumber");
    serialNumberNode->value(serialNumber.c_str());
    metaDataNode->append_node(serialNumberNode);
  }
  for (const auto& manufacturer : metadata.manufacturer)
  {
    auto* manufacturerNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Manufacturer");
    auto* refAttr = xmlDocument_->allocate_attribute("Ref", manufacturer.c_str());
    manufacturerNode->append_attribute(refAttr);
    metaDataNode->append_node(manufacturerNode);
  }
  parent->append_node(metaDataNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::SystemContextDescriptor& systemContext)
{
  auto* systemContextNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:SystemContext");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", systemContext.handle.c_str());
  systemContextNode->append_attribute(handleAttr);
  if (systemContext.patientContext.has_value())
  {
    serialize(systemContextNode, systemContext.patientContext.value());
  }
  if (systemContext.locationContext.has_value())
  {
    serialize(systemContextNode, systemContext.locationContext.value());
  }
  parent->append_node(systemContextNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::PatientContextDescriptor& patientContext)
{
  auto* patientContextNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "pm:PatientContext");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", patientContext.handle.c_str());
  patientContextNode->append_attribute(handleAttr);
  if (patientContext.safetyClassification.has_value())
  {
    auto* safetyClassification = xmlDocument_->allocate_string(
        toString(patientContext.safetyClassification.value()).c_str());
    auto* safetyClassificationAttr =
        xmlDocument_->allocate_attribute("SafetyClassification", safetyClassification);
    patientContextNode->append_attribute(safetyClassificationAttr);
  }
  parent->append_node(patientContextNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationContextDescriptor& locationContext)
{
  auto* patientContextNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "pm:LocationContext");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", locationContext.handle.c_str());
  patientContextNode->append_attribute(handleAttr);
  if (locationContext.safetyClassification.has_value())
  {
    auto* safetyClassification = xmlDocument_->allocate_string(
        toString(locationContext.safetyClassification.value()).c_str());
    auto* safetyClassificationAttr =
        xmlDocument_->allocate_attribute("SafetyClassification", safetyClassification);
    patientContextNode->append_attribute(safetyClassificationAttr);
  }
  parent->append_node(patientContextNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::VmdDescriptor& vmd)
{
  auto* vmdNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Vmd");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", vmd.handle.c_str());
  vmdNode->append_attribute(handleAttr);
  for (const auto& channel : vmd.channel)
  {
    serialize(vmdNode, channel);
  }
  if (vmd.sco.has_value())
  {
    serialize(vmdNode, vmd.sco.value());
  }
  parent->append_node(vmdNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ChannelDescriptor& channel)
{
  auto* channelNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Channel");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", channel.handle.c_str());
  channelNode->append_attribute(handleAttr);
  if (channel.descriptorVersion.has_value())
  {
    auto* descriptorVersion =
        xmlDocument_->allocate_string(std::to_string(channel.descriptorVersion.value()).c_str());
    auto* descriptorVersionAttr =
        xmlDocument_->allocate_attribute("DescriptorVersion", descriptorVersion);
    channelNode->append_attribute(descriptorVersionAttr);
  }
  if (channel.safetyClassification.has_value())
  {
    auto* safetyClassification =
        xmlDocument_->allocate_string(toString(channel.safetyClassification.value()).c_str());
    auto* safetyClassificationAttr =
        xmlDocument_->allocate_attribute("SafetyClassification", safetyClassification);
    channelNode->append_attribute(safetyClassificationAttr);
  }
  if (channel.type.has_value())
  {
    auto* typeNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Type");
    auto* codeAttr = xmlDocument_->allocate_attribute("Code", channel.type.value().code.c_str());
    typeNode->append_attribute(codeAttr);
    channelNode->append_node(typeNode);
  }
  for (const auto& metric : channel.metric)
  {
    serialize(channelNode, *metric);
  }
  parent->append_node(channelNode);
}

void MessageSerializer::serialize(
    rapidxml::xml_node<>* parent,
    const BICEPS::PM::AbstractMetricDescriptor& abstractMetricDescriptor)
{
  auto* metricNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Metric");
  auto* handleAttr =
      xmlDocument_->allocate_attribute("Handle", abstractMetricDescriptor.handle.c_str());
  metricNode->append_attribute(handleAttr);

  auto* descriptorVersion = xmlDocument_->allocate_string(
      std::to_string(abstractMetricDescriptor.descriptorVersion.value_or(0)).c_str());
  auto* descriptorVersionAttr =
      xmlDocument_->allocate_attribute("DescriptorVersion", descriptorVersion);
  metricNode->append_attribute(descriptorVersionAttr);

  if (abstractMetricDescriptor.safetyClassification.has_value())
  {
    auto* safetyClassification = xmlDocument_->allocate_string(
        toString(abstractMetricDescriptor.safetyClassification.value()).c_str());
    auto* safetyClassificationAttr =
        xmlDocument_->allocate_attribute("SafetyClassification", safetyClassification);
    metricNode->append_attribute(safetyClassificationAttr);
  }

  auto* unitNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Unit");
  auto* unitCodeAttr =
      xmlDocument_->allocate_attribute("Code", abstractMetricDescriptor.unit.code.c_str());
  unitNode->append_attribute(unitCodeAttr);
  metricNode->append_node(unitNode);

  auto* metricCategory =
      xmlDocument_->allocate_string(toString(abstractMetricDescriptor.metricCategory).c_str());
  auto* metricCategoryAttr = xmlDocument_->allocate_attribute("MetricCategory", metricCategory);
  metricNode->append_attribute(metricCategoryAttr);

  auto* metricAvailability =
      xmlDocument_->allocate_string(toString(abstractMetricDescriptor.metricAvailability).c_str());
  auto* metricAvailabilityAttr =
      xmlDocument_->allocate_attribute("MetricAvailability", metricAvailability);
  metricNode->append_attribute(metricAvailabilityAttr);

  if (const auto* const numericDescriptor =
          dyn_cast<BICEPS::PM::NumericMetricDescriptor>(&abstractMetricDescriptor);
      numericDescriptor != nullptr)
  {
    auto* typeAttr = xmlDocument_->allocate_attribute("xsi:type", "pm:NumericMetricDescriptor");
    metricNode->append_attribute(typeAttr);

    for (const auto& range : numericDescriptor->technicalRange)
    {
      auto* technicalRangeNode =
          xmlDocument_->allocate_node(rapidxml::node_element, "TechnicalRange");
      serialize(technicalRangeNode, range);
      metricNode->append_node(technicalRangeNode);
    }

    auto* resolution =
        xmlDocument_->allocate_string(std::to_string(numericDescriptor->resolution).c_str());
    auto* resolutionAttr = xmlDocument_->allocate_attribute("Resolution", resolution);
    metricNode->append_attribute(resolutionAttr);

    if (numericDescriptor->averagingPeriod.has_value())
    {
      auto* averagingPeriodAttr = xmlDocument_->allocate_attribute(
          "AveragingPeriod", numericDescriptor->averagingPeriod->c_str());
      metricNode->append_attribute(averagingPeriodAttr);
    }
  }

  parent->append_node(metricNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::Range& range)
{
  if (range.lower.has_value())
  {
    auto* lower = xmlDocument_->allocate_string(std::to_string(range.lower.value()).c_str());
    auto* lowerAttr = xmlDocument_->allocate_attribute("Lower", lower);
    parent->append_attribute(lowerAttr);
  }
  if (range.upper.has_value())
  {
    auto* upper = xmlDocument_->allocate_string(std::to_string(range.upper.value()).c_str());
    auto* upperAttr = xmlDocument_->allocate_attribute("Upper", upper);
    parent->append_attribute(upperAttr);
  }
  if (range.stepWidth.has_value())
  {
    auto* stepWidth =
        xmlDocument_->allocate_string(std::to_string(range.stepWidth.value()).c_str());
    auto* stepWidthAttr = xmlDocument_->allocate_attribute("StepWidth", stepWidth);
    parent->append_attribute(stepWidthAttr);
  }
  if (range.relativeAccuracy.has_value())
  {
    auto* relativeAccuracy =
        xmlDocument_->allocate_string(std::to_string(range.relativeAccuracy.value()).c_str());
    auto* relativeAccuracyAttr =
        xmlDocument_->allocate_attribute("RelativeAccuracy", relativeAccuracy);
    parent->append_attribute(relativeAccuracyAttr);
  }
  if (range.absoluteAccuracy.has_value())
  {
    auto* absoluteAccuracy =
        xmlDocument_->allocate_string(std::to_string(range.absoluteAccuracy.value()).c_str());
    auto* absoluteAccuracyAttr =
        xmlDocument_->allocate_attribute("AbsoluteAccuracy", absoluteAccuracy);
    parent->append_attribute(absoluteAccuracyAttr);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const BICEPS::PM::MdState& mdState)
{
  auto* mdStateNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:MdState");
  for (const auto& state : mdState.state)
  {
    serialize(mdStateNode, *state);
  }
  parent->append_node(mdStateNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractState& state)
{
  auto* stateNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:State");
  auto* descriptor = xmlDocument_->allocate_string(state.descriptorHandle.c_str());
  auto* descriptorHandleAttr = xmlDocument_->allocate_attribute("DescriptorHandle", descriptor);
  stateNode->append_attribute(descriptorHandleAttr);

  if (state.stateVersion.has_value())
  {
    auto* version =
        xmlDocument_->allocate_string(std::to_string(state.stateVersion.value()).c_str());
    auto* versionAttr = xmlDocument_->allocate_attribute("StateVersion", version);
    stateNode->append_attribute(versionAttr);
  }

  if (const auto* numericMetricState = dyn_cast<BICEPS::PM::NumericMetricState>(&state);
      numericMetricState != nullptr)
  {
    if (numericMetricState->metricValue.has_value())
    {
      serialize(stateNode, numericMetricState->metricValue.value());
    }
    auto* typeAttr = xmlDocument_->allocate_attribute("xsi:type", "pm:NumericMetricState");
    stateNode->append_attribute(typeAttr);
  }
  if (const auto* locationContextState = dyn_cast<BICEPS::PM::LocationContextState>(&state);
      locationContextState != nullptr)
  {
    for (const auto& validator : locationContextState->validator)
    {
      auto* node = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Validator");
      serialize(node, validator);
      stateNode->append_node(node);
    }
    for (const auto& identifier : locationContextState->identification)
    {
      auto* node = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Identification");
      serialize(node, identifier);
      stateNode->append_node(node);
    }
    if (locationContextState->locationDetail.has_value())
    {
      serialize(stateNode, locationContextState->locationDetail.value());
    }
    if (locationContextState->bindingMdibVersion.has_value())
    {
      auto* version = xmlDocument_->allocate_string(
          std::to_string(locationContextState->bindingMdibVersion.value()).c_str());
      auto* attr = xmlDocument_->allocate_attribute("BindingMdibVersion", version);
      stateNode->append_attribute(attr);
    }
    if (locationContextState->contextAssociation.has_value())
    {
      auto* assoc = xmlDocument_->allocate_string(
          toString(locationContextState->contextAssociation.value()).c_str());
      auto* attr = xmlDocument_->allocate_attribute("ContextAssociation", assoc);
      stateNode->append_attribute(attr);
    }
    auto* typeAttr = xmlDocument_->allocate_attribute("xsi:type", "pm:LocationContextState");
    stateNode->append_attribute(typeAttr);
    auto* handleAttr =
        xmlDocument_->allocate_attribute("Handle", locationContextState->handle.c_str());
    stateNode->append_attribute(handleAttr);
  }
  parent->append_node(stateNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::InstanceIdentifier& identifier)
{
  if (identifier.root.has_value())
  {
    auto* rootAttr = xmlDocument_->allocate_attribute("Root", identifier.root.value().c_str());
    parent->append_attribute(rootAttr);
  }
  if (identifier.extension.has_value())
  {
    auto* extensionAttr =
        xmlDocument_->allocate_attribute("Extension", identifier.extension.value().c_str());
    parent->append_attribute(extensionAttr);
  }
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::LocationDetail& locationDetail)
{
  auto* locationDetailNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "pm:LocationDetail");
  if (locationDetail.poC.has_value())
  {
    auto* pocAttr = xmlDocument_->allocate_attribute("PoC", locationDetail.poC.value().c_str());
    locationDetailNode->append_attribute(pocAttr);
  }
  if (locationDetail.room.has_value())
  {
    auto* pocAttr = xmlDocument_->allocate_attribute("Room", locationDetail.room.value().c_str());
    locationDetailNode->append_attribute(pocAttr);
  }
  if (locationDetail.bed.has_value())
  {
    auto* pocAttr = xmlDocument_->allocate_attribute("Bed", locationDetail.bed.value().c_str());
    locationDetailNode->append_attribute(pocAttr);
  }
  if (locationDetail.facility.has_value())
  {
    auto* pocAttr =
        xmlDocument_->allocate_attribute("Facility", locationDetail.facility.value().c_str());
    locationDetailNode->append_attribute(pocAttr);
  }
  if (locationDetail.building.has_value())
  {
    auto* pocAttr =
        xmlDocument_->allocate_attribute("Building", locationDetail.building.value().c_str());
    locationDetailNode->append_attribute(pocAttr);
  }
  if (locationDetail.floor.has_value())
  {
    auto* pocAttr = xmlDocument_->allocate_attribute("Floor", locationDetail.floor.value().c_str());
    locationDetailNode->append_attribute(pocAttr);
  }
  parent->append_node(locationDetailNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractMetricValue& value)
{
  auto* valueNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:MetricValue");
  serialize(valueNode, value.metricQuality);

  if (const auto* numericValue = dyn_cast<BICEPS::PM::NumericMetricValue>(&value);
      numericValue != nullptr)
  {
    if (numericValue->value.has_value())
    {
      auto* num =
          xmlDocument_->allocate_string(std::to_string(numericValue->value.value()).c_str());
      auto* valueAttr = xmlDocument_->allocate_attribute("Value", num);
      valueNode->append_attribute(valueAttr);
    }
  }

  parent->append_node(valueNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::MetricQuality& quality)
{
  auto* metricQualityNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:MetricQuality");
  auto* validity = xmlDocument_->allocate_string(toString(quality.validity).c_str());
  auto* validityAttr = xmlDocument_->allocate_attribute("Validity", validity);
  metricQualityNode->append_attribute(validityAttr);
  parent->append_node(metricQualityNode);
}


void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::SubscribeResponse& subscribeResponse)
{
  auto* subscribeResponseNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wse:SubscribeResponse");
  auto* subscriptionManagerNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wse:SubscriptionManager");

  auto* addressNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:Address");
  addressNode->value(subscribeResponse.subscriptionManager.address.c_str());
  subscriptionManagerNode->append_node(addressNode);

  serialize(subscriptionManagerNode,
            subscribeResponse.subscriptionManager.referenceParameters.value());

  subscribeResponseNode->append_node(subscriptionManagerNode);

  serialize(subscribeResponseNode, subscribeResponse.expires);

  parent->append_node(subscribeResponseNode);
}

void MessageSerializer::serialize(
    rapidxml::xml_node<>* parent,
    const WS::ADDRESSING::ReferenceParametersType& referenceParameters)
{
  auto* referenceParametersNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsa:ReferenceParameters");
  if (referenceParameters.identifier.has_value())
  {
    auto* identifierNode = xmlDocument_->allocate_node(rapidxml::node_element, "wse:Identifier");
    identifierNode->value(referenceParameters.identifier.value().c_str());
    referenceParametersNode->append_node(identifierNode);
  }
  parent->append_node(referenceParametersNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::RenewResponse& renewResponse)
{
  auto* renewResponseNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wse:RenewResponse");
  if (renewResponse.expires.has_value())
  {
    serialize(renewResponseNode, renewResponse.expires.value());
  }
  parent->append_node(renewResponseNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::SetValueResponse& setValueResponse)
{
  auto* setValueResponseNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "msg:SetValueResponse");
  auto* xmlnsBicepsMessage =
      xmlDocument_->allocate_attribute("xmlns:msg", SDC::NS_BICEPS_MESSAGE_MODEL);
  setValueResponseNode->append_attribute(xmlnsBicepsMessage);
  if (setValueResponse.mdibVersion.has_value())
  {
    auto* mdibVersion =
        xmlDocument_->allocate_string(std::to_string(setValueResponse.mdibVersion.value()).c_str());
    auto* mdibVersionAttr = xmlDocument_->allocate_attribute("MdibVersion", mdibVersion);
    setValueResponseNode->append_attribute(mdibVersionAttr);
  }
  auto* sequenceId = xmlDocument_->allocate_string(setValueResponse.sequenceId.c_str());
  auto* SequenceIdAttr = xmlDocument_->allocate_attribute("SequenceId", sequenceId);
  setValueResponseNode->append_attribute(SequenceIdAttr);
  if (setValueResponse.instanceId.has_value())
  {
    auto* instanceId =
        xmlDocument_->allocate_string(std::to_string(setValueResponse.instanceId.value()).c_str());
    auto* instanceIdAttr = xmlDocument_->allocate_attribute("SequenceId", instanceId);
    setValueResponseNode->append_attribute(instanceIdAttr);
  }
  serialize(setValueResponseNode, setValueResponse.invocationInfo);
  parent->append_node(setValueResponseNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::InvocationInfo& invocationInfo)
{
  auto* invocationInfoNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "msg:InvocationInfo");

  auto* transactionId =
      xmlDocument_->allocate_string(std::to_string(invocationInfo.transactionId).c_str());
  auto* transactionIdNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "msg:TransactionId", transactionId);
  invocationInfoNode->append_node(transactionIdNode);

  auto* invocationState =
      xmlDocument_->allocate_string(toString(invocationInfo.invocationState).c_str());
  auto* invocationStateNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "msg:InvocationState", invocationState);
  invocationInfoNode->append_node(invocationStateNode);

  if (invocationInfo.invocationError.has_value())
  {
    auto* invocationError =
        xmlDocument_->allocate_string(toString(invocationInfo.invocationError.value()).c_str());
    auto* invocationErrorNode =
        xmlDocument_->allocate_node(rapidxml::node_element, "msg:InvocationError", invocationError);
    invocationInfoNode->append_node(invocationErrorNode);
  }

  if (invocationInfo.invocationErrorMessage.has_value())
  {
    auto* invocationErrorMessage =
        xmlDocument_->allocate_string(invocationInfo.invocationErrorMessage.value().c_str());
    auto* invocationErrorMessageNode = xmlDocument_->allocate_node(
        rapidxml::node_element, "msg:InvocationErrorMessage", invocationErrorMessage);
    invocationInfoNode->append_node(invocationErrorMessageNode);
  }
  parent->append_node(invocationInfoNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::EpisodicMetricReport& report)
{
  auto* reportNode = xmlDocument_->allocate_node(rapidxml::node_element, "mm:EpisodicMetricReport");
  if (report.mdibVersion.has_value())
  {
    auto* version =
        xmlDocument_->allocate_string(std::to_string(report.mdibVersion.value()).c_str());
    auto* versionAttr = xmlDocument_->allocate_attribute("MdibVersion", version);
    reportNode->append_attribute(versionAttr);
  }
  for (const auto& part : report.reportPart)
  {
    serialize(reportNode, part);
  }
  parent->append_node(reportNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::MetricReportPart& part)
{
  auto* reportPartNode = xmlDocument_->allocate_node(rapidxml::node_element, "mm:ReportPart");
  for (const auto& state : part.metricState)
  {
    serialize(reportPartNode, *state);
  }
  parent->append_node(reportPartNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::ScoDescriptor& sco)
{
  auto* scoNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Sco");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", sco.handle.c_str());
  scoNode->append_attribute(handleAttr);
  auto* typeAttr = xmlDocument_->allocate_attribute("xsi:type", "pm:ScoDescriptor");
  scoNode->append_attribute(typeAttr);
  for (const auto& operation : sco.operation)
  {
    serialize(scoNode, *operation);
  }
  parent->append_node(scoNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::PM::AbstractOperationDescriptor& operation)
{
  auto* operationNode = xmlDocument_->allocate_node(rapidxml::node_element, "pm:Operation");
  auto* handleAttr = xmlDocument_->allocate_attribute("Handle", operation.handle.c_str());
  operationNode->append_attribute(handleAttr);
  auto* operationTargetAttr =
      xmlDocument_->allocate_attribute("OperationTarget", operation.operationTarget.c_str());
  operationNode->append_attribute(operationTargetAttr);
  if (isa<BICEPS::PM::SetValueOperationDescriptor>(&operation))
  {
    auto* typeAttr = xmlDocument_->allocate_attribute("xsi:type", "pm:SetValueOperationDescriptor");
    operationNode->append_attribute(typeAttr);
  }
  parent->append_node(operationNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::EVENTING::ExpirationType& expiration)
{
  const auto* expiresDuration = xmlDocument_->allocate_string(toString(expiration).c_str());
  auto* expiresNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wse:Expires", expiresDuration);
  parent->append_node(expiresNode);
}

/*static*/ std::string
MessageSerializer::toString(BICEPS::PM::SafetyClassification safetyClassification)
{
  switch (safetyClassification)
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

std::string MessageSerializer::toString(const WS::DISCOVERY::UriListType& uriList)
{
  std::string out;
  for (auto uri = uriList.begin(); uri != uriList.end(); uri++)
  {
    if (uri != uriList.begin())
    {
      out += " ";
    }
    out += *uri;
  }
  return out;
}

std::string MessageSerializer::toString(const WS::DISCOVERY::QNameListType& qNameList)
{
  std::string out;
  for (auto qname = qNameList.begin(); qname != qNameList.end(); qname++)
  {
    if (qname != qNameList.begin())
    {
      out += " ";
    }
    out += std::string(std::string(qname->ns) + ":" + qname->name);
  }
  return out;
}

std::string MessageSerializer::toString(BICEPS::PM::MetricCategory category)
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

std::string MessageSerializer::toString(BICEPS::PM::MetricAvailability availability)
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

std::string MessageSerializer::toString(BICEPS::PM::MeasurementValidity validity)
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

std::string MessageSerializer::toString(BICEPS::MM::InvocationState invocationState)
{
  switch (invocationState)
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

std::string MessageSerializer::toString(BICEPS::MM::InvocationError invocationError)
{
  switch (invocationError)
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

std::string MessageSerializer::toString(BICEPS::PM::ContextAssociation contextAssociation)
{
  switch (contextAssociation)
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

std::string MessageSerializer::toString(Duration duration)
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
