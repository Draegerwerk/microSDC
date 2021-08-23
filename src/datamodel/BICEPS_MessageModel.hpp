#pragma once

#include "BICEPS_ParticipantModel.hpp"
#include "ws-addressing.hpp"
#include <string>
#include <utility>

namespace BICEPS::MM
{
  enum class InvocationState
  {
    WAIT,
    START,
    CNCLLD,
    CNCLLD_MAN,
    FIN,
    FIN_MOD,
    FAIL
  };

  enum class InvocationError
  {
    UNSPEC,
    UNKN,
    INV,
    OTH
  };

  struct GetMdib
  {
  };

  struct AbstractGetResponse
  {
    PM::MdibVersionGroup mdib_version_group;

  protected:
    explicit AbstractGetResponse(PM::MdibVersionGroup mdib_version_group);
  };

  struct GetMdibResponse : public AbstractGetResponse
  {
    using MdibType = PM::Mdib;
    MdibType mdib;

    explicit GetMdibResponse(PM::MdibVersionGroup mdib_version_group, MdibType mdib);
  };

  struct AbstractReport
  {
    PM::MdibVersionGroup mdib_version_group;

  protected:
    explicit AbstractReport(PM::MdibVersionGroup mdib_version_group);
  };

  struct AbstractReportPart
  {
    using SourceMdsType = std::string;
    using SourceMdsOptional = boost::optional<SourceMdsType>;
    SourceMdsOptional source_mds;

  protected:
    AbstractReportPart() = default;
  };

  struct MetricReportPart : public AbstractReportPart
  {
    using MetricStateType = std::shared_ptr<const PM::AbstractMetricState>;
    using MetricStateSequence = std::vector<MetricStateType>;
    MetricStateSequence metric_state;
  };

  struct AbstractMetricReport : public AbstractReport
  {
    using ReportPartType = MetricReportPart;
    using ReportPartSequence = std::vector<ReportPartType>;
    ReportPartSequence report_part;

  protected:
    explicit AbstractMetricReport(const PM::MdibVersionGroup& mdib_version_group);
  };

  struct EpisodicMetricReport : public AbstractMetricReport
  {
    explicit EpisodicMetricReport(const PM::MdibVersionGroup& mdib_version_group);
  };

  using OperationHandleRef = PM::HandleRef;

  struct AbstractSet
  {
    enum class SetKind
    {
      SET_VALUE,
      SET_STRING
    };
    SetKind get_kind() const;

    using OperationHandleRefType = OperationHandleRef;
    OperationHandleRefType operation_handle_ref;


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
    RequestedNumericValueType requested_numeric_value{0.0};

    explicit SetValue(const rapidxml::xml_node<>& node);
    static bool classof(const AbstractSet* other);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct SetString : public AbstractSet
  {
    using RequestedStringValueType = std::string;
    RequestedStringValueType requested_string_value;

    explicit SetString(const rapidxml::xml_node<>& node);
    static bool classof(const AbstractSet* other);

  private:
    void parse(const rapidxml::xml_node<>& node);
  };

  struct InvocationErrorMessage : public std::string
  {
    explicit InvocationErrorMessage(std::string invocation_error_message);
  };

  struct InvocationInfo
  {
    using TransactionIdType = unsigned int;
    TransactionIdType transaction_id;

    using InvocationStateType = InvocationState;
    InvocationStateType invocation_state;

    using InvocationErrorType = InvocationError;
    using InvocationErrorOptional = boost::optional<InvocationErrorType>;
    InvocationErrorOptional invocation_error;

    using InvocationErrorMessageType = InvocationErrorMessage;
    using InvocationErrorMessageOptional = boost::optional<InvocationErrorMessageType>;
    InvocationErrorMessageOptional invocation_error_message;

    InvocationInfo(TransactionIdType transaction_id, InvocationStateType invocation_state);
  };

  struct AbstractSetResponse
  {
    PM::MdibVersionGroup mdib_version_group;

    using InvocationInfoType = InvocationInfo;
    InvocationInfoType invocation_info;

    AbstractSetResponse(PM::MdibVersionGroup mdib_version_group,
                        InvocationInfoType invocation_info);
  };

  struct SetValueResponse : public AbstractSetResponse
  {
    using AbstractSetResponse::AbstractSetResponse;
  };

  struct SetStringResponse : public AbstractSetResponse
  {
    using AbstractSetResponse::AbstractSetResponse;
  };

  struct OperationTarget : public std::string
  {
  };

  struct InvocationSource
  {
  };

  struct ComponentReportPart
  {
    using ComponentStateType = std::shared_ptr<const PM::AbstractDeviceComponentState>;
    using ComponentStateSequence = std::vector<ComponentStateType>;
    ComponentStateSequence component_state;
  };

  struct AbstractComponentReport : public AbstractReport
  {
    using ReportPartType = ComponentReportPart;
    using ReportPartSequence = std::vector<ReportPartType>;
    ReportPartSequence report_part;

    explicit AbstractComponentReport(const PM::MdibVersionGroup& mdib_version_group);
  };

  struct EpisodicComponentReport : public AbstractComponentReport
  {
    explicit EpisodicComponentReport(const PM::MdibVersionGroup& mdib_version_group);
  };

  struct OperationInvokedReportPart : public AbstractReportPart
  {
    using OperationHandleRefType = PM::HandleRef;
    OperationHandleRefType operation_handle_ref;

    using OperationTargetType = PM::HandleRef;
    using OperationTargetOptional = boost::optional<OperationTargetType>;
    OperationTargetOptional operation_target;

    using InvocationInfoType = InvocationInfo;
    InvocationInfoType invocation_info;

    using InvocationSourceType = PM::InstanceIdentifier;
    InvocationSourceType invocation_source;

    OperationInvokedReportPart(OperationHandleRefType operation_handle_ref,
                               InvocationInfoType invocation_info,
                               InvocationSourceType invocation_source);
  };

  struct OperationInvokedReport : public AbstractReport
  {
    using ReportPartType = OperationInvokedReportPart;
    ReportPartType report_part;

    OperationInvokedReport(const PM::MdibVersionGroup& mdib_version_group,
                           ReportPartType report_part);
  };

} // namespace BICEPS::MM
