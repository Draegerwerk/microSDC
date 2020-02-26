#pragma once

#include "MicroSDC.hpp"
#include "SoapService.hpp"
#include "dpws/MetadataProvider.hpp"
#include "SubscriptionManager.hpp"
#include <memory>

class SetService : public SoapService
{
public:
  SetService(const MicroSDC& microSDC, MetadataProvider metadata);
  std::string getURI() const override;
  void handleRequest(httpd_req* req, char* message) override;

private:
  const MicroSDC& microSDC_;
  const MetadataProvider metadata_;
  std::vector<std::shared_ptr<SubscriptionManager>> subscriptionManagers_;
};
