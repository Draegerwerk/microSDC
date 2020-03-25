#pragma once

#include "SessionManager.hpp"

struct esp_http_client;

class ClientSessionEsp32 : public ClientSessionInterface
{
public:
  explicit ClientSessionEsp32(const std::string& notifyTo);
  ~ClientSessionEsp32() override;
  void send(const std::string& message) const override;

private:
  esp_http_client* session_{};
  const std::string notifyTo_;
};

class SessionManagerEsp32 : public SessionManagerInterface
{
public:
  void createSession(const std::string& notifyTo) override;
  void sendToSession(const std::string& notifyTo, const std::string& message) override;
  void deleteSession(const std::string& notifyTo) override;

private:
  std::map<std::string, std::shared_ptr<ClientSessionInterface>> sessions_;
};
