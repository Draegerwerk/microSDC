#include "UUID.hpp"

UUID::UUID(uint32_t timeLow, uint32_t timeMid, uint32_t timeHiAndVersion, uint16_t clockSeq,
           std::array<uint8_t, 6> node)
  : timeLow_(timeLow)
  , timeMid_(timeMid)
  , timeHiAndVersion_(timeHiAndVersion)
  , clockSeq_(clockSeq)
  , node_(std::move(node))
{
}

std::string UUID::toString() const
{
  std::string result;
  result.reserve(30);
  appendHex(result, timeLow_);
  result += '-';
  appendHex(result, timeMid_);
  result += '-';
  appendHex(result, timeHiAndVersion_);
  result += '-';
  for (const auto node : node_)
  {
    appendHex(result, node);
  }
  return result;
}

void UUID::appendHex(std::string& str, uint8_t n)
{
  static constexpr const char* digits__ = "0123456789abcdef";
  str += digits__[(n >> 4u) & 0xFu];
  str += digits__[n & 0xFu];
}

void UUID::appendHex(std::string& str, uint16_t n)
{
  appendHex(str, static_cast<uint8_t>(n >> 8u));
  appendHex(str, static_cast<uint8_t>(n & 0xFFu));
}

void UUID::appendHex(std::string& str, uint32_t n)
{
  appendHex(str, static_cast<uint16_t>(n >> 16u));
  appendHex(str, static_cast<uint16_t>(n & 0xFFFFu));
}
