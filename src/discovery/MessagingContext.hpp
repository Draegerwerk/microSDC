#pragma once

#include <mutex>

/// @brief MessagingContext holds information about the general context of messages send by a single
/// instance. This information is used for AppSequence information and ordering of messages in terms
/// of ws-discovery
class MessagingContext
{
public:
  /// @brief constructs a new defaulted MessagingContext
  MessagingContext() = default;

  /// @brief resets the instance id of this context to 0
  void resetInstanceId();

  /// @brief retrieves the instance id
  /// @return the instance id of this context
  uint64_t getInstanceId() const;
  /// @brief retrieves the message counter for ordering messages in a single sequence
  /// @return the message id of this sequence
  uint64_t getCurrentMessageCounter() const;
  /// @brief increments the mesage counter by 1
  /// @return the new incremented counter
  uint64_t getNextMessageCounter();

private:
  mutable std::mutex instanceIdMutex_;
  mutable std::mutex messageCounterMutex_;

  /// the instance id of this sequence and context
  uint64_t instanceId_{0};
  /// the message counter representing the number of messages sent in this sequence
  uint64_t messageCounter_{0};
};
