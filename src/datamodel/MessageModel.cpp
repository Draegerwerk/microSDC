#include "MessageModel.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include "SDCConstants.hpp"
#include "ws-eventing.hpp"
#include <cstring>
#include <memory>
#include <optional>

namespace MESSAGEMODEL
{
  // Header
  //

  Header::Header(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void Header::parse(const rapidxml::xml_node<>& node)
  {
    // Find mandatory action node
    auto actionNode = node.first_node("Action", MDPWS::WS_NS_ADDRESSING);
    if (actionNode == nullptr)
    {
      throw ExpectedElement("Action", MDPWS::WS_NS_ADDRESSING);
    }
    action = ActionType(*actionNode);

    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (strncmp(entry->name(), "AppSequence", entry->name_size()) == 0 &&
          strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        // AppSequence
      }
      else if (strncmp(entry->name(), "FaultTo", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        // FaultTo
      }
      else if (strncmp(entry->name(), "From", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        // From
      }
      else if (strncmp(entry->name(), "MessageID", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        messageID = std::make_optional<MessageIDType>(*entry);
      }
      else if (strncmp(entry->name(), "ReferenceParameters", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        // ReferenceParameters
      }
      else if (strncmp(entry->name(), "RelatesTo", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        // RelatesTo
      }
      else if (strncmp(entry->name(), "ReplyTo", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        replyTo = std::make_optional<ReplyToType>(*entry);
      }
      else if (strncmp(entry->name(), "To", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        to = std::make_optional<ToType>(*entry);
      }
      else if (strncmp(entry->name(), "Identifier", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        identifier = std::make_optional<IdentifierType>(*entry);
      }
    }
  }

  // Body
  //

  Body::Body(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void Body::parse(const rapidxml::xml_node<>& node)
  {
    rapidxml::xml_node<>* bodyContent = node.first_node();
    if (bodyContent == nullptr)
    {
      // Received empty Body node
      return;
    }
    if (strncmp(bodyContent->name(), "Probe", bodyContent->name_size()) == 0 &&
        strncmp(bodyContent->xmlns(), MDPWS::WS_NS_DISCOVERY, bodyContent->xmlns_size()) == 0)
    {
      probe = std::make_optional<ProbeType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Resolve", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_DISCOVERY, bodyContent->xmlns_size()) == 0)
    {
      resolve = std::make_optional<ResolveType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "GetMetadata", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_METADATA_EXCHANGE,
                     bodyContent->xmlns_size()) == 0)
    {
      getMetadata = std::make_optional<GetMetadataType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Subscribe", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_EVENTING, bodyContent->xmlns_size()) == 0)
    {
      subscribe = std::make_optional<SubscribeType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Renew", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_EVENTING, bodyContent->xmlns_size()) == 0)
    {
      renew = std::make_optional<RenewType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Unsubscribe", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_EVENTING, bodyContent->xmlns_size()) == 0)
    {
      unsubscribe = std::make_optional<UnsubscribeType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "SetString", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL,
                     bodyContent->xmlns_size()) == 0)
    {
      setString = std::make_optional<SetStringType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "SetValue", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL,
                     bodyContent->xmlns_size()) == 0)
    {
      setValue = std::make_optional<SetValueType>(*bodyContent);
    }
  }


  Envelope::Envelope(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void Envelope::parse(const rapidxml::xml_node<>& node)
  {
    auto headerNode = node.first_node("Header", MDPWS::WS_NS_SOAP_ENVELOPE);
    if (headerNode == nullptr)
    {
      throw ExpectedElement("Header", MDPWS::WS_NS_SOAP_ENVELOPE);
    }
    header = HeaderType(*headerNode);

    auto bodyNode = node.first_node("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
    if (bodyNode == nullptr)
    {
      throw ExpectedElement("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
    }
    body = BodyType(*bodyNode);
  }
} // namespace MESSAGEMODEL
