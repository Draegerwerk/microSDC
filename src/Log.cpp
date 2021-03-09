#include "Log.hpp"

void Log::set_log_level(const LogLevel level)
{
  log_level__ = level;
}

// default log level to INFO
LogLevel Log::log_level__{LogLevel::INFO};
