#pragma once

#include "ClientSession.hpp"
#include <map>
#include <memory>
#include <string>

/// @brief SessionManager defines an interface to client sessions for eventing
/// notifications
class SessionManager
{
public:
  /// @brief constructs a new SessionManager
  /// @param useTls whether to use TLS for communication
  explicit SessionManager(bool use_tls);
  /// @brief creates a new session for a given client address
  /// @param notifyTo the address of the client
  void create_session(const std::string& notify_to);

  /// @brief sends a message string to a client session with given address
  /// @param notifyTo the address of the client
  /// @param message the message to send to the client
  void send_to_session(const std::string& notify_to, const std::string& message);

  /// @brief deletes the session of the client with given address
  /// @param notifyTo the address of the client
  void delete_session(const std::string& notify_to);

private:
  /// whether to use TLS encrypted communication
  const bool use_tls_;
  /// the map of all sessions this manager manages, address->session
  std::map<std::string, std::shared_ptr<ClientSessionInterface>> sessions_;
};
