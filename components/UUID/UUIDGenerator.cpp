#include "UUIDGenerator.hpp"
#include "esp_system.h"

UUID UUIDGenerator::create()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (!haveNode_)
  {
    esp_read_mac(node_.data(), ESP_MAC_ETH);
    haveNode_ = true;
  }
  const uint32_t timeLow = esp_random();
  const uint32_t randomMidHigh = esp_random();
  const auto timeMid = static_cast<uint16_t>(randomMidHigh & 0xFFFFu);
  const auto timeHiAndVersion =
      static_cast<uint16_t>((randomMidHigh >> 16u) & 0x0FFFu) + (1u << 12u);
  const auto clockSeq = static_cast<uint16_t>((esp_random() >> 4u) & 0x3FFFu) | 0x8000u;
  return UUID(timeLow, timeMid, timeHiAndVersion, clockSeq, node_);
}
