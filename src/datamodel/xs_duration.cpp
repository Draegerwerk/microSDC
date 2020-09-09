#include "xs_duration.hpp"
#include <regex>

Duration::Duration(const std::string& string)
{
  parse(string);
}

Duration::Duration(const bool isNegative, const unsigned int years, const unsigned int months,
                   const unsigned int days, const unsigned int hours, const unsigned int minutes,
                   const double seconds)
  : isNegative_(isNegative)
  , years_(years)
  , months_(months)
  , days_(days)
  , hours_(hours)
  , minutes_(minutes)
  , seconds_(seconds)
{
}

bool Duration::isNegative() const
{
  return isNegative_;
}
unsigned int Duration::years() const
{
  return years_;
}
unsigned int Duration::months() const
{
  return months_;
}
unsigned int Duration::days() const
{
  return days_;
}
unsigned int Duration::hours() const
{
  return hours_;
}
unsigned int Duration::minutes() const
{
  return minutes_;
}
double Duration::seconds() const
{
  return seconds_;
}

std::string Duration::str() const
{
  std::ostringstream res;
  res << "P" << years() << "Y" << months() << "M" << days() << "DT" << hours() << "H" << minutes()
      << "M" << seconds() << "S";
  return res.str();
}

std::chrono::seconds Duration::toDurationSeconds() const
{
  // TODO rewrite using std::chrono
  const auto now = std::time(nullptr);
  auto localTime = *std::localtime(&now);
  localTime.tm_year += years();
  localTime.tm_mon += months();
  localTime.tm_mday += days();
  localTime.tm_hour += hours();
  localTime.tm_min += minutes();
  localTime.tm_sec += seconds();
  const auto durationTime = std::mktime(&localTime);
  return std::chrono::seconds(static_cast<long>(std::difftime(durationTime, now)));
}

std::chrono::system_clock::time_point Duration::toExpirationTimePoint() const
{
  const auto durationSeconds = toDurationSeconds();
  return std::chrono::system_clock::now() + durationSeconds;
}

void Duration::parse(const std::string& string)
{

  const std::regex durationRegex("(-)?P(?:([0-9]+)Y)?(?:([0-9]+)M)?(?:([0-9]+)D)?(?:T(?:([0-9]+)H)?"
                                 "(?:([0-9]+)M)?(?:((?:[0-9]*[.])?[0-9]+)S)?)");
  std::smatch match;
  std::regex_match(string, match, durationRegex);
  isNegative_ = match[1].matched;
  if (match[2].matched)
  {
    years_ = std::atoi(match[2].str().c_str());
  }
  if (match[3].matched)
  {
    months_ = std::atoi(match[3].str().c_str());
  }
  if (match[4].matched)
  {
    days_ = std::atoi(match[4].str().c_str());
  }
  if (match[5].matched)
  {
    hours_ = std::atoi(match[5].str().c_str());
  }
  if (match[6].matched)
  {
    minutes_ = std::atoi(match[6].str().c_str());
  }
  if (match[7].matched)
  {
    seconds_ = std::atof(match[7].str().c_str());
  }
}
