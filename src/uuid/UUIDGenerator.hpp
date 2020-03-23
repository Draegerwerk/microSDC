#pragma once

#include "UUID.hpp"
#include <memory>
#include <mutex>
#include <random>

/// @brief UUIDGenerator generates version 4 UUIDs using a pseudo-random number generator engine
class UUIDGenerator
{
public:
  UUIDGenerator() = default;

  /// @brief create a new UUID
  /// @return the created UUID
  UUID operator()();
};
