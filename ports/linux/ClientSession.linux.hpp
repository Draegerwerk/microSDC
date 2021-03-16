#pragma once

#include "ClientSession/SessionManager.hpp"
#include "Log.hpp"
#include "client_https.hpp"
#include <regex>

template <typename SocketType>
class ClientSessionSimple : public ClientSessionInterface
{
public:
  explicit ClientSessionSimple(const URL& url);

  void send(const URL& url, const std::string& message) override;

private:
  SimpleWeb::Client<SocketType> client_;
};

template <>
inline ClientSessionSimple<SimpleWeb::HTTPS>::ClientSessionSimple(const URL& url)
  : client_(url.host(), false, "certs/server.crt", "certs/server.key", "certs/ca.crt")
{
}

template <>
inline ClientSessionSimple<SimpleWeb::HTTP>::ClientSessionSimple(const URL& url)
  : client_(url.host())
{
}

template <typename SocketType>
void ClientSessionSimple<SocketType>::send(const URL& url, const std::string& message)
{
  client_.request("POST", url.path(), message);
}
