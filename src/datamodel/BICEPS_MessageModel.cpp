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
    : Mdib(std::move(mdib))
  {
  }

  AbstractReport::AbstractReport(WS::ADDRESSING::URIType sequenceId)
    : SequenceId(std::move(sequenceId))
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
        AbstractSet::OperationHandleRef = std::string(entry->value(), entry->value_size());
      }
      else if (entry->name() != nullptr &&
               strncmp(entry->name(), "RequestedNumericValue", entry->name_size()) == 0 &&
               entry->xmlns() != nullptr &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE,
                       entry->xmlns_size()) == 0)
      {
        RequestedNumericValue = std::stod(std::string(entry->value(), entry->value_size()));
      }
    }
  }

  InvocationErrorMessage::InvocationErrorMessage(std::string invocationError)
    : std::string(std::move(invocationError))
  {
  }

  InvocationInfo::InvocationInfo(TransactionIdType transactionId,
                                 InvocationStateType invocationState)
    : TransactionId(transactionId)
    , InvocationState(invocationState)
  {
  }

  AbstractSetResponse::AbstractSetResponse(SequenceIdType sequenceId,
                                           InvocationInfoType invocationInfo)
    : SequenceId(std::move(sequenceId))
    , InvocationInfo(std::move(invocationInfo))
  {
  }

  ReportPart::ReportPart(OperationHandleRefType operationHandleRef,
                         InvocationInfoType invocationInfo, InvocationSourceType invocationSource)
    : OperationHandleRef(std::move(operationHandleRef))
    , InvocationInfo(std::move(invocationInfo))
    , InvocationSource(invocationSource)
  {
  }

  OperationInvokedReport::OperationInvokedReport(const SequenceIdType& sequenceId,
                                                 ReportPartType reportPart)
    : AbstractReport(sequenceId)
    , ReportPart(std::move(reportPart))
  {
  }
} // namespace BICEPS::MM