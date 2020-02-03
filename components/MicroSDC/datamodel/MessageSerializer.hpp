#pragma once

#include "MDPWSConstants.hpp"
#include "MessageModel.hpp"
#include "NamespaceMap.hpp"
#include "SDCConstants.hpp"
#include "frozen/string.h"
#include "frozen/unordered_map.h"
#include <sstream>
#include <string>

class MessageSerializer
{
public:
  struct Attribute
  {
    Attribute(const std::string& name, const std::string& value)
      : name_(name)
      , value_(value)
    {
    }

    const std::string name_;
    const std::string value_;
  };
  /**
   * @brief Construct a new MessageSerializer
   */
  MessageSerializer();
  /**
   * @brief get the serialized string
   */
  const std::string& str() const;

  void serialize(const std::string& string);

  void serialize(const MESSAGEMODEL::Envelope& message);
  void serialize(const MESSAGEMODEL::Header& header);
  void serialize(const MESSAGEMODEL::Body& Body);
  void serialize(const WS::ADDRESSING::URIType& uri);
  void serialize(const WS::ADDRESSING::RelatesToType& relatesTo);
  void serialize(const WS::ADDRESSING::EndpointReferenceType& endpointReference);
  void serialize(const WS::DISCOVERY::QNameListType& qNameList);
  void serialize(const WS::DISCOVERY::UriListType& uriList);
  void serialize(const WS::DISCOVERY::AppSequenceType& appSequence);
  void serialize(const WS::DISCOVERY::HelloType& hello);
  void serialize(const WS::DISCOVERY::ProbeMatchType& probeMatch);
  void serialize(const WS::DISCOVERY::ProbeMatchesType& probeMatches);
  void serialize(const WS::DISCOVERY::ResolveMatchType& resolveMatch);
  void serialize(const WS::DISCOVERY::ResolveMatchesType& resolveMatches);
  void serialize(const WS::MEX::Metadata& metadata);
  void serialize(const WS::MEX::MetadataSection& metadataSection);
  void serialize(const WS::DPWS::ThisModelType& thisModel);
  void serialize(const WS::DPWS::ThisDeviceType& thisDevice);
  void serialize(const WS::DPWS::Relationship& relationship);
  void serialize(const WS::DPWS::HostServiceType& host);
  void serialize(const WS::DPWS::HostedServiceType& hosted);
  void serialize(const BICEPS::MM::GetMdibResponse& getMdibResponse);

private:
  static constexpr frozen::unordered_map<frozen::string, frozen::string, 10> nsMap__ = {
      {MDPWS::WS_NS_SOAP_ENVELOPE, "soap"},
      {MDPWS::WS_NS_DISCOVERY, "wsd"},
      {MDPWS::WS_NS_ADDRESSING, "wsa"},
      {MDPWS::WS_NS_DPWS, "dpws"},
      {MDPWS::NS_MDPWS, "mdpws"},
      {MDPWS::WS_NS_METADATA_EXCHANGE, "mex"},
      {SDC::NS_GLUE, "glue"},
      {SDC::NS_BICEPS_MESSAGE_MODEL, "mm"},
      {SDC::NS_BICEPS_PARTICIPANT_MODEL, "pm"},
      {SDC::NS_BICEPS_EXTENSION, "ext"}};

  std::string sstream_;

  void openTag(const std::string& name, const frozen::string& xmlns,
               const std::vector<Attribute>& attributes);
  void closeTag(const std::string& name, const frozen::string& xmlns);
};
