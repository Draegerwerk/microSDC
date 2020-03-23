#include "MessagingContext.hpp"
#include <chrono>

void MessagingContext::resetInstanceId()
{
  instanceId_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
}

uint64_t MessagingContext::getInstanceId() const
{
  return instanceId_;
}

uint64_t MessagingContext::getCurrentMessageCounter() const
{
  return messageCounter_;
}

uint64_t MessagingContext::getNextMessageCounter()
{
  return ++messageCounter_;
}
