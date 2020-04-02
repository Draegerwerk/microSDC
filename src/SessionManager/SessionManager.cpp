#include "SessionManager.hpp"
#include "Log.hpp"

void SessionManager::createSession(const std::string& notifyTo)
{
  if (sessions_.count(notifyTo) != 0)
  {
    LOG(LogLevel::INFO, "Client session already exists");
    return;
  }
  sessions_.emplace(notifyTo, ClientSessionFactory::produce(notifyTo));
}

void SessionManager::sendToSession(const std::string& notifyTo, const std::string& message)
{
  auto sessionIt = sessions_.find(notifyTo);
  if (sessionIt == sessions_.end())
  {
    LOG(LogLevel::ERROR, "Cannot find client session with address " << notifyTo);
    return;
  }
  LOG(LogLevel::INFO, "Sending to " << notifyTo);
  sessionIt->second->send(message);
}

void SessionManager::deleteSession(const std::string& notifyTo)
{
  sessions_.erase(notifyTo);
}
