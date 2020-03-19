#pragma once

#include "UUID.hpp"
#include <memory>
#include <mutex>
#include <random>

class UUIDGenerator
{
public:
  UUIDGenerator() = default;
  /**
   * @brief create a new UUID
   * @return the created UUID
   */
  UUID operator()();
};
