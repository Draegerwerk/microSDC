#pragma once

#include <memory>

/// @brief ClientSessionInterface defines an interface to a client session
class ClientSessionInterface
{
public:
  ClientSessionInterface() = default;
  ClientSessionInterface(const ClientSessionInterface&) = default;
  ClientSessionInterface(ClientSessionInterface&&) = default;
  ClientSessionInterface& operator=(const ClientSessionInterface&) = default;
  ClientSessionInterface& operator=(ClientSessionInterface&&) = default;
  virtual ~ClientSessionInterface() = default;
  /// @brief sends a given data message string the this client
  /// @param message the message to send
  virtual void send(const std::string& message) = 0;
};


class ClientSessionFactory
{
public:
  static std::unique_ptr<ClientSessionInterface> produce(const std::string& address, bool useTls);
};
