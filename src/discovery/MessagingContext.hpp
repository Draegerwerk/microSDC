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
  void reset_instance_id();

  /// @brief retrieves the instance id
  /// @return the instance id of this context
  uint64_t get_instance_id() const;
  /// @brief retrieves the message counter for ordering messages in a single sequence
  /// @return the message id of this sequence
  uint64_t get_current_message_counter() const;
  /// @brief increments the mesage counter by 1
  /// @return the new incremented counter
  uint64_t get_next_message_counter();

private:
  mutable std::mutex instance_id_mutex_;
  mutable std::mutex message_counter_mutex_;

  /// the instance id of this sequence and context
  uint64_t instance_id_{0};
  /// the message counter representing the number of messages sent in this sequence
  uint64_t message_counter_{0};
};
