#pragma once

#include <array>
#include <string>

class UUID
{
public:
  /**
   * @brief default construct an empty UUID
   */
  UUID() = default;
  /**
   * @brief Construct UUID from data
   */
  constexpr explicit UUID(std::array<std::uint8_t, 16> data)
    : data_(data)
  {
  }

  /**
   * @brief convert string from UUID object
   * @return string representing the UUID
   */
  std::string toString() const;

private:
  std::array<std::uint8_t, 16> data_{};
};
