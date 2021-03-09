#include "MessagingContext.hpp"
#include <chrono>

void MessagingContext::reset_instance_id()
{
  std::lock_guard<std::mutex> lock(instance_id_mutex_);
  instance_id_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
}

uint64_t MessagingContext::get_instance_id() const
{
  std::lock_guard<std::mutex> lock(instance_id_mutex_);
  return instance_id_;
}

uint64_t MessagingContext::get_current_message_counter() const
{
  std::lock_guard<std::mutex> lock(message_counter_mutex_);
  return message_counter_;
}

uint64_t MessagingContext::get_next_message_counter()
{
  std::lock_guard<std::mutex> lock(message_counter_mutex_);
  return ++message_counter_;
}
