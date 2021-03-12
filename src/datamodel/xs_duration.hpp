#pragma once

#include <chrono>
#include <string>

class Duration
{
public:
  static constexpr std::intmax_t seconds_in_a_year__ = 31556952;
  using Years = std::chrono::duration<std::int64_t, std::ratio<seconds_in_a_year__>>;
  static constexpr std::intmax_t seconds_in_a_month__ = 2629746;
  using Months = std::chrono::duration<std::int64_t, std::ratio<seconds_in_a_month__>>;
  static constexpr std::intmax_t seconds_in_a_day__ = 86400;
  using Days = std::chrono::duration<std::int64_t, std::ratio<seconds_in_a_day__>>;
  using Hours = std::chrono::hours;
  using Minutes = std::chrono::minutes;
  using Seconds = std::chrono::duration<float, std::chrono::seconds::period>;
  using TimePoint = std::chrono::steady_clock::time_point;

  explicit Duration(const std::string& string);
  Duration(Years years, Months months, Days days, Hours hours, Minutes minutes, Seconds seconds,
           bool is_negative);
  Duration(const Duration&) = default;
  Duration(Duration&&) = default;
  Duration& operator=(const Duration&) = default;
  Duration& operator=(Duration&&) = default;
  ~Duration() = default;

  TimePoint to_expiration_time_point() const;

  Years::rep years() const;
  Months::rep months() const;
  Days::rep days() const;
  Hours::rep hours() const;
  Minutes::rep minutes() const;
  Seconds::rep seconds() const;

private:
  void parse(const std::string& string);

  bool is_negative_{};
  Years years_{};
  Months months_{};
  Days days_{};
  Hours hours_{};
  Minutes minutes_{};
  Seconds seconds_{};
};
