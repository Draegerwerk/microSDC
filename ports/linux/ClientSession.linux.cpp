#include "ClientSession.linux.hpp"

std::unique_ptr<ClientSessionInterface> ClientSessionFactory::produce(const std::string& address,
                                                                      const bool useTls)
{
  if (useTls)
  {
    return std::make_unique<ClientSessionSimple<SimpleWeb::HTTPS>>(address);
  }
  return std::make_unique<ClientSessionSimple<SimpleWeb::HTTP>>(address);
}
