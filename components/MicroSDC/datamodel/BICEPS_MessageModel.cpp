#include "BICEPS_MessageModel.hpp"
#include "MDPWSConstants.hpp"
#include "SDCConstants.hpp"
#include <cstring>

namespace BICEPS::MM
{
  // GetMdibResponse
  //
  GetMdibResponse::GetMdibResponse(const MdibType& mdib)
    : Mdib_(mdib)
  {
  }

  // AbstractReport
  //

  const AbstractReport::MdibVersionOptional& AbstractReport::MdibVersion() const
  {
    return MdibVersion_;
  }
  AbstractReport::MdibVersionOptional& AbstractReport::MdibVersion()
  {
    return MdibVersion_;
  }
  const AbstractReport::SequenceIdType& AbstractReport::SequenceId() const
  {
    return SequenceId_;
  }
  AbstractReport::SequenceIdType& AbstractReport::SequenceId()
  {
    return SequenceId_;
  }
  const AbstractReport::InstanceIdOptional& AbstractReport::InstanceId() const
  {
    return InstanceId_;
  }
  AbstractReport::InstanceIdOptional& AbstractReport::InstanceId()
  {
    return InstanceId_;
  }

  // MetricReportPart
  //

  const MetricReportPart::MetricStateSequence& MetricReportPart::MetricState() const
  {
    return MetricState_;
  }
  MetricReportPart::MetricStateSequence& MetricReportPart::MetricState()
  {
    return MetricState_;
  }

  // AbstractMetricReport
  //

  const AbstractMetricReport::ReportPartSequence& AbstractMetricReport::ReportPart() const
  {
    return ReportPart_;
  }
  AbstractMetricReport::ReportPartSequence& AbstractMetricReport::ReportPart()
  {
    return ReportPart_;
  }

  AbstractMetricReport::AbstractMetricReport(const SequenceIdType& sequenceId)
    : AbstractReport(sequenceId)
  {
  }

  // EpisodicMetricReport
  //
  EpisodicMetricReport::EpisodicMetricReport(const SequenceIdType& sequenceId)
    : AbstractMetricReport(sequenceId)
  {
  }
  // OperationHandleRef
  //
  OperationHandleRef::OperationHandleRef(std::string operationHandleRef)
    : std::string(operationHandleRef)
  {
  }

  // AbstractSet
  //
  const AbstractSet::OperationHandleRefType& AbstractSet::OperationHandleRef() const
  {
    return OperationHandleRef_;
  }

  // SetValue
  //
  SetValue::SetValue(const rapidxml::xml_node<>& node)
  {
    this->parse(node);
  }
  void SetValue::parse(const rapidxml::xml_node<>& node)
  {
    for (const rapidxml::xml_node<>* entry = node.first_node(); entry != nullptr;
         entry = entry->next_sibling())
    {
      if (strncmp(entry->name(), "OperationHandleRef", entry->name_size()) == 0 &&
          strncmp(entry->xmlns(), ::MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE, entry->xmlns_size()) ==
              0)
      {
        OperationHandleRef_ =
            OperationHandleRefType(std::string(entry->value(), entry->value_size()));
      }
      else if (strncmp(entry->name(), "RequestedNumericValue", entry->name_size()) == 0 &&
               strncmp(entry->xmlns(), ::MDPWS::WS_NS_WSDL_XML_SCHEMA_INSTANCE,
                       entry->xmlns_size()) == 0)
      {
        RequestedNumericValue_ = std::stod(std::string(entry->value(), entry->value_size()));
      }
    }
  }

  // InvocationErrorMessage
  //
  InvocationErrorMessage::InvocationErrorMessage(std::string invocationError)
    : std::string(invocationError)
  {
  }

