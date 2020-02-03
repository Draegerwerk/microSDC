#pragma once

#include "Ethernet.hpp"
#include "NetworkInterface.hpp"
#include "WiFi.hpp"
#include <memory>

class NetworkHandler
{
public:
  NetworkHandler(NetworkHandler const&) = delete;
  void operator=(NetworkHandler const&) = delete;

  static NetworkHandler& getInstance();

  std::string address() const;

  esp_err_t connect();

private:
  NetworkHandler();
  std::unique_ptr<NetworkInterface> networkInterface_;
};
