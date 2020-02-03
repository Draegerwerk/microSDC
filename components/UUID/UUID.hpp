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
   * @brief Construct UUID from separate parts
   */
  UUID(uint32_t timeLow, uint32_t timeMid, uint32_t timeHiAndVersion, uint16_t clockSeq,
       std::array<uint8_t, 6> node);

  /**
   * @brief convert string from UUID object
   * @return string representing the UUID
   */
  std::string toString() const;

private:
  uint32_t timeLow_;
  uint16_t timeMid_;
  uint16_t timeHiAndVersion_;
  uint16_t clockSeq_;
  std::array<uint8_t, 6> node_;

  static void appendHex(std::string& str, uint8_t n);
  static void appendHex(std::string& str, uint16_t n);
  static void appendHex(std::string& str, uint32_t n);
};
