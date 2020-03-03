#pragma once

#include "BICEPS_ParticipantModel.hpp"
#include <string>
#include <utility>

namespace BICEPS::MM
{
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
} // namespace BICEPS::MM
