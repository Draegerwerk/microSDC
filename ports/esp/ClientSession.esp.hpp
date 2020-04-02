#pragma once

#include "SessionManager/SessionManager.hpp"

struct esp_http_client;

class ClientSessionEsp32 : public ClientSessionInterface
{
public:
  explicit ClientSessionEsp32(std::string notifyTo);
  ~ClientSessionEsp32() override;
  void send(const std::string& message) override;

private:
  esp_http_client* session_{};
  const std::string notifyTo_;
};
