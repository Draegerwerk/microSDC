#pragma once

#include <map>
#include <memory>
#include <string>

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

/// @brief SessionManager defines an interface to client sessions for eventing
/// notifications
class SessionManager
{
public:
  /// @brief constructs a new SessionManager
  /// @param useTls whether to use TLS for communication
  explicit SessionManager(bool useTls);
  /// @brief creates a new session for a given client address
  /// @param notifyTo the address of the client
  void createSession(const std::string& notifyTo);

  /// @brief sends a message string to a client session with given address
  /// @param notifyTo the address of the client
  /// @param message the message to send to the client
  void sendToSession(const std::string& notifyTo, const std::string& message);

  /// @brief deletes the session of the client with given address
  /// @param notifyTo the address of the client
  void deleteSession(const std::string& notifyTo);

private:
  /// whether to use TLS encrypted communication
  const bool useTls_;
  /// the map of all sessions this manager manages, address->session
  std::map<std::string, std::shared_ptr<ClientSessionInterface>> sessions_;
};

class ClientSessionFactory
{
public:
  static std::unique_ptr<ClientSessionInterface> produce(const std::string& address, bool useTls);
};
