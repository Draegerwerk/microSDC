#include "MessageModel.hpp"
#include "ExpectedElement.hpp"
#include "MDPWSConstants.hpp"
#include "SDCConstants.hpp"
#include "ws-eventing.hpp"
#include <cstring>
#include <memory>
#include <boost/optional.hpp>

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
    auto* action_node = node.first_node("Action", MDPWS::WS_NS_ADDRESSING);
    if (action_node == nullptr)
    {
      throw ExpectedElement("Action", MDPWS::WS_NS_ADDRESSING);
    }
    action = ActionType(*action_node);

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
        message_id = boost::optional<MessageIDType>(*entry);
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
        reply_to = boost::optional<ReplyToType>(*entry);
      }
      else if (strncmp(entry->name(), "To", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        to = boost::optional<ToType>(*entry);
      }
      else if (strncmp(entry->name(), "Identifier", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        identifier = boost::optional<IdentifierType>(*entry);
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
    rapidxml::xml_node<>* body_content = node.first_node();
    if (body_content == nullptr)
    {
      // Received empty Body node
      return;
    }
    if (strncmp(body_content->name(), "Probe", body_content->name_size()) == 0 &&
        strncmp(body_content->xmlns(), MDPWS::WS_NS_DISCOVERY, body_content->xmlns_size()) == 0)
    {
      probe = boost::optional<ProbeType>(*body_content);
    }
    else if (strncmp(body_content->name(), "Resolve", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), MDPWS::WS_NS_DISCOVERY, body_content->xmlns_size()) ==
                 0)
    {
      resolve = boost::optional<ResolveType>(*body_content);
    }
    else if (strncmp(body_content->name(), "GetMetadata", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), MDPWS::WS_NS_METADATA_EXCHANGE,
                     body_content->xmlns_size()) == 0)
    {
      get_metadata = boost::optional<GetMetadataType>(*body_content);
    }
    else if (strncmp(body_content->name(), "Subscribe", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), MDPWS::WS_NS_EVENTING, body_content->xmlns_size()) == 0)
    {
      subscribe = boost::optional<SubscribeType>(*body_content);
    }
    else if (strncmp(body_content->name(), "Renew", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), MDPWS::WS_NS_EVENTING, body_content->xmlns_size()) == 0)
    {
      renew = boost::optional<RenewType>(*body_content);
    }
    else if (strncmp(body_content->name(), "Unsubscribe", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), MDPWS::WS_NS_EVENTING, body_content->xmlns_size()) == 0)
    {
      unsubscribe = boost::optional<UnsubscribeType>(*body_content);
    }
    else if (strncmp(body_content->name(), "SetString", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL,
                     body_content->xmlns_size()) == 0)
    {
      set_string = boost::optional<SetStringType>(*body_content);
    }
    else if (strncmp(body_content->name(), "SetValue", body_content->name_size()) == 0 &&
             strncmp(body_content->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL,
                     body_content->xmlns_size()) == 0)
    {
      set_value = boost::optional<SetValueType>(*body_content);
    }
  }


  Envelope::Envelope(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  void Envelope::parse(const rapidxml::xml_node<>& node)
  {
    auto* header_node = node.first_node("Header", MDPWS::WS_NS_SOAP_ENVELOPE);
    if (header_node == nullptr)
    {
      throw ExpectedElement("Header", MDPWS::WS_NS_SOAP_ENVELOPE);
    }
    header = HeaderType(*header_node);

    auto* body_node = node.first_node("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
    if (body_node == nullptr)
    {
      throw ExpectedElement("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
    }
    body = BodyType(*body_node);
  }
} // namespace MESSAGEMODEL
