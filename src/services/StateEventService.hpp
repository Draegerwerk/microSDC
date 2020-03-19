#pragma once

#include "SoapService.hpp"
#include "SubscriptionManager.hpp"
#include "dpws/MetadataProvider.hpp"

class MicroSDC;

class StateEventService : public SoapService
{
public:
  StateEventService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata,
                    std::shared_ptr<SubscriptionManager> subscriptionManager);
  std::string getURI() const override;
  void handleRequest(std::shared_ptr<Request> req) override;

private:
  const MicroSDC& microSDC_;
  const std::shared_ptr<const MetadataProvider> metadata_;
  const std::shared_ptr<SubscriptionManager> subscriptionManager_;
};
