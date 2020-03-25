#pragma once

#include <map>
#include <memory>
#include <string>

/// @brief ClientSessionInterface defines an interface to a client session
class ClientSessionInterface
{
public:
  virtual ~ClientSessionInterface() = default;
  /// @brief sends a given data message string the this client
  /// @param message the message to send
  virtual void send(const std::string& message) const = 0;
};

/// @brief SessionManagerInterface defines an interface to client sessions for eventing
/// notifications
class SessionManagerInterface
{
public:
  virtual ~SessionManagerInterface() = default;
  /// @brief creates a new session for a given client address
  /// @param notifyTo the address of the client
  virtual void createSession(const std::string& notifyTo) = 0;

  /// @brief sends a message string to a client session with given address
  /// @param notifyTo the address of the client
  /// @param message the message to send to the client
  virtual void sendToSession(const std::string& notifyTo, const std::string& message) = 0;

  /// @brief deletes the session of the client with given address
  /// @param notifyTo the address of the client
  virtual void deleteSession(const std::string& notifyTo) = 0;
};

class SessionManagerFactory
{
public:
  static std::unique_ptr<SessionManagerInterface> produce();
};
