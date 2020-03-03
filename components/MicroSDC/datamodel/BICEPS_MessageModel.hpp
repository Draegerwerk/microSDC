#pragma once

#include "BICEPS_ParticipantModel.hpp"
#include "ws-addressing.hpp"
#include <string>
#include <utility>

namespace BICEPS::MM
{
  enum class InvocationState
  {
    Wait,
    Start,
    Cnclld,
    CnclldMan,
    Fin,
    FinMod,
    Fail
  };

  enum class InvocationError
  {
    Unspec,
    Unkn,
    Inv,
    Oth
  };

  class GetMdib
  {
  };

  class GetMdibResponse
  {
  public:
    using MdibType = BICEPS::PM::Mdib;
    const MdibType& Mdib() const;
    MdibType& Mdib();

    GetMdibResponse(const MdibType& mdib);

  protected:
    MdibType Mdib_;
  };

  class AbstractReport
  {
  public:
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    const MdibVersionOptional& MdibVersion() const;
    MdibVersionOptional& MdibVersion();

    using SequenceIdType = WS::ADDRESSING::URIType;
    const SequenceIdType& SequenceId() const;
    SequenceIdType& SequenceId();

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    const InstanceIdOptional& InstanceId() const;
    InstanceIdOptional& InstanceId();

  protected:
    // Constructor
    //
    explicit AbstractReport(WS::ADDRESSING::URIType sequenceId)
      : SequenceId_(std::move(sequenceId))
    {
    }

  protected:
    MdibVersionOptional MdibVersion_;
    SequenceIdType SequenceId_;
    InstanceIdOptional InstanceId_;
  };

  class AbstractReportPart
  {
  public:
    using SourceMdsType = std::string;
    using SourceMdsOptional = std::optional<SourceMdsType>;

  protected:
    AbstractReportPart() = default;

  protected:
    SourceMdsOptional SourceMds_;
  };

  class MetricReportPart : public AbstractReportPart
  {
  public:
    using MetricStateType = std::shared_ptr<const PM::AbstractMetricState>;
    using MetricStateSequence = std::vector<MetricStateType>;
    const MetricStateSequence& MetricState() const;
    MetricStateSequence& MetricState();

  protected:
    MetricStateSequence MetricState_;
  };

  class AbstractMetricReport : public AbstractReport
  {
  public:
    using ReportPartType = MetricReportPart;
    using ReportPartSequence = std::vector<ReportPartType>;
    const ReportPartSequence& ReportPart() const;
    ReportPartSequence& ReportPart();

  protected:
    explicit AbstractMetricReport(const SequenceIdType& sequenceId);

  protected:
    ReportPartSequence ReportPart_;
  };

  class EpisodicMetricReport : public AbstractMetricReport
  {
  public:
    explicit EpisodicMetricReport(const SequenceIdType& sequenceId);
  };
  // SetValue
  //
  class OperationHandleRef : public std::string
  {
  public:
    OperationHandleRef(std::string operationHandleRef);
    OperationHandleRef() = default;
  };

  class AbstractSet
  {
  public:
    using OperationHandleRefType = ::BICEPS::MM::OperationHandleRef;
    const OperationHandleRefType& OperationHandleRef() const;

  protected:
    OperationHandleRefType OperationHandleRef_;

    virtual void parse(const rapidxml::xml_node<>& node) = 0;
  };

  class SetValue : public AbstractSet
  {
  public:
    using RequestedNumericValueType = double;
    const RequestedNumericValueType& RequestedNumericValue() const;

    SetValue(const rapidxml::xml_node<>& node);

  protected:
    RequestedNumericValueType RequestedNumericValue_;

    void parse(const rapidxml::xml_node<>& node);
  };

  // SetValueResponse
  //
  class InvocationErrorMessage : public std::string
  {
  public:
    InvocationErrorMessage(std::string invocationErrorMessage);
  };

  class InvocationInfo
  {
  public:
    using TransactionIdType = unsigned int;
    const TransactionIdType& TransactionId() const;
    TransactionIdType& TransactionId();

