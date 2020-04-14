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

    explicit GetMdibResponse(MdibType mdib);

  private:
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

  private:
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

  private:
    SourceMdsOptional SourceMds_;
  };

  class MetricReportPart : public AbstractReportPart
  {
  public:
    using MetricStateType = std::shared_ptr<const PM::AbstractMetricState>;
    using MetricStateSequence = std::vector<MetricStateType>;
    const MetricStateSequence& MetricState() const;
    MetricStateSequence& MetricState();

  private:
    MetricStateSequence MetricState_;
  };

  class OperationalStateReportPart : AbstractReportPart
  {
  public:
    using OperationStateType = std::shared_ptr<const PM::AbstractOperationState>;
    using OperationStateSequence = std::vector<OperationStateType>;
    const OperationStateSequence& OperationState() const;
    OperationStateSequence& OperationState();

  private:
    OperationStateSequence OperationState_;
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

  private:
    ReportPartSequence ReportPart_;
  };

  class AbstractOperationalStateReport : public AbstractReport
  {
  public:
    using ReportPartType = OperationalStateReportPart;
    using ReportPartSequence = std::vector<ReportPartType>;
    const ReportPartSequence& ReportPart() const;
    ReportPartSequence& ReportPart();

  protected:
    explicit AbstractOperationalStateReport(const SequenceIdType& sequenceId);

  private:
    ReportPartSequence ReportPart_;
  };

  class EpisodicMetricReport : public AbstractMetricReport
  {
  public:
    explicit EpisodicMetricReport(const SequenceIdType& sequenceId);
  };

  class EpisodicOperationalStateReport : public AbstractOperationalStateReport
  {
  public:
    explicit EpisodicOperationalStateReport(const SequenceIdType& sequenceId);
  };

  // SetValue
  //
  class OperationHandleRef : public std::string
  {
  public:
    explicit OperationHandleRef(std::string operationHandleRef);
    OperationHandleRef() = default;
  };

  class AbstractSet
  {
  public:
    using OperationHandleRefType = ::BICEPS::MM::OperationHandleRef;
    const OperationHandleRefType& OperationHandleRef() const;
    OperationHandleRefType& OperationHandleRef();

  private:
    OperationHandleRefType OperationHandleRef_;

    virtual void parse(const rapidxml::xml_node<>& node) = 0;
  };

  class SetValue : public AbstractSet
  {
  public:
    using RequestedNumericValueType = double;
    const RequestedNumericValueType& RequestedNumericValue() const;

    explicit SetValue(const rapidxml::xml_node<>& node);

  private:
    RequestedNumericValueType RequestedNumericValue_{};

    void parse(const rapidxml::xml_node<>& node) override;
  };

  // SetValueResponse
  //
  class InvocationErrorMessage : public std::string
  {
  public:
    explicit InvocationErrorMessage(std::string invocationErrorMessage);
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

  private:
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

    AbstractSetResponse(SequenceIdType sequenceId, InvocationInfoType invocationInfo);

  private:
    MdibVersionOptional MdibVersion_;
    SequenceIdType SequenceId_;
    InstanceIdOptional InstanceId_;
    InvocationInfoType InvocationInfo_;
  };

  class SetValueResponse : public AbstractSetResponse
  {
    // use constructors of base class
    using AbstractSetResponse::AbstractSetResponse;
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

    ReportPart(OperationHandleRefType operationHandleRef, InvocationInfoType invocationInfo,
               InvocationSourceType invocationSource);

  private:
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

    OperationInvokedReport(const SequenceIdType& sequenceId, ReportPartType reportPart);

  private:
    ReportPartType ReportPart_;
  };

} // namespace BICEPS::MM
