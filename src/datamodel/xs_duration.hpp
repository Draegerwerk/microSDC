#pragma once

#include <chrono>
#include <string>

class Duration
{
public:
  static constexpr std::intmax_t secondsInAYear__ = 31556952;
  using Years = std::chrono::duration<std::int64_t, std::ratio<secondsInAYear__>>;
  static constexpr std::intmax_t secondsInAMonth__ = 2629746;
  using Months = std::chrono::duration<std::int64_t, std::ratio<secondsInAMonth__>>;
  static constexpr std::intmax_t secondsInADay__ = 86400;
  using Days = std::chrono::duration<std::int64_t, std::ratio<secondsInADay__>>;
  using Hours = std::chrono::hours;
  using Minutes = std::chrono::minutes;
  using Seconds = std::chrono::duration<float, std::chrono::seconds::period>;
  using TimePoint = std::chrono::steady_clock::time_point;

  explicit Duration(const std::string& string);
  Duration(Years years, Months months, Days days, Hours hours, Minutes minutes, Seconds seconds,
           bool isNegative);
  Duration(const Duration&) = default;
  Duration(Duration&&) = default;
  Duration& operator=(const Duration&) = default;
  Duration& operator=(Duration&&) = default;
  ~Duration() = default;

  TimePoint toExpirationTimePoint() const;

  Years::rep years() const;
  Months::rep months() const;
  Days::rep days() const;
  Hours::rep hours() const;
  Minutes::rep minutes() const;
  Seconds::rep seconds() const;

private:
  void parse(const std::string& string);

  bool isNegative_{};
  Years years_{};
  Months months_{};
  Days days_{};
  Hours hours_{};
  Minutes minutes_{};
  Seconds seconds_{};
};
