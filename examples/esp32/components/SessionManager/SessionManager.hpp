#pragma once

#include "SessionManagerInterface.hpp"

struct esp_http_client;

class ClientSession : public ClientSessionInterface
{
public:
  explicit ClientSession(const std::string& notifyTo);
  ~ClientSession();
  void send(const std::string& message) const override;

private:
  esp_http_client* session_{};
  const std::string notifyTo_;
};

class SessionManager : public SessionManagerInterface
{
public:
  void createSession(const std::string& notifyTo) override;
  void sendToSession(const std::string& notifyTo, const std::string& message) override;
  void deleteSession(const std::string& notifyTo) override;

private:
  std::map<std::string, std::shared_ptr<ClientSessionInterface>> sessions_;
};
