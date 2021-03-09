#include "SessionManager.hpp"
#include "Log.hpp"

SessionManager::SessionManager(const bool use_tls)
  : use_tls_(use_tls)
{
}

void SessionManager::create_session(const std::string& notify_to)
{
  if (sessions_.count(notify_to) != 0)
  {
    LOG(LogLevel::INFO, "Client session already exists");
    return;
  }
  sessions_.emplace(notify_to, ClientSessionFactory::produce(notify_to, use_tls_));
}

void SessionManager::send_to_session(const std::string& notify_to, const std::string& message)
{
  const auto session_it = sessions_.find(notify_to);
  if (session_it == sessions_.end())
  {
    LOG(LogLevel::ERROR, "Cannot find client session with address " << notify_to);
    return;
  }
  LOG(LogLevel::INFO, "Sending to " << notify_to);
  session_it->second->send(message);
}

void SessionManager::delete_session(const std::string& notify_to)
{
  sessions_.erase(notify_to);
}
