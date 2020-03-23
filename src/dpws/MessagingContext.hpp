#pragma once

#include <mutex>

// TODO: add doxygen
class MessagingContext
{
public:
  MessagingContext() = default;

  void resetInstanceId();
  uint64_t getInstanceId() const;
  uint64_t getCurrentMessageCounter() const;
  uint64_t getNextMessageCounter();

private:
  mutable std::mutex instanceIdMutex_;
  mutable std::mutex messageCounterMutex_;

  uint64_t instanceId_{0};
  uint64_t messageCounter_{0};
};
