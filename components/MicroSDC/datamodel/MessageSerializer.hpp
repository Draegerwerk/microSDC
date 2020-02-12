#pragma once

#include "MDPWSConstants.hpp"
#include "MessageModel.hpp"
#include "NamespaceMap.hpp"
#include "SDCConstants.hpp"
#include <sstream>
#include <string>

class MessageSerializer
{
public:
  /**
   * @brief Construct a new MessageSerializer
   */
  MessageSerializer();
  /**
   * @brief get the serialized string
   */
  std::string str() const;

  void serialize(const MESSAGEMODEL::Envelope& message);
  void serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Envelope& message);
  void serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Header& header);
  void serialize(rapidxml::xml_node<>* parent, const MESSAGEMODEL::Body& body);
  void serialize(rapidxml::xml_node<>* parent, const WS::ADDRESSING::RelatesToType& relatesTo);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::ADDRESSING::EndpointReferenceType& endpointReference);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::AppSequenceType& appSequence);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::HelloType& hello);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ProbeMatchType& probeMatch);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ProbeMatchesType& probeMatches);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ResolveMatchType& resolveMatch);
  void serialize(rapidxml::xml_node<>* parent,
                 const WS::DISCOVERY::ResolveMatchesType& resolveMatches);
  void serialize(rapidxml::xml_node<>* parent, const WS::MEX::Metadata& metadata);
  void serialize(rapidxml::xml_node<>* parent, const WS::MEX::MetadataSection& metadataSection);
  void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::ScopesType& scopes);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::ThisModelType& thisModel);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::ThisDeviceType& thisDevice);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::Relationship& relationship);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::HostServiceType& host);
  void serialize(rapidxml::xml_node<>* parent, const WS::DPWS::HostedServiceType& hosted);
  void serialize(rapidxml::xml_node<>* parent, const BICEPS::MM::GetMdibResponse& getMdibResponse);

  static std::string toString(const WS::DISCOVERY::UriListType& uriList);
  static std::string toString(const WS::DISCOVERY::QNameListType& qNameList);
  //void serialize(rapidxml::xml_node<>* parent, const WS::DISCOVERY::QNameListType& qNameList);
private:
  /*
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
      */

  std::unique_ptr<rapidxml::xml_document<>> xmlDocument_;

  // std::string outString_;
};
