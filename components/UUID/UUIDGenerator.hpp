#pragma once

#include "UUID.hpp"
#include <mutex>

class UUIDGenerator
{
public:
  UUIDGenerator() = default;
  /**
   * @brief create a new UUID
   * @return the created UUID
   */
  UUID create();

private:
  /// whether the node part was already retreived
  bool haveNode_{false};
  mutable std::mutex mutex_;
  /// node part of UUID containing the mac address
  std::array<uint8_t, 6> node_;
};
