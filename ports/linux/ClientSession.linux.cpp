#include "ClientSession.linux.hpp"

std::unique_ptr<ClientSessionInterface> ClientSessionFactory::produce(const std::string& address,
                                                                      const bool use_tls)
{
  if (use_tls)
  {
    return std::make_unique<ClientSessionSimple<SimpleWeb::HTTPS>>(address);
  }
  return std::make_unique<ClientSessionSimple<SimpleWeb::HTTP>>(address);
}
