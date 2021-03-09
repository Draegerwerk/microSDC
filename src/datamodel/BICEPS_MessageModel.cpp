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
    : sequenceId(std::move(sequenceId))
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
    : kind_(kind)
    , operationHandleRef(std::move(operation_handle_ref))
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
        operationHandleRef = std::string(entry->value(), entry->value_size());
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
        requestedNumericValue = std::stod(std::string(entry->value(), entry->value_size()));
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