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

  SetValue::SetValue(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void SetValue::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (entry->name() != nullptr &&
          strncmp(entry->name(), "OperationHandleRef", entry->name_size()) == 0 &&
          entry->xmlns() != nullptr &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE, entry->xmlns_size()) ==
              0)
      {
        AbstractSet::operationHandleRef = std::string(entry->value(), entry->value_size());
      }
      else if (entry->name() != nullptr &&
               strncmp(entry->name(), "RequestedNumericValue", entry->name_size()) == 0 &&
               entry->xmlns() != nullptr &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE,
                       entry->xmlns_size()) == 0)
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