#include "BICEPS_MessageModel.hpp"
#include "MDPWSConstants.hpp"
#include "SDCConstants.hpp"
#include <cstring>
#include <utility>

namespace BICEPS::MM
{
  // GetMdibResponse
  //
  GetMdibResponse::GetMdibResponse(MdibType mdib)
    : mdib(std::move(mdib))
  {
  }

  AbstractReport::AbstractReport(WS::ADDRESSING::URIType sequenceId)
    : sequence_id(std::move(sequenceId))
  {
  }

  AbstractMetricReport::AbstractMetricReport(const SequenceIdType& sequenceId)
    : AbstractReport(sequenceId)
  {
  }

  EpisodicMetricReport::EpisodicMetricReport(const SequenceIdType& sequenceId)
    : AbstractMetricReport(sequenceId)
  {
  }

  AbstractSet::AbstractSet(SetKind kind, OperationHandleRefType operation_handle_ref)
    : operation_handle_ref(std::move(operation_handle_ref))
    , kind_(kind)
  {
  }
  AbstractSet::AbstractSet(SetKind kind, const rapidxml::xml_node<>& node)
    : kind_(kind)
  {
    this->parse(node);
  }
  AbstractSet::SetKind AbstractSet::get_kind() const
  {
    return kind_;
  }
  void AbstractSet::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() != nullptr &&
          strncmp(entry->name(), "OperationHandleRef", entry->name_size()) == 0 &&
          entry->xmlns() != nullptr &&
          strncmp(entry->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL, entry->xmlns_size()) == 0)
      {
        operation_handle_ref = std::string(entry->value(), entry->value_size());
      }
    }
  }

  SetValue::SetValue(const rapidxml::xml_node<>& node)
    : AbstractSet(SetKind::SET_VALUE, node)
  {
    this->parse(node);
  }
  bool SetValue::classof(const AbstractSet* other)
  {
    return other->get_kind() == SetKind::SET_VALUE;
  }
  void SetValue::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() != nullptr &&
          strncmp(entry->name(), "RequestedNumericValue", entry->name_size()) == 0 &&
          entry->xmlns() != nullptr &&
          strncmp(entry->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL, entry->xmlns_size()) == 0)
      {
        requested_numeric_value = std::stod(std::string(entry->value(), entry->value_size()));
      }
    }
  }

  SetString::SetString(const rapidxml::xml_node<>& node)
    : AbstractSet(SetKind::SET_STRING, node)
  {
    this->parse(node);
  }
  bool SetString::classof(const AbstractSet* other)
  {
    return other->get_kind() == SetKind::SET_STRING;
  }
  void SetString::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() != nullptr &&
          strncmp(entry->name(), "RequestedStringValue", entry->name_size()) == 0 &&
          entry->xmlns() != nullptr &&
          strncmp(entry->xmlns(), SDC::NS_BICEPS_MESSAGE_MODEL, entry->xmlns_size()) == 0)
      {
        requestedStringValue = std::string(entry->value(), entry->value_size());
      }
    }
  }

  InvocationErrorMessage::InvocationErrorMessage(std::string invocationError)
    : std::string(std::move(invocationError))
  {
  }

  InvocationInfo::InvocationInfo(TransactionIdType transactionId,
                                 InvocationStateType invocationState)
    : transactionId(transactionId)
    , invocationState(invocationState)
  {
  }

  AbstractSetResponse::AbstractSetResponse(SequenceIdType sequenceId,
                                           InvocationInfoType invocationInfo)
    : sequenceId(std::move(sequenceId))
    , invocationInfo(std::move(invocationInfo))
  {
  }

  ReportPart::ReportPart(OperationHandleRefType operationHandleRef,
                         InvocationInfoType invocationInfo, InvocationSourceType invocationSource)
    : operationHandleRef(std::move(operationHandleRef))
    , invocationInfo(std::move(invocationInfo))
    , invocationSource(invocationSource)
  {
  }

  OperationInvokedReport::OperationInvokedReport(const SequenceIdType& sequenceId,
                                                 ReportPartType reportPart)
    : AbstractReport(sequenceId)
    , reportPart(std::move(reportPart))
  {
  }
} // namespace BICEPS::MM