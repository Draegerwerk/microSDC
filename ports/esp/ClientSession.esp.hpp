#pragma once

#include "SessionManager/SessionManager.hpp"

struct esp_http_client;

class ClientSessionEsp32 : public ClientSessionInterface
{
public:
  /// @brief Constructs a new ClientSession
  /// @param notifyTo the address to connect this session to
  /// @param useTls whether to use TLS encrypted communication
  explicit ClientSessionEsp32(std::string notifyTo, bool useTls);
  ClientSessionEsp32(const ClientSessionEsp32&) = delete;
  ClientSessionEsp32(ClientSessionEsp32&&) = delete;
  ClientSessionEsp32& operator=(const ClientSessionEsp32&) = delete;
  ClientSessionEsp32& operator=(ClientSessionEsp32&&) = delete;
  ~ClientSessionEsp32() override;
  void send(const std::string& message) override;

private:
  /// pointer to the esp http session instance
  esp_http_client* session_{};
  /// the address this session is connected to
  const std::string notifyTo_;
};