    using InvocationStateType = ::BICEPS::MM::InvocationState;
    const InvocationStateType& InvocationState() const;
    InvocationStateType& InvocationState();

    using InvocationErrorType = ::BICEPS::MM::InvocationError;
    using InvocationErrorOptional = std::optional<InvocationErrorType>;
    const InvocationErrorOptional& InvocationError() const;
    InvocationErrorOptional& InvocationError();

    using InvocationErrorMessageType = ::BICEPS::MM::InvocationErrorMessage;
    using InvocationErrorMessageOptional = std::optional<InvocationErrorMessageType>;
    const InvocationErrorMessageOptional& InvocationErrorMessage() const;
    InvocationErrorMessageOptional& InvocationErrorMessage();

    InvocationInfo(const TransactionIdType& transactionId,
                   const InvocationStateType& invocationState);
    InvocationInfo() = default;

  protected:
    TransactionIdType TranscationId_;
    InvocationStateType InvocationState_;
    InvocationErrorOptional InvocationError_;
    InvocationErrorMessageOptional InvocationErrorMessage_;
  };

  class AbstractSetResponse
  {
  public:
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    const MdibVersionOptional& MdibVersion() const;
    MdibVersionOptional& MdibVersion();

    using SequenceIdType = WS::ADDRESSING::URIType;
    const SequenceIdType& SequenceId() const;
    SequenceIdType& SequenceId();

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    const InstanceIdOptional& InstanceId() const;
    InstanceIdOptional& InstanceId();

    using InvocationInfoType = ::BICEPS::MM::InvocationInfo;
    const InvocationInfoType& InvocationInfo() const;
    InvocationInfoType& InvocationInfo();

    AbstractSetResponse(const SequenceIdType& sequenceId, const InvocationInfoType& invocationInfo);
    AbstractSetResponse() = default;

  protected:
    MdibVersionOptional MdibVersion_;
    SequenceIdType SequenceId_;
    InstanceIdOptional InstanceId_;
    InvocationInfoType InvocationInfo_;
  };

  class SetValueReponse : public AbstractSetResponse
  {
  };

  // OperationInvokedReport
  //
  class OperationTarget : public std::string
  {
  };

  class InvocationSource
  {
  };

  class ReportPart
  {
  public:
    using OperationHandleRefType = ::BICEPS::MM::OperationHandleRef;
    const OperationHandleRefType& OperationHandleRef() const;
    OperationHandleRefType& OperationHandleRef();

    using OperationTargetType = ::BICEPS::MM::OperationTarget;
    using OperationTargetOptional = std::optional<OperationTargetType>;
    const OperationTargetOptional& OperationTarget() const;
    OperationTargetOptional& OperationTarget();

    using InvocationInfoType = ::BICEPS::MM::InvocationInfo;
    const InvocationInfoType& InvocationInfo() const;
    InvocationInfoType& InvocationInfo();

    using InvocationSourceType = ::BICEPS::MM::InvocationSource;
    const InvocationSourceType& InvocationSource() const;
    InvocationSourceType& InvocationSource();

    ReportPart(const OperationHandleRefType& operationHandleRef,
               const InvocationInfoType& invocationInfo,
               const InvocationSourceType& invocationSource);
    ReportPart() = default;

  protected:
    OperationHandleRefType OperationHandleRef_;
    OperationTargetOptional OperationTarget_;
    InvocationInfoType InvocationInfo_;
    InvocationSourceType InvocationSource_;
  };

  class OperationInvokedReport : public AbstractReport
  {
  public:
    using ReportPartType = ::BICEPS::MM::ReportPart;
    const ReportPartType& ReportPart() const;
    ReportPartType& ReportPart();

    OperationInvokedReport(const SequenceIdType& sequenceId, const ReportPartType& reportPart);
    OperationInvokedReport() = default;

  protected:
    ReportPartType ReportPart_;
  };

} // namespace BICEPS::MM
