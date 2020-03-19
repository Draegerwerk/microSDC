#include "Log.hpp"

void Log::setLogLevel(const LogLevel level)
{
  logLevel__ = level;
}

// default log level to INFO
LogLevel Log::logLevel__{LogLevel::INFO};
