#include "ClientSession.linux.hpp"
#include <regex>

std::unique_ptr<ClientSessionInterface> ClientSessionFactory::produce(const std::string& address)
{
  return std::make_unique<ClientSessionSimple>(address);
}

ClientSessionSimple::ClientSessionSimple(const std::string& address)
  : client_(std::regex_replace(address, std::regex("https://"), ""), false, "certs/server.crt",
            "certs/server.key", "certs/ca.crt")
{
}

void ClientSessionSimple::send(const std::string& message)
{
  client_.request("POST", "", message);
}
