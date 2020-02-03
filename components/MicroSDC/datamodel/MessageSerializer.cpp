#include "MessageSerializer.hpp"
#include "NamespaceMap.hpp"

MessageSerializer::MessageSerializer()
{
  sstream_ += "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
}

const std::string& MessageSerializer::str() const
{
  return sstream_;
}

void MessageSerializer::serialize(const std::string& string)
{
  sstream_ += string;
}

void MessageSerializer::serialize(const MESSAGEMODEL::Envelope& message)
{
  sstream_ += std::string("<") + nsMap__.at(MDPWS::WS_NS_SOAP_ENVELOPE).data() + ":Envelope ";
  for (const auto& [ns, prefix] : nsMap__)
  {
    sstream_ += std::string("xmlns:") + prefix.data() + "=\"" + ns.data() + "\" ";
  }
  sstream_ += ">";
  serialize(message.Header());
  serialize(message.Body());
  sstream_ += std::string("</") + nsMap__.at(MDPWS::WS_NS_SOAP_ENVELOPE).data() + ":Envelope>";
}

void MessageSerializer::serialize(const MESSAGEMODEL::Header& header)
{
  openTag("Header", MDPWS::WS_NS_SOAP_ENVELOPE, {});

  // Mandatory action element
  openTag("Action", MDPWS::WS_NS_ADDRESSING, {});
  serialize(header.Action());
  closeTag("Action", MDPWS::WS_NS_ADDRESSING);

  if (header.MessageID().has_value())
  {
    openTag("MessageID", MDPWS::WS_NS_ADDRESSING, {});
    serialize(header.MessageID().value());
    closeTag("MessageID", MDPWS::WS_NS_ADDRESSING);
  }
  if (header.To().has_value())
  {
    openTag("To", MDPWS::WS_NS_ADDRESSING, {});
    serialize(header.To().value());
    closeTag("To", MDPWS::WS_NS_ADDRESSING);
  }
  if (header.AppSequence().has_value())
  {
    serialize(header.AppSequence().value());
  }
  if (header.RelatesTo().has_value())
  {
    serialize(header.RelatesTo().value());
  }
  closeTag("Header", MDPWS::WS_NS_SOAP_ENVELOPE);
}

