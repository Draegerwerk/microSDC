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

/// @brief Log models a simple logger; logging to an std::ostream
class Log
{
private:
  /// @brief writes an LogData to the output
  /// @param os the output stream to write to
  /// @param data the templated list of data to log
  template <typename Begin, typename Last>
  static void output(std::ostream& os, std::pair<Begin, Last>&& data)
  {
    output(os, std::move(data.first));
    os << data.second;
  }

  /// @brief list termination for the None LogData
  static inline void output(std::ostream& os, None /*unused*/) {}

  /// the lowest log level this logger is writing to the output
  static LogLevel logLevel__;

public:
  /// @brief sets the lowest log level this logger is writing to its output
  static void setLogLevel(LogLevel level);

  /// @brief logs data to the output
  /// @param file the filename the log command was issued
  /// @param line the line of the file the log statement was issued
  /// @param data the data to log to the output
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
    else if constexpr (level == LogLevel::WARNING)
    {
      std::cout << "33m";
    }
    else if constexpr (level == LogLevel::INFO)
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
