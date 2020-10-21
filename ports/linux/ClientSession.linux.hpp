#pragma once

#include "ClientSession/SessionManager.hpp"
#include "Log.hpp"
#include "client_https.hpp"
#include <regex>

template <typename SocketType>
class ClientSessionSimple : public ClientSessionInterface
{
public:
  explicit ClientSessionSimple(const std::string& address);

  void send(const std::string& message) override;

private:
  SimpleWeb::Client<SocketType> client_;
};

template <>
inline ClientSessionSimple<SimpleWeb::HTTPS>::ClientSessionSimple(const std::string& address)
  : client_(std::regex_replace(address, std::regex("http(s)?://"), ""), false, "certs/server.crt",
            "certs/server.key", "certs/ca.crt")
{
}

template <>
inline ClientSessionSimple<SimpleWeb::HTTP>::ClientSessionSimple(const std::string& address)
  : client_(std::regex_replace(address, std::regex("http(s)?://"), ""))
{
}

template <typename SocketType>
void ClientSessionSimple<SocketType>::send(const std::string& message)
{
  client_.request("POST", "", message);
}
