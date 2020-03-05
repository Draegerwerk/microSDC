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

  const Header::ActionType& Header::Action() const
  {
    return Action_;
  }
  Header::ActionType& Header::Action()
  {
    return Action_;
  }

  const Header::AppSequenceOptional& Header::AppSequence() const
  {
    return AppSequence_;
  }
  Header::AppSequenceOptional& Header::AppSequence()
  {
    return AppSequence_;
  }

  const Header::FaultToOptional& Header::FaultTo() const
  {
    return FaultTo_;
  }
  Header::FaultToOptional& Header::FaultTo()
  {
    return FaultTo_;
  }

  const Header::FromOptional& Header::From() const
  {
    return From_;
  }
  Header::FromOptional& Header::From()
  {
    return From_;
  }

  const Header::MessageIDOptional& Header::MessageID() const
  {
    return MessageID_;
  }
  Header::MessageIDOptional& Header::MessageID()
  {
    return MessageID_;
  }

  const Header::ReferenceParametersOptional& Header::ReferenceParameters() const
  {
    return ReferenceParameters_;
  }
  Header::ReferenceParametersOptional& Header::ReferenceParameters()
  {
    return ReferenceParameters_;
  }

  const Header::RelatesToOptional& Header::RelatesTo() const
  {
    return RelatesTo_;
  }
  Header::RelatesToOptional& Header::RelatesTo()
  {
    return RelatesTo_;
  }

  const Header::ReplyToOptional& Header::ReplyTo() const
  {
    return ReplyTo_;
  }
  Header::ReplyToOptional& Header::ReplyTo()
  {
    return ReplyTo_;
  }

  const Header::ToOptional& Header::To() const
  {
    return To_;
  }
  Header::ToOptional& Header::To()
  {
    return To_;
  }

  const Header::IdentifierOptional& Header::Identifier() const
  {
    return Identifier_;
  }
  Header::IdentifierOptional& Header::Identifier()
  {
    return Identifier_;
  }

  void Header::parse(const rapidxml::xml_node<>& node)
  {
    // Find mandatory action node
    auto actionNode = node.first_node("Action", MDPWS::WS_NS_ADDRESSING);
    if (actionNode == nullptr)
    {
      throw ExpectedElement("Action", MDPWS::WS_NS_ADDRESSING);
    }
    Action_ = ActionType(*actionNode);

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
        MessageID_ = std::make_optional<MessageIDType>(*entry);
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
        ReplyTo_ = std::make_optional<ReplyToType>(*entry);
      }
      else if (strncmp(entry->name(), "To", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_ADDRESSING, entry->xmlns_size()) == 0)
      {
        To_ = std::make_optional<ToType>(*entry);
      }
      else if (strncmp(entry->name(), "Identifier", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), MDPWS::WS_NS_EVENTING, entry->xmlns_size()) == 0)
      {
        Identifier_ = std::make_optional<IdentifierType>(*entry);
      }
    }
  }

  // Body
  //

  Body::Body(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }

  const Body::ByeOptional& Body::Bye() const
  {
    return Bye_;
  }

  Body::ByeOptional& Body::Bye()
  {
    return Bye_;
  }

  const Body::HelloOptional& Body::Hello() const
  {
    return Hello_;
  }

  Body::HelloOptional& Body::Hello()
  {
    return Hello_;
  }

  void Body::Hello(const HelloType& x)
  {
    Hello_ = x;
  }

  const Body::ProbeOptional& Body::Probe() const
  {
    return Probe_;
  }

  Body::ProbeOptional& Body::Probe()
  {
    return Probe_;
  }

  const Body::ProbeMatchesOptional& Body::ProbeMatches() const
  {
    return ProbeMatches_;
  }

  Body::ProbeMatchesOptional& Body::ProbeMatches()
  {
    return ProbeMatches_;
  }

  void Body::ProbeMatches(const ProbeMatchesType& x)
  {
    ProbeMatches_ = x;
  }

  const Body::ResolveOptional& Body::Resolve() const
  {
    return Resolve_;
  }

  Body::ResolveOptional& Body::Resolve()
  {
    return Resolve_;
  }

  const Body::ResolveMatchesOptional& Body::ResolveMatches() const
  {
    return ResolveMatches_;
  }

  Body::ResolveMatchesOptional& Body::ResolveMatches()
  {
    return ResolveMatches_;
  }

  const Body::GetMetadataOptional& Body::GetMetadata() const
  {
    return GetMetadata_;
  }
  Body::GetMetadataOptional& Body::GetMetadata()
  {
    return GetMetadata_;
  }

  const Body::MetadataOptional& Body::Metadata() const
  {
    return Metadata_;
  }
  Body::MetadataOptional& Body::Metadata()
  {
    return Metadata_;
  }

  const Body::GetMdibOptional& Body::GetMdib() const
  {
    return GetMdib_;
  }
  Body::GetMdibOptional& Body::GetMdib()
  {
    return GetMdib_;
  }

  const Body::GetMdibResponseOptional& Body::GetMdibResponse() const
  {
    return GetMdibResponse_;
  }
  Body::GetMdibResponseOptional& Body::GetMdibResponse()
  {
    return GetMdibResponse_;
  }

  const Body::SubscribeOptional& Body::Subscribe() const
  {
    return Subscribe_;
  }
  Body::SubscribeOptional& Body::Subscribe()
  {
    return Subscribe_;
  }

  const Body::SubscribeResponseOptional& Body::SubscribeResponse() const
  {
    return SubscribeResponse_;
  }
  Body::SubscribeResponseOptional& Body::SubscribeResponse()
  {
    return SubscribeResponse_;
  }

  const Body::RenewOptional& Body::Renew() const
  {
    return Renew_;
  }
  Body::RenewOptional& Body::Renew()
  {
    return Renew_;
  }

  const Body::RenewResponseOptional& Body::RenewResponse() const
  {
    return RenewResponse_;
  }
  Body::RenewResponseOptional& Body::RenewResponse()
  {
    return RenewResponse_;
  }

  const Body::UnsubscribeOptional& Body::Unsubscribe() const
  {
    return Unsubscribe_;
  }
  Body::UnsubscribeOptional& Body::Unsubscribe()
  {
    return Unsubscribe_;
  }

  const Body::SetValueOptional& Body::SetValue() const
  {
    return SetValue_;
  }
  Body::SetValueOptional& Body::SetValue()
  {
    return SetValue_;
  }

  const Body::SetValueResponseOptional& Body::SetValueResponse() const
  {
    return SetValueResponse_;
  }
  Body::SetValueResponseOptional& Body::SetValueResponse()
  {
    return SetValueResponse_;
  }

  const Body::EpisodicMetricReportOptional& Body::EpisodicMetricReport() const
  {
    return EpisodicMetricReport_;
  }
  Body::EpisodicMetricReportOptional& Body::EpisodicMetricReport()
  {
    return EpisodicMetricReport_;
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
      Probe_ = std::make_optional<ProbeType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Resolve", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_DISCOVERY, bodyContent->xmlns_size()) == 0)
    {
      Resolve_ = std::make_optional<ResolveType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "GetMetadata", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_METADATA_EXCHANGE,
                     bodyContent->xmlns_size()) == 0)
    {
      GetMetadata_ = std::make_optional<GetMetadataType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Subscribe", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_EVENTING, bodyContent->xmlns_size()) == 0)
    {
      Subscribe_ = std::make_optional<SubscribeType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Renew", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_EVENTING, bodyContent->xmlns_size()) == 0)
    {
      Renew_ = std::make_optional<RenewType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "Unsubscribe", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), MDPWS::WS_NS_EVENTING, bodyContent->xmlns_size()) == 0)
    {
      Unsubscribe_ = std::make_optional<UnsubscribeType>(*bodyContent);
    }
    else if (strncmp(bodyContent->name(), "SetValue", bodyContent->name_size()) == 0 &&
             strncmp(bodyContent->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL,
                     bodyContent->xmlns_size()) == 0)
    {
      SetValue_ = std::make_optional<SetValueType>(*bodyContent);
    }
  }

  // Envelope
  //

  const Envelope::HeaderType& Envelope::Header() const
  {
    return Header_;
  }

  Envelope::HeaderType& Envelope::Header()
  {
    return Header_;
  }

  const Envelope::BodyType& Envelope::Body() const
  {
    return Body_;
  }

  Envelope::BodyType& Envelope::Body()
  {
    return Body_;
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
    Header_ = HeaderType(*headerNode);

    auto bodyNode = node.first_node("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
    if (bodyNode == nullptr)
    {
      throw ExpectedElement("Body", MDPWS::WS_NS_SOAP_ENVELOPE);
    }
    Body_ = BodyType(*bodyNode);
  }
} // namespace MESSAGEMODEL
