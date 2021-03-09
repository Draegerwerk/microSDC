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

  struct GetMdib
  {
  };

  struct GetMdibResponse
  {
    using MdibType = BICEPS::PM::Mdib;
    MdibType mdib;

    explicit GetMdibResponse(MdibType mdib);
  };

  struct AbstractReport
  {
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    MdibVersionOptional mdibVersion;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType sequenceId;

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    InstanceIdOptional instanceId;

  protected:
    explicit AbstractReport(WS::ADDRESSING::URIType sequenceId);
  };

  struct AbstractReportPart
  {
    using SourceMdsType = std::string;
    using SourceMdsOptional = std::optional<SourceMdsType>;
    SourceMdsOptional sourceMds;

  protected:
    AbstractReportPart() = default;
  };

  struct MetricReportPart : public AbstractReportPart
  {
    using MetricStateType = std::shared_ptr<const PM::AbstractMetricState>;
    using MetricStateSequence = std::vector<MetricStateType>;
    MetricStateSequence metricState;
  };

  struct AbstractMetricReport : public AbstractReport
  {
    using ReportPartType = MetricReportPart;
    using ReportPartSequence = std::vector<ReportPartType>;
    ReportPartSequence reportPart;

  protected:
    explicit AbstractMetricReport(const SequenceIdType& sequenceId);
  };

  struct EpisodicMetricReport : public AbstractMetricReport
  {
    explicit EpisodicMetricReport(const SequenceIdType& sequenceId);
  };

  struct OperationHandleRef : public std::string
  {
    using std::string::string;
    using std::string::operator=;
  };

  struct AbstractSet
  {
    enum class SetKind
    {
      SET_VALUE,
    };
    SetKind get_kind() const;

    using OperationHandleRefType = ::BICEPS::MM::OperationHandleRef;
    OperationHandleRefType operationHandleRef;


  protected:
    AbstractSet(SetKind kind, OperationHandleRefType operation_handle_ref);
    explicit AbstractSet(SetKind kind, const rapidxml::xml_node<>& node);

  private:
    SetKind kind_;
    void parse(const rapidxml::xml_node<>& node);
  };

  struct SetValue : public AbstractSet
  {
    using RequestedNumericValueType = double;
    RequestedNumericValueType requestedNumericValue{0.0};

    explicit SetValue(const rapidxml::xml_node<>& node);
    static bool classof(const AbstractSet* other);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct InvocationErrorMessage : public std::string
  {
    explicit InvocationErrorMessage(std::string invocationErrorMessage);
  };

  struct InvocationInfo
  {
    using TransactionIdType = unsigned int;
    TransactionIdType transactionId;

    using InvocationStateType = ::BICEPS::MM::InvocationState;
    InvocationStateType invocationState;

    using InvocationErrorType = ::BICEPS::MM::InvocationError;
    using InvocationErrorOptional = std::optional<InvocationErrorType>;
    InvocationErrorOptional invocationError;

    using InvocationErrorMessageType = ::BICEPS::MM::InvocationErrorMessage;
    using InvocationErrorMessageOptional = std::optional<InvocationErrorMessageType>;
    InvocationErrorMessageOptional invocationErrorMessage;

    InvocationInfo(TransactionIdType transactionId, InvocationStateType invocationState);
  };

  struct AbstractSetResponse
  {
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    MdibVersionOptional mdibVersion;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType sequenceId;

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    InstanceIdOptional instanceId;

    using InvocationInfoType = ::BICEPS::MM::InvocationInfo;
    InvocationInfoType invocationInfo;

    AbstractSetResponse(SequenceIdType sequenceId, InvocationInfoType invocationInfo);
  };

  struct SetValueResponse : public AbstractSetResponse
  {
    using AbstractSetResponse::AbstractSetResponse;
  };

  struct OperationTarget : public std::string
  {
  };

  struct InvocationSource
  {
  };

  struct ReportPart
  {
    using OperationHandleRefType = ::BICEPS::MM::OperationHandleRef;
    OperationHandleRefType operationHandleRef;

    using OperationTargetType = ::BICEPS::MM::OperationTarget;
    using OperationTargetOptional = std::optional<OperationTargetType>;
    OperationTargetOptional operationTarget;

    using InvocationInfoType = ::BICEPS::MM::InvocationInfo;
    InvocationInfoType invocationInfo;

    using InvocationSourceType = ::BICEPS::MM::InvocationSource;
    InvocationSourceType invocationSource;

    ReportPart(OperationHandleRefType operationHandleRef, InvocationInfoType invocationInfo,
               InvocationSourceType invocationSource);
  };

  struct OperationInvokedReport : public AbstractReport
  {
    using ReportPartType = ::BICEPS::MM::ReportPart;
    ReportPartType reportPart;

    OperationInvokedReport(const SequenceIdType& sequenceId, ReportPartType reportPart);
  };

} // namespace BICEPS::MM
