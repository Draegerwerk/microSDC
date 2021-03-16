#pragma once

#include "Log.hpp"
#include <regex>
#include <string>

class URL
{
public:
  URL() = default;

  explicit URL(const std::string& url)
    : url_(url)
  {
    parse(url);
  }

  const std::string& url() const
  {
    return url_;
  }
  const std::string& protocol() const
  {
    return protocol_;
  }
  const std::string& host() const
  {
    return host_;
  }
  const std::string& path() const
  {
    return path_;
  }

private:
  void parse(const std::string& url)
  {
    std::regex regex{"(?:(http|https)://)?([^/]+)(.*)"};
    std::smatch match;
    if (!std::regex_match(url, match, regex))
    {
      LOG(LogLevel::ERROR, "Cannot match url: " << url);
      throw std::runtime_error("Cannot match url");
    }
    if (match[1].matched)
    {
      protocol_ = match[1].str();
    }
    if (match[2].matched)
    {
      host_ = match[2].str();
    }
    if (match[3].matched)
    {
      path_ = match[3].str();
    }
  }

  std::string url_;
  std::string protocol_;
  std::string host_;
  std::string path_;
};