void MessageSerializer::serialize(const MESSAGEMODEL::Body& body)
{
  openTag("Body", MDPWS::WS_NS_SOAP_ENVELOPE, {});
  if (body.Hello().has_value())
  {
    serialize(body.Hello().value());
  }
  else if (body.ProbeMatches().has_value())
  {
    serialize(body.ProbeMatches().value());
  }
  else if (body.ResolveMatches().has_value())
  {
    serialize(body.ResolveMatches().value());
  }
  else if (body.Metadata().has_value())
  {
    serialize(body.Metadata().value());
  }
  else if (body.GetMdibResponse().has_value())
  {
    serialize(body.GetMdibResponse().value());
  }
  closeTag("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
}

void MessageSerializer::serialize(const WS::ADDRESSING::URIType& uri)
{
  sstream_ += uri.uri();
}

void MessageSerializer::serialize(const WS::ADDRESSING::RelatesToType& relatesTo)
{
  openTag("RelatesTo", MDPWS::WS_NS_ADDRESSING, {});
  sstream_ += relatesTo.uri();
  closeTag("RelatesTo", MDPWS::WS_NS_ADDRESSING);
}

void MessageSerializer::serialize(const WS::ADDRESSING::EndpointReferenceType& endpointReference)
{
  openTag("EndpointReference", MDPWS::WS_NS_ADDRESSING, {});
  openTag("Address", MDPWS::WS_NS_ADDRESSING, {});
  serialize(endpointReference.Address());
  closeTag("Address", MDPWS::WS_NS_ADDRESSING);
  closeTag("EndpointReference", MDPWS::WS_NS_ADDRESSING);
}

void MessageSerializer::serialize(const WS::DISCOVERY::QNameListType& qNameList)
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

void MessageSerializer::serialize(const WS::DISCOVERY::UriListType& uriList)
{
  for (const auto uri : uriList)
  {
    serialize(uri);
    sstream_ += " ";
  }
}

void MessageSerializer::serialize(const WS::DISCOVERY::AppSequenceType& appSequence)
{
  std::vector<Attribute> attributes;
  attributes.reserve(3);
  attributes.emplace_back("InstanceId", std::to_string(appSequence.InstanceId()));
  if (appSequence.SequenceId().has_value())
  {
    attributes.emplace_back("SequenceId", appSequence.SequenceId()->uri());
  }
  attributes.emplace_back("MessageNumber", std::to_string(appSequence.MessageNumber()));
  openTag("AppSequence", MDPWS::WS_NS_DISCOVERY, attributes);
  closeTag("AppSequence", MDPWS::WS_NS_DISCOVERY);
}

void MessageSerializer::serialize(const WS::DISCOVERY::HelloType& hello)
{
  openTag("Hello", MDPWS::WS_NS_DISCOVERY, {});
  serialize(hello.EndpointReference());
  if (hello.Types().has_value())
  {
    openTag("Types", MDPWS::WS_NS_DISCOVERY, {});
    serialize(hello.Types().value());
    closeTag("Types", MDPWS::WS_NS_DISCOVERY);
  }
  if (hello.Scopes().has_value())
  {
    openTag("Scopes", MDPWS::WS_NS_DISCOVERY, {});
    serialize(hello.Scopes().value());
    closeTag("Scopes", MDPWS::WS_NS_DISCOVERY);
  }
  if (hello.XAddrs().has_value())
  {
    openTag("XAddrs", MDPWS::WS_NS_DISCOVERY, {});
    serialize(hello.XAddrs().value());
    closeTag("XAddrs", MDPWS::WS_NS_DISCOVERY);
  }
  openTag("MetadataVersion", MDPWS::WS_NS_DISCOVERY, {});
  sstream_ += hello.MetadataVersion();
  closeTag("MetadataVersion", MDPWS::WS_NS_DISCOVERY);
  closeTag("Hello", MDPWS::WS_NS_DISCOVERY);
}

void MessageSerializer::serialize(const WS::DISCOVERY::ProbeMatchType& probeMatch)
{
  openTag("ProbeMatch", MDPWS::WS_NS_DISCOVERY, {});
  serialize(probeMatch.EndpointReference());
  if (probeMatch.Types().has_value())
  {
    openTag("Types", MDPWS::WS_NS_DISCOVERY, {});
    serialize(probeMatch.Types().value());
    closeTag("Types", MDPWS::WS_NS_DISCOVERY);
  }
  if (probeMatch.Scopes().has_value())
  {
    openTag("Scopes", MDPWS::WS_NS_DISCOVERY, {});
    serialize(probeMatch.Scopes().value());
    closeTag("Scopes", MDPWS::WS_NS_DISCOVERY);
  }
  if (probeMatch.XAddrs().has_value())
  {
    openTag("XAddrs", MDPWS::WS_NS_DISCOVERY, {});
    serialize(probeMatch.XAddrs().value());
    closeTag("XAddrs", MDPWS::WS_NS_DISCOVERY);
  }
  openTag("MetadataVersion", MDPWS::WS_NS_DISCOVERY, {});
  sstream_ += std::to_string(probeMatch.MetadataVersion());
  closeTag("MetadataVersion", MDPWS::WS_NS_DISCOVERY);
  closeTag("ProbeMatch", MDPWS::WS_NS_DISCOVERY);
}

void MessageSerializer::serialize(const WS::DISCOVERY::ProbeMatchesType& probeMatches)
{
  openTag("ProbeMatches", MDPWS::WS_NS_DISCOVERY, {});
  for (const auto& probeMatch : probeMatches.ProbeMatch())
  {
    serialize(probeMatch);
  }
  closeTag("ProbeMatches", MDPWS::WS_NS_DISCOVERY);
}

void MessageSerializer::serialize(const WS::DISCOVERY::ResolveMatchType& probeMatch)
{
  openTag("ResolveMatch", MDPWS::WS_NS_DISCOVERY, {});
  serialize(probeMatch.EndpointReference());
  if (probeMatch.Types().has_value())
  {
    openTag("Types", MDPWS::WS_NS_DISCOVERY, {});
    serialize(probeMatch.Types().value());
    closeTag("Types", MDPWS::WS_NS_DISCOVERY);
  }
  if (probeMatch.Scopes().has_value())
  {
    openTag("Scopes", MDPWS::WS_NS_DISCOVERY, {});
    serialize(probeMatch.Scopes().value());
    closeTag("Scopes", MDPWS::WS_NS_DISCOVERY);
  }
  if (probeMatch.XAddrs().has_value())
  {
    openTag("XAddrs", MDPWS::WS_NS_DISCOVERY, {});
    serialize(probeMatch.XAddrs().value());
    closeTag("XAddrs", MDPWS::WS_NS_DISCOVERY);
  }
  openTag("MetadataVersion", MDPWS::WS_NS_DISCOVERY, {});
  sstream_ += std::to_string(probeMatch.MetadataVersion());
  closeTag("MetadataVersion", MDPWS::WS_NS_DISCOVERY);
  closeTag("ResolveMatch", MDPWS::WS_NS_DISCOVERY);
}

void MessageSerializer::serialize(const WS::DISCOVERY::ResolveMatchesType& resolveMatches)
{
  openTag("ResolveMatches", MDPWS::WS_NS_DISCOVERY, {});
  for (const auto& resolveMatch : resolveMatches.ResolveMatch())
  {
    serialize(resolveMatch);
  }
  closeTag("ResolveMatches", MDPWS::WS_NS_DISCOVERY);
}

void MessageSerializer::serialize(const WS::MEX::Metadata& metadata)
{
  openTag("Metadata", MDPWS::WS_NS_METADATA_EXCHANGE, {});
  for (const auto& metadataSection : metadata.MetadataSection())
  {
    serialize(metadataSection);
  }
  closeTag("Metadata", MDPWS::WS_NS_METADATA_EXCHANGE);
}

void MessageSerializer::serialize(const WS::MEX::MetadataSection& metadataSection)
{
  openTag("MetadataSection", MDPWS::WS_NS_METADATA_EXCHANGE,
          {Attribute("Dialect", metadataSection.Dialect().uri())});
  if (metadataSection.ThisModel().has_value())
  {
    serialize(metadataSection.ThisModel().value());
  }
  else if (metadataSection.ThisDevice().has_value())
  {
    serialize(metadataSection.ThisDevice().value());
  }
  else if (metadataSection.Relationship().has_value())
  {
    serialize(metadataSection.Relationship().value());
  }
  else if (metadataSection.Location().has_value())
  {
    openTag("Location", MDPWS::WS_NS_METADATA_EXCHANGE, {});
    serialize(metadataSection.Location().value());
    closeTag("Location", MDPWS::WS_NS_METADATA_EXCHANGE);
  }
  closeTag("MetadataSection", MDPWS::WS_NS_METADATA_EXCHANGE);
}

void MessageSerializer::serialize(const WS::DPWS::ThisModelType& thisModel)
{
  openTag("ThisModel", MDPWS::WS_NS_DPWS, {});
  // Manufacturer
  for (const auto& manufacturer : thisModel.Manufacturer())
  {
    openTag("Manufacturer", MDPWS::WS_NS_DPWS, {});
    serialize(manufacturer);
    closeTag("Manufacturer", MDPWS::WS_NS_DPWS);
  }
  // ModelName
  for (const auto& modelName : thisModel.ModelName())
  {
    openTag("ModelName", MDPWS::WS_NS_DPWS, {});
    serialize(modelName);
    closeTag("ModelName", MDPWS::WS_NS_DPWS);
  }
  closeTag("ThisModel", MDPWS::WS_NS_DPWS);
}

void MessageSerializer::serialize(const WS::DPWS::ThisDeviceType& thisDevice)
{
  openTag("ThisDevice", MDPWS::WS_NS_DPWS, {});
  // FriendlyName
  for (const auto& friendlyName : thisDevice.FriendlyName())
  {
    openTag("FriendlyName", MDPWS::WS_NS_DPWS, {});
    serialize(friendlyName);
    closeTag("FriendlyName", MDPWS::WS_NS_DPWS);
  }
  closeTag("ThisDevice", MDPWS::WS_NS_DPWS);
}

void MessageSerializer::serialize(const WS::DPWS::Relationship& relationship)
{
  openTag("Relationship", MDPWS::WS_NS_DPWS, {Attribute("Type", relationship.Type())});
  serialize(relationship.Host());
  for (const auto& hosted : relationship.Hosted())
  {
    serialize(hosted);
  }
  closeTag("Relationship", MDPWS::WS_NS_DPWS);
}

void MessageSerializer::serialize(const WS::DPWS::HostServiceType& host)
{
  openTag("Host", MDPWS::WS_NS_DPWS, {});
  serialize(host.EndpointReference());
  if (host.Types().has_value())
  {
    openTag("Types", MDPWS::WS_NS_DPWS, {});
    serialize(host.Types().value());
    closeTag("Types", MDPWS::WS_NS_DPWS);
  }
  closeTag("Host", MDPWS::WS_NS_DPWS);
}

void MessageSerializer::serialize(const WS::DPWS::HostedServiceType& hosted)
{
  openTag("Hosted", MDPWS::WS_NS_DPWS, {});
  for (const auto& epr : hosted.EndpointReference())
  {
    serialize(epr);
  }
  // Types
  openTag("Types", MDPWS::WS_NS_DPWS, {});
  serialize(hosted.Types());
  closeTag("Types", MDPWS::WS_NS_DPWS);
  // ServiceId
  openTag("ServiceId", MDPWS::WS_NS_DPWS, {});
  serialize(hosted.ServiceId());
  closeTag("ServiceId", MDPWS::WS_NS_DPWS);
  closeTag("Hosted", MDPWS::WS_NS_DPWS);
}

void MessageSerializer::serialize(const BICEPS::MM::GetMdibResponse& getMdibResponse) {}

void MessageSerializer::openTag(const std::string& name, const frozen::string& xmlns,
                                const std::vector<Attribute>& attributes)
{
  const auto tagName = std::string(nsMap__.at(xmlns).data()) + ":" + name;
  sstream_ += "<" + tagName;
  for (const auto& attribute : attributes)
  {
    sstream_ += " " + attribute.name_ + "=\"" + attribute.value_ + "\"";
  }
  sstream_ += ">";
}
void MessageSerializer::closeTag(const std::string& name, const frozen::string& xmlns)
{
  const auto tagName = std::string(nsMap__.at(xmlns).data()) + ":" + name;
  sstream_ += "</" + tagName + ">";
}
