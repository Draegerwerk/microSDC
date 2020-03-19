#pragma once

#include <map>
#include <memory>
#include <string>

class ClientSessionInterface
{
public:
  virtual void send(const std::string& message) const = 0;
};

class SessionManagerInterface
{
public:
  virtual void createSession(const std::string& notifyTo) = 0;
  virtual void sendToSession(const std::string& notifyTo, const std::string& message) = 0;
  virtual void deleteSession(const std::string& notifyTo) = 0;
};
