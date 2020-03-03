#include "BICEPS_MessageModel.hpp"

namespace BICEPS::MM
{
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
} // namespace BICEPS::MM
