#pragma once

#include <iostream>

/// @brief LogLevel describes the level of severity of a log message
enum class LogLevel
{
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  NONE
};

struct None
{
};

template <typename List>
struct LogData
{
  List list;
};

template <typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(LogData<Begin>&& begin,
                                                          Value&& value) noexcept
{
  return {{std::forward<Begin>(begin.list), std::forward<Value>(value)}};
}

template <typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(LogData<Begin>&& begin,
                                                              const char (&value)[n]) noexcept
{
  return {{std::forward<Begin>(begin.list), value}};
}

using PfnManipulator = std::ostream& (*)(std::ostream&);

template <typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(LogData<Begin>&& begin,
                                                                 PfnManipulator value) noexcept
{
  return {{std::forward<Begin>(begin.list), value}};
}

class Log
{
private:
  template <typename Begin, typename Last>
  static void output(std::ostream& os, std::pair<Begin, Last>&& data)
  {
    output(os, std::move(data.first));
    os << data.second;
  }

  static inline void output(std::ostream& os, None /*unused*/) {}

  static LogLevel logLevel__;

public:

  static void setLogLevel(LogLevel level);

  template <LogLevel level, typename List>
  static void log(const char* file, int line, LogData<List>&& data)
  {
    if (level < logLevel__)
    {
      return;
    }
    std::cout << "\x1B[";
    if constexpr (level == LogLevel::ERROR)
    {
      std::cout << "31m";
    }
 else if constexpr    (level == LogLevel::WARNING)
    {
      std::cout << "33m";
    }
 else if constexpr    (level == LogLevel::INFO)
    {
      std::cout << "32m";
    }
    else
    {
      std::cout << "37m";
    }
    std::cout << file << ":" << line << ": ";
    output(std::cout, std::move(data.list));
    std::cout << "\033[0m";
    std::cout << std::endl;
  }
};

#define LOG(level, msg) (Log::log<level>(__FILE__, __LINE__, LogData<None>() << msg))
