#include "SessionManager.hpp"
#include "Log.hpp"
#include "url.hpp"

SessionManager::SessionManager(const bool use_tls)
  : use_tls_(use_tls)
{
}

void SessionManager::create_session(const std::string& notify_to)
{
  const URL url{notify_to};
  if (sessions_.count(url.host()) != 0)
  {
    LOG(LogLevel::INFO, "Client session already exists");
    return;
  }
  sessions_.emplace(url.host(), ClientSessionFactory::produce(url, use_tls_));
}

void SessionManager::send_to_session(const std::string& notify_to, const std::string& message)
{
  const URL url{notify_to};
  const auto session_it = sessions_.find(url.host());
  if (session_it == sessions_.end())
  {
    LOG(LogLevel::ERROR, "Cannot find client session with address " << url.host());
    return;
  }
  LOG(LogLevel::INFO, "Sending to " << notify_to);
  session_it->second->send(url, message);
}

void SessionManager::delete_session(const std::string& notify_to)
{
  sessions_.erase(notify_to);
}
