#include "MessagingContext.hpp"
#include <chrono>

void MessagingContext::resetInstanceId()
{
  std::lock_guard<std::mutex> lock(instanceIdMutex_);
  instanceId_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
}

uint64_t MessagingContext::getInstanceId() const
{
  std::lock_guard<std::mutex> lock(instanceIdMutex_);
  return instanceId_;
}

uint64_t MessagingContext::getCurrentMessageCounter() const
{
  std::lock_guard<std::mutex> lock(messageCounterMutex_);
  return messageCounter_;
}

uint64_t MessagingContext::getNextMessageCounter()
{
  std::lock_guard<std::mutex> lock(messageCounterMutex_);
  return ++messageCounter_;
}
