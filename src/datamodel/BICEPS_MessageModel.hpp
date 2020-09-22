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
    MdibType Mdib;

    explicit GetMdibResponse(MdibType mdib);
  };

  struct AbstractReport
  {
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    MdibVersionOptional MdibVersion;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType SequenceId;

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    InstanceIdOptional InstanceId;

  protected:
    explicit AbstractReport(WS::ADDRESSING::URIType sequenceId);
  };

  struct AbstractReportPart
  {
    using SourceMdsType = std::string;
    using SourceMdsOptional = std::optional<SourceMdsType>;
    SourceMdsOptional SourceMds;

  protected:
    AbstractReportPart() = default;
  };

  struct MetricReportPart : public AbstractReportPart
  {
    using MetricStateType = std::shared_ptr<const PM::AbstractMetricState>;
    using MetricStateSequence = std::vector<MetricStateType>;
    MetricStateSequence MetricState;
  };

  struct AbstractMetricReport : public AbstractReport
  {
    using ReportPartType = MetricReportPart;
    using ReportPartSequence = std::vector<ReportPartType>;
    ReportPartSequence ReportPart;

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
    using OperationHandleRefType = ::BICEPS::MM::OperationHandleRef;
    OperationHandleRefType OperationHandleRef;
  };

  struct SetValue : public AbstractSet
  {
    using RequestedNumericValueType = double;
    RequestedNumericValueType RequestedNumericValue{0.0};

    explicit SetValue(const rapidxml::xml_node<>& node);

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
    TransactionIdType TransactionId;

    using InvocationStateType = ::BICEPS::MM::InvocationState;
    InvocationStateType InvocationState;

    using InvocationErrorType = ::BICEPS::MM::InvocationError;
    using InvocationErrorOptional = std::optional<InvocationErrorType>;
    InvocationErrorOptional InvocationError;

    using InvocationErrorMessageType = ::BICEPS::MM::InvocationErrorMessage;
    using InvocationErrorMessageOptional = std::optional<InvocationErrorMessageType>;
    InvocationErrorMessageOptional InvocationErrorMessage;

    InvocationInfo(TransactionIdType transactionId, InvocationStateType invocationState);
  };

  struct AbstractSetResponse
  {
    using MdibVersionType = unsigned int;
    using MdibVersionOptional = std::optional<MdibVersionType>;
    MdibVersionOptional MdibVersion;

    using SequenceIdType = WS::ADDRESSING::URIType;
    SequenceIdType SequenceId;

    using InstanceIdType = unsigned int;
    using InstanceIdOptional = std::optional<InstanceIdType>;
    InstanceIdOptional InstanceId;

    using InvocationInfoType = ::BICEPS::MM::InvocationInfo;
    InvocationInfoType InvocationInfo;

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
    OperationHandleRefType OperationHandleRef;

    using OperationTargetType = ::BICEPS::MM::OperationTarget;
    using OperationTargetOptional = std::optional<OperationTargetType>;
    OperationTargetOptional OperationTarget;

    using InvocationInfoType = ::BICEPS::MM::InvocationInfo;
    InvocationInfoType InvocationInfo;

    using InvocationSourceType = ::BICEPS::MM::InvocationSource;
    InvocationSourceType InvocationSource;

    ReportPart(OperationHandleRefType operationHandleRef, InvocationInfoType invocationInfo,
               InvocationSourceType invocationSource);
  };

  struct OperationInvokedReport : public AbstractReport
  {
    using ReportPartType = ::BICEPS::MM::ReportPart;
    ReportPartType ReportPart;

    OperationInvokedReport(const SequenceIdType& sequenceId, ReportPartType reportPart);
  };

} // namespace BICEPS::MM
