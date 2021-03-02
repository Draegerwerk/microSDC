#pragma once

#include <arpa/inet.h>
#include <array>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>

class NetworkInterface
{
public:
  enum class IPFamily
  {
    V4,
    V6,
    NONE,
  };

  explicit NetworkInterface(const ifaddrs* interface)
  {
    if (interface->ifa_addr->sa_family == AF_INET)
    {
      auto* tmpAddrPtr = &reinterpret_cast<sockaddr_in*>(interface->ifa_addr)->sin_addr;
      std::array<char, INET_ADDRSTRLEN> addressBuffer{};
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer.data(), INET_ADDRSTRLEN);
      address_ = std::string(addressBuffer.data());
    }
    else if (interface->ifa_addr->sa_family == AF_INET6)
    {
      auto* tmpAddrPtr = &reinterpret_cast<sockaddr_in6*>(interface->ifa_addr)->sin6_addr;
      std::array<char, INET6_ADDRSTRLEN> addressBuffer{};
      inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer.data(), INET6_ADDRSTRLEN);
      address_ = std::string(addressBuffer.data());
    }
  }

  static NetworkInterface findDefaultInterface()
  {
    return findInterface(IPFamily::NONE, "");
  }

  static NetworkInterface findInterface(const IPFamily family, const std::string& name)
  {
    ifaddrs* ifAddrStruct = nullptr;
    getifaddrs(&ifAddrStruct);

    for (auto* ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next)
    {
      // filter by type
      if ((ifa->ifa_flags & IFF_LOOPBACK) != 0 || (ifa->ifa_flags & IFF_UP) == 0 ||
          (ifa->ifa_flags & IFF_RUNNING) == 0 || ifa->ifa_addr == nullptr ||
          (ifa->ifa_addr->sa_family != AF_INET && ifa->ifa_addr->sa_family != AF_INET6))
      {
        continue;
      }
      // filter by ip family
      if (family == IPFamily::V4 && ifa->ifa_addr->sa_family != AF_INET ||
          family == IPFamily::V6 && ifa->ifa_addr->sa_family != AF_INET6)
      {
        continue;
      }
      // filter by name
      if (!name.empty() && name != ifa->ifa_name)
      {
        continue;
      }
      return NetworkInterface(ifa);
    }
    if (ifAddrStruct != nullptr)
    {
      freeifaddrs(ifAddrStruct);
    }
    throw std::runtime_error("No default interface found!");
  }

  const std::string& address() const
  {
    return address_;
  }

private:
  std::string address_;
};
