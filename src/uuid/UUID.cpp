#include "UUID.hpp"
#include <iomanip>
#include <ios>

std::string UUID::toString() const
{
  std::stringstream s;
  s << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(data_[0]) << std::setw(2)
    << static_cast<int>(data_[1]) << std::setw(2) << static_cast<int>(data_[2]) << std::setw(2)
    << static_cast<int>(data_[3]);

  s << '-' << std::setw(2) << static_cast<int>(data_[4]) << std::setw(2)
    << static_cast<int>(data_[5]);

  s << '-' << std::setw(2) << static_cast<int>(data_[6]) << std::setw(2)
    << static_cast<int>(data_[7]);

  s << '-' << std::setw(2) << static_cast<int>(data_[8]) << std::setw(2)
    << static_cast<int>(data_[9]);

  s << '-' << std::setw(2) << static_cast<int>(data_[10]) << std::setw(2)
    << static_cast<int>(data_[11]) << std::setw(2) << static_cast<int>(data_[12]) << std::setw(2)
    << static_cast<int>(data_[13]) << std::setw(2) << static_cast<int>(data_[14]) << std::setw(2)
    << static_cast<int>(data_[15]);
  return s.str();
}

