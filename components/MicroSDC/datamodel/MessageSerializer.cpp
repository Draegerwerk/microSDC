#include "MessageSerializer.hpp"
#include "NamespaceMap.hpp"
#include "rapidxml_print.hpp"

MessageSerializer::MessageSerializer()
  : xmlDocument_(std::make_unique<rapidxml::xml_document<>>())
{
  auto declaration = xmlDocument_->allocate_node(rapidxml::node_declaration);
  auto version = xmlDocument_->allocate_attribute("version", "1.0");
  auto encoding = xmlDocument_->allocate_attribute("encoding", "utf-8");
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
  auto envelope = xmlDocument_->allocate_node(rapidxml::node_element, "soap:Envelope");

  auto xmlns_soap = xmlDocument_->allocate_attribute("xmlns:soap", MDPWS::WS_NS_SOAP_ENVELOPE);
  auto xmlns_wsd = xmlDocument_->allocate_attribute("xmlns:wsd", MDPWS::WS_NS_DISCOVERY);
  auto xmlns_wsa = xmlDocument_->allocate_attribute("xmlns:wsa", MDPWS::WS_NS_ADDRESSING);
  auto xmlns_dpws = xmlDocument_->allocate_attribute("xmlns:dpws", MDPWS::WS_NS_DPWS);
  auto xmlns_mdpws = xmlDocument_->allocate_attribute("xmlns:mdpws", MDPWS::NS_MDPWS);
  auto xmlns_mex = xmlDocument_->allocate_attribute("xmlns:mex", MDPWS::WS_NS_METADATA_EXCHANGE);
  auto xmlns_glue = xmlDocument_->allocate_attribute("xmlns:glue", SDC::NS_GLUE);
  auto xmlns_mm = xmlDocument_->allocate_attribute("xmlns:mm", SDC::NS_BICEPS_MESSAGE_MODEL);
  auto xmlns_pm = xmlDocument_->allocate_attribute("xmlns:pm", SDC::NS_BICEPS_PARTICIPANT_MODEL);
  auto xmlns_ext = xmlDocument_->allocate_attribute("xmlns:ext", SDC::NS_BICEPS_EXTENSION);

  envelope->append_attribute(xmlns_soap);
  envelope->append_attribute(xmlns_wsd);
  envelope->append_attribute(xmlns_wsa);
  envelope->append_attribute(xmlns_dpws);
  envelope->append_attribute(xmlns_mdpws);
  envelope->append_attribute(xmlns_mex);
  envelope->append_attribute(xmlns_glue);
  envelope->append_attribute(xmlns_mm);
  envelope->append_attribute(xmlns_pm);
  envelope->append_attribute(xmlns_ext);

  serialize(envelope, message.Header());
  serialize(envelope, message.Body());

  parent->append_node(envelope);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Header& header)
{
  auto headerNode = xmlDocument_->allocate_node(rapidxml::node_element, "soap:Header");
  // Mandatory action element
  auto actionNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:Action");
  actionNode->value(header.Action().uri().c_str());
  headerNode->append_node(actionNode);
  // optionals
  if (header.MessageID().has_value())
  {
    auto messageIdNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:MessageID");
    messageIdNode->value(header.MessageID()->uri().c_str());
    headerNode->append_node(messageIdNode);
  }
  if (header.To().has_value())
  {
    auto toNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:To");
    toNode->value(header.To()->uri().c_str());
    headerNode->append_node(toNode);
  }
  if (header.AppSequence().has_value())
  {
    serialize(headerNode, header.AppSequence().value());
  }
  if (header.RelatesTo().has_value())
  {
    serialize(headerNode, header.RelatesTo().value());
  }
  parent->append_node(headerNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Body& body)
{
  auto bodyNode = xmlDocument_->allocate_node(rapidxml::node_element, "soap:Body");
  if (body.Hello().has_value())
  {
    serialize(bodyNode, body.Hello().value());
  }
  else if (body.ProbeMatches().has_value())
  {
    serialize(bodyNode, body.ProbeMatches().value());
  }
  else if (body.ResolveMatches().has_value())
  {
    serialize(bodyNode, body.ResolveMatches().value());
  }
  else if (body.Metadata().has_value())
  {
    serialize(bodyNode, body.Metadata().value());
  }
  else if (body.GetMdibResponse().has_value())
  {
    bodyNode->value(body.GetMdibResponse()->c_str());
  }
  parent->append_node(bodyNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::RelatesToType& relatesTo)
{
  auto relatesToNode = xmlDocument_->allocate_node(rapidxml::node_element, "mdpws:RelatesTo");
  relatesToNode->value(relatesTo.uri().c_str(), relatesTo.uri().size());
  parent->append_node(relatesToNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::ADDRESSING::EndpointReferenceType& endpointReference)
{
  auto eprNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:EndpointReference");
  auto addressNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsa:Address");
  addressNode->value(endpointReference.Address().uri().c_str());
  eprNode->append_node(addressNode);
  parent->append_node(eprNode);
}

/*
void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::QNameListType& qNameList)
{
  for (auto qname = qNameList.begin(); qname != qNameList.end(); qname++)
  {
    if (qname != qNameList.begin())
    {
      sstream_ += " ";
    }
    sstream_ += std::string(nsMap__.at(qname->ns()).data()) + ":" + qname->name();
  }
}
*/

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::AppSequenceType& appSequence)
{
  auto appSequenceNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:AppSequence");
  const auto instanceIdStr = std::to_string(appSequence.InstanceId());
  auto instanceId = xmlDocument_->allocate_string(instanceIdStr.c_str());
  auto instanceIdAttr = xmlDocument_->allocate_attribute("InstanceId", instanceId);
  appSequenceNode->append_attribute(instanceIdAttr);
  if (appSequence.SequenceId().has_value())
  {
    auto sequenceId = xmlDocument_->allocate_string(appSequence.SequenceId()->uri().c_str(),
                                                    appSequence.SequenceId()->uri().size());
    auto sequenceIdAttr = xmlDocument_->allocate_attribute("SequenceId", sequenceId);
    appSequenceNode->append_attribute(sequenceIdAttr);
  }
  auto messageNumberStr = std::to_string(appSequence.MessageNumber());
  auto messageNumber = xmlDocument_->allocate_string(messageNumberStr.c_str());
  auto messageNumberAttr = xmlDocument_->allocate_attribute("MessageNumber", messageNumber);
  appSequenceNode->append_attribute(messageNumberAttr);
  parent->append_node(appSequenceNode);
}
void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ScopesType& scopes)
{
  auto scopesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Scopes");
  auto matchByAttr = xmlDocument_->allocate_attribute("MatchBy", scopes.MatchBy()->uri().c_str());
  scopesNode->append_attribute(matchByAttr);
  auto uriList = xmlDocument_->allocate_string(toString(scopes).c_str());
  scopesNode->value(uriList);
  parent->append_node(scopesNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::HelloType& hello)
{
  auto helloNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Hello");
  serialize(helloNode, hello.EndpointReference());
  if (hello.Types().has_value())
  {
    auto typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto typesStr = xmlDocument_->allocate_string(toString(hello.Types().value()).c_str());
    typesNode->value(typesStr);
    helloNode->append_node(typesNode);
  }
  if (hello.Scopes().has_value())
  {
    serialize(helloNode, hello.Scopes().value());
  }
  if (hello.XAddrs().has_value())
  {
    auto xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto xAddrsStr = xmlDocument_->allocate_string(toString(hello.XAddrs().value()).c_str());
    xAddrsNode->value(xAddrsStr);
    helloNode->append_node(xAddrsNode);
  }
  auto metadataVersionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto metadataVersion =
      xmlDocument_->allocate_string(std::to_string(hello.MetadataVersion()).c_str());
  metadataVersionNode->value(metadataVersion);
  parent->append_node(helloNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchType& probeMatch)
{
  auto probeMatchNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ProbeMatch");
  serialize(probeMatchNode, probeMatch.EndpointReference());
  if (probeMatch.Types().has_value())
  {
    auto typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto typesStr = xmlDocument_->allocate_string(toString(probeMatch.Types().value()).c_str());
    typesNode->value(typesStr);
    probeMatchNode->append_node(typesNode);
  }
  if (probeMatch.Scopes().has_value())
  {
    auto scopesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Scopes");
    serialize(scopesNode, probeMatch.Scopes().value());
    probeMatchNode->append_node(scopesNode);
  }
  if (probeMatch.XAddrs().has_value())
  {
    auto xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto xAddrsStr = xmlDocument_->allocate_string(toString(probeMatch.XAddrs().value()).c_str());
    xAddrsNode->value(xAddrsStr);
    probeMatchNode->append_node(xAddrsNode);
  }
  auto metadataVersionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto metadataVersion =
      xmlDocument_->allocate_string(std::to_string(probeMatch.MetadataVersion()).c_str());
  metadataVersionNode->value(metadataVersion);
  probeMatchNode->append_node(metadataVersionNode);
  parent->append_node(probeMatchNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ProbeMatchesType& probeMatches)
{
  auto probeMatchesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ProbeMatches");
  for (const auto& probeMatch : probeMatches.ProbeMatch())
  {
    serialize(probeMatchesNode, probeMatch);
  }
  parent->append_node(probeMatchesNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchType& resolveMatch)
{
  auto resolveMatchNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  serialize(resolveMatchNode, resolveMatch.EndpointReference());
  if (resolveMatch.Types().has_value())
  {
    auto typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:Types");
    auto typesStr = xmlDocument_->allocate_string(toString(resolveMatch.Types().value()).c_str());
    typesNode->value(typesStr);
    resolveMatchNode->append_node(typesNode);
  }
  if (resolveMatch.Scopes().has_value())
  {
    serialize(resolveMatchNode, resolveMatch.Scopes().value());
  }
  if (resolveMatch.XAddrs().has_value())
  {
    auto xAddrsNode = xmlDocument_->allocate_node(rapidxml::node_element, "wsd:XAddrs");
    auto xAddrsStr = xmlDocument_->allocate_string(toString(resolveMatch.XAddrs().value()).c_str());
    xAddrsNode->value(xAddrsStr);
    resolveMatchNode->append_node(xAddrsNode);
  }
  auto metadataVersionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:MetadataVersion");
  auto metadataVersion =
      xmlDocument_->allocate_string(std::to_string(resolveMatch.MetadataVersion()).c_str());
  metadataVersionNode->value(metadataVersion);
  resolveMatchNode->append_node(metadataVersionNode);
  parent->append_node(resolveMatchNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DISCOVERY::ResolveMatchesType& resolveMatches)
{
  auto resolveMatchesNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "wsd:ResolveMatches");
  for (const auto& resolveMatch : resolveMatches.ResolveMatch())
  {
    serialize(resolveMatchesNode, resolveMatch);
  }
  parent->append_node(resolveMatchesNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent, const WS::MEX::Metadata& metadata)
{
  auto metadataNode = xmlDocument_->allocate_node(rapidxml::node_element, "mex:Metadata");
  for (const auto& metadataSection : metadata.MetadataSection())
  {
    serialize(metadataNode, metadataSection);
  }
  parent->append_node(metadataNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::MEX::MetadataSection& metadataSection)
{
  auto metadataSectionNode =
      xmlDocument_->allocate_node(rapidxml::node_element, "mex:MetadataSection");
  auto dialectAttr =
      xmlDocument_->allocate_attribute("Dialect", metadataSection.Dialect().uri().c_str());
  metadataSectionNode->append_attribute(dialectAttr);
  if (metadataSection.ThisModel().has_value())
  {
    serialize(metadataSectionNode, metadataSection.ThisModel().value());
  }
  else if (metadataSection.ThisDevice().has_value())
  {
    serialize(metadataSectionNode, metadataSection.ThisDevice().value());
  }
  else if (metadataSection.Relationship().has_value())
  {
    serialize(metadataSectionNode, metadataSection.Relationship().value());
  }
  else if (metadataSection.Location().has_value())
  {
    auto locationNode = xmlDocument_->allocate_node(rapidxml::node_element, "mex:Location");
    locationNode->value(metadataSection.Location()->uri().c_str());
    metadataSectionNode->append_node(locationNode);
  }
  parent->append_node(metadataSectionNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::ThisModelType& thisModel)
{
  auto thisModelNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ThisModel");
  // Manufacturer
  for (const auto& manufacturer : thisModel.Manufacturer())
  {
    auto manufacturerNode =
        xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Manufacturer");
    manufacturerNode->value(manufacturer.c_str());
    thisModelNode->append_node(manufacturerNode);
  }
  // ModelName
  for (const auto& modelName : thisModel.ModelName())
  {
    auto modelNameNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ModelName");
    modelNameNode->value(modelName.c_str());
    thisModelNode->append_node(modelNameNode);
  }
  parent->append_node(thisModelNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::ThisDeviceType& thisDevice)
{
  auto thisDeviceNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ThisDevice");
  // FriendlyName
  for (const auto& friendlyName : thisDevice.FriendlyName())
  {
    auto friendlyNameNode =
        xmlDocument_->allocate_node(rapidxml::node_element, "dpws:FriendlyName");
    friendlyNameNode->value(friendlyName.c_str());
  }
  parent->append_node(thisDeviceNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::Relationship& relationship)
{
  auto relationshipNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Relationship");
  auto typeAttr = xmlDocument_->allocate_attribute("Type", relationship.Type().c_str());
  relationshipNode->append_attribute(typeAttr);

  serialize(relationshipNode, relationship.Host());
  for (const auto& hosted : relationship.Hosted())
  {
    serialize(relationshipNode, hosted);
  }
  parent->append_node(relationshipNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::HostServiceType& host)
{
  auto hostNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Host");
  serialize(hostNode, host.EndpointReference());
  if (host.Types().has_value())
  {
    auto typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Types");
    auto typesStr = xmlDocument_->allocate_string(toString(host.Types().value()).c_str());
    typesNode->value(typesStr);
    hostNode->append_node(typesNode);
  }
  parent->append_node(hostNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const WS::DPWS::HostedServiceType& hosted)
{
  auto hostedNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Hosted");
  for (const auto& epr : hosted.EndpointReference())
  {
    serialize(hostedNode, epr);
  }
  // Types
  auto typesNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:Types");
  auto typesStr = xmlDocument_->allocate_string(toString(hosted.Types()).c_str());
  typesNode->value(typesStr);
  hostedNode->append_node(typesNode);
  // ServiceId
  auto serviceIdNode = xmlDocument_->allocate_node(rapidxml::node_element, "dpws:ServiceId");
  serviceIdNode->value(hosted.ServiceId().uri().c_str());
  hostedNode->append_node(serviceIdNode);
  parent->append_node(hostedNode);
}

void MessageSerializer::serialize(rapidxml::xml_node<>* parent,
                                  const BICEPS::MM::GetMdibResponse& getMdibResponse)
{
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
    out += uri->uri();
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
    out += std::string(std::string(qname->ns()) + ":" + qname->name());
  }
  return out;
}
