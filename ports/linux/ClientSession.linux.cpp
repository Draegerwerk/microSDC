#include "ClientSession.linux.hpp"

std::unique_ptr<ClientSessionInterface> ClientSessionFactory::produce(const URL& url,
                                                                      const bool use_tls)
{
  if (use_tls)
  {
    return std::make_unique<ClientSessionSimple<SimpleWeb::HTTPS>>(url);
  }
  return std::make_unique<ClientSessionSimple<SimpleWeb::HTTP>>(url);
}
