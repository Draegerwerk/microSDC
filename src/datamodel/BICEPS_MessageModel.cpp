#include "BICEPS_MessageModel.hpp"
#include "MDPWSConstants.hpp"
#include "SDCConstants.hpp"
#include <cstring>
#include <utility>

namespace BICEPS::MM
{
  AbstractGetResponse::AbstractGetResponse(PM::MdibVersionGroup mdib_version_group)
    : mdib_version_group(std::move(mdib_version_group))
  {
  }

  GetMdibResponse::GetMdibResponse(PM::MdibVersionGroup mdib_version_group, MdibType mdib)
    : AbstractGetResponse(std::move(mdib_version_group))
    , mdib(std::move(mdib))
  {
  }

  AbstractReport::AbstractReport(PM::MdibVersionGroup mdib_version_group)
    : mdib_version_group(std::move(mdib_version_group))
  {
  }

  AbstractMetricReport::AbstractMetricReport(const PM::MdibVersionGroup& mdib_version_group)
    : AbstractReport(mdib_version_group)
  {
  }

  EpisodicMetricReport::EpisodicMetricReport(const PM::MdibVersionGroup& mdib_version_group)
    : AbstractMetricReport(mdib_version_group)
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
        requested_string_value = std::string(entry->value(), entry->value_size());
      }
    }
  }

  InvocationErrorMessage::InvocationErrorMessage(std::string invocation_error)
    : std::string(std::move(invocation_error))
  {
  }

  InvocationInfo::InvocationInfo(TransactionIdType transaction_id,
                                 InvocationStateType invocation_state)
    : transaction_id(transaction_id)
    , invocation_state(invocation_state)
  {
  }

  AbstractSetResponse::AbstractSetResponse(PM::MdibVersionGroup mdib_version_group,
                                           InvocationInfoType invocation_info)
    : mdib_version_group(std::move(mdib_version_group))
    , invocation_info(std::move(invocation_info))
  {
  }

  AbstractComponentReport::AbstractComponentReport(const PM::MdibVersionGroup& mdib_version_group)
    : AbstractReport(mdib_version_group)
  {
  }

  EpisodicComponentReport::EpisodicComponentReport(const PM::MdibVersionGroup& mdib_version_group)
    : AbstractComponentReport(mdib_version_group)
  {
  }

  OperationInvokedReportPart::OperationInvokedReportPart(
      OperationHandleRefType operation_handle_ref, InvocationInfoType invocation_info,
      InvocationSourceType invocation_source)
    : operation_handle_ref(std::move(operation_handle_ref))
    , invocation_info(std::move(invocation_info))
    , invocation_source(std::move(invocation_source))
  {
  }

  OperationInvokedReport::OperationInvokedReport(const PM::MdibVersionGroup& mdib_version_group,
                                                 ReportPartType report_part)
    : AbstractReport(mdib_version_group)
    , report_part(std::move(report_part))
  {
  }
} // namespace BICEPS::MM