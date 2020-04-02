#pragma once

#include "Log.hpp"
#include "SessionManager/SessionManager.hpp"
#include "client_https.hpp"

class ClientSessionSimple : public ClientSessionInterface
{
public:
  explicit ClientSessionSimple(const std::string& address);

  void send(const std::string& message) override;

private:
  SimpleWeb::Client<SimpleWeb::HTTPS> client_;
};
