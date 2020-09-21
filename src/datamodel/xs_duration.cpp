#include "xs_duration.hpp"
#include <regex>

Duration::Duration(const std::string& string)
{
  parse(string);
}

Duration::Duration(Years years, Months months, Days days, Hours hours, Minutes minutes,
                   Seconds seconds, bool isNegative)
  : isNegative_(isNegative)
  , years_(years)
  , months_(months)
  , days_(days)
  , hours_(hours)
  , minutes_(minutes)
  , seconds_(seconds)
{
}

Duration::Years::rep Duration::years() const
{
  return years_.count();
}

Duration::Months::rep Duration::months() const
{
  return months_.count();
}

Duration::Days::rep Duration::days() const
{
  return days_.count();
}

Duration::Hours::rep Duration::hours() const
{
  return hours_.count();
}

Duration::Minutes::rep Duration::minutes() const
{
  return minutes_.count();
}

Duration::Seconds::rep Duration::seconds() const
{
  return seconds_.count();
}

Duration::TimePoint Duration::toExpirationTimePoint() const
{
  const auto duration = years_ + months_ + days_ + hours_ + minutes_ +
                        std::chrono::duration_cast<std::chrono::steady_clock::duration>(seconds_);
  return std::chrono::steady_clock::now() + duration * (isNegative_ ? -1 : 1);
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
    years_ = Years{std::stoi(match[2].str())};
  }
  if (match[3].matched)
  {
    months_ = Months{std::stoi(match[3].str())};
  }
  if (match[4].matched)
  {
    days_ = Days{std::stoi(match[4].str())};
  }
  if (match[5].matched)
  {
    hours_ = std::chrono::hours{std::stoi(match[5].str())};
  }
  if (match[6].matched)
  {
    minutes_ = std::chrono::minutes{std::stoi(match[6].str())};
  }
  if (match[7].matched)
  {
    seconds_ =
        std::chrono::duration<float, std::chrono::seconds::period>{std::stof(match[7].str())};
  }
}