  // InvocationInfo
  //
  InvocationInfo::InvocationInfo(const TransactionIdType& transactionId,
                                 const InvocationStateType& invocationState)
    : TranscationId_(transactionId)
    , InvocationState_(invocationState)
  {
  }
  const InvocationInfo::TransactionIdType& InvocationInfo::TransactionId() const
  {
    return TranscationId_;
  }
  InvocationInfo::TransactionIdType& InvocationInfo::TransactionId()
  {
    return TranscationId_;
  }
  const InvocationInfo::InvocationStateType& InvocationInfo::InvocationState() const
  {
    return InvocationState_;
  }
  InvocationInfo::InvocationStateType& InvocationInfo::InvocationState()
  {
    return InvocationState_;
  }
  const InvocationInfo::InvocationErrorOptional& InvocationInfo::InvocationError() const
  {
    return InvocationError_;
  }
  InvocationInfo::InvocationErrorOptional& InvocationInfo::InvocationError()
  {
    return InvocationError_;
  }
  const InvocationInfo::InvocationErrorMessageOptional&
  InvocationInfo::InvocationErrorMessage() const
  {
    return InvocationErrorMessage_;
  }
  InvocationInfo::InvocationErrorMessageOptional& InvocationInfo::InvocationErrorMessage()
  {
    return InvocationErrorMessage_;
  }

  // AbstractSetResponse
  //
  AbstractSetResponse::AbstractSetResponse(const SequenceIdType& sequenceId,
                                           const InvocationInfoType& invocationInfo)
    : SequenceId_(sequenceId)
    , InvocationInfo_(invocationInfo)
  {
  }
  const AbstractSetResponse::MdibVersionOptional& AbstractSetResponse::MdibVersion() const
  {
    return MdibVersion_;
  }
  AbstractSetResponse::MdibVersionOptional& AbstractSetResponse::MdibVersion()
  {
    return MdibVersion_;
  }
  const AbstractSetResponse::SequenceIdType& AbstractSetResponse::SequenceId() const
  {
    return SequenceId_;
  }
  AbstractSetResponse::SequenceIdType& AbstractSetResponse::SequenceId()
  {
    return SequenceId_;
  }
  const AbstractSetResponse::InstanceIdOptional& AbstractSetResponse::InstanceId() const
  {
    return InstanceId_;
  }
  AbstractSetResponse::InstanceIdOptional& AbstractSetResponse::InstanceId()
  {
    return InstanceId_;
  }
  const AbstractSetResponse::InvocationInfoType& AbstractSetResponse::InvocationInfo() const
  {
    return InvocationInfo_;
  }
  AbstractSetResponse::InvocationInfoType& AbstractSetResponse::InvocationInfo()
  {
    return InvocationInfo_;
  }

  // ReportPart
  //
  ReportPart::ReportPart(const OperationHandleRefType& operationHandleRef,
                         const InvocationInfoType& invocationInfo,
                         const InvocationSourceType& invocationSource)
    : OperationHandleRef_(operationHandleRef)
    , InvocationInfo_(invocationInfo)
    , InvocationSource_(invocationSource)
  {
  }
  const ReportPart::OperationHandleRefType& ReportPart::OperationHandleRef() const
  {
    return OperationHandleRef_;
  }
  ReportPart::OperationHandleRefType& ReportPart::OperationHandleRef()
  {
    return OperationHandleRef_;
  }
  const ReportPart::OperationTargetOptional& ReportPart::OperationTarget() const
  {
    return OperationTarget_;
  }
  ReportPart::OperationTargetOptional& ReportPart::OperationTarget()
  {
    return OperationTarget_;
  }
  const ReportPart::InvocationInfoType& ReportPart::InvocationInfo() const
  {
    return InvocationInfo_;
  }
  ReportPart::InvocationInfoType& ReportPart::InvocationInfo()
  {
    return InvocationInfo_;
  }
  const ReportPart::InvocationSourceType& ReportPart::InvocationSource() const
  {
    return InvocationSource_;
  }
  ReportPart::InvocationSourceType& ReportPart::InvocationSource()
  {
    return InvocationSource_;
  }

  // OperationInvokedReport
  //
  OperationInvokedReport::OperationInvokedReport(const SequenceIdType& sequenceId,
                                                 const ReportPartType& reportPart)
    : AbstractReport(sequenceId)
    , ReportPart_(reportPart)
  {
  }
  const OperationInvokedReport::ReportPartType& OperationInvokedReport::ReportPart() const
  {
    return ReportPart_;
  }
  OperationInvokedReport::ReportPartType& OperationInvokedReport::ReportPart()
  {
    return ReportPart_;
  }
} // namespace BICEPS::MM