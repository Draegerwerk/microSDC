#pragma once

#include "SoapService.hpp"
#include "SubscriptionManager.hpp"
#include "dpws/MetadataProvider.hpp"
#include "datamodel/BICEPS_MessageModel.hpp"
#include <memory>

class MicroSDC;

class SetService : public SoapService
{
public:
  SetService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata,
             std::shared_ptr<SubscriptionManager> subscriptionManager);
  std::string getURI() const override;
  void handleRequest(std::shared_ptr<Request> req) override;

private:
  const MicroSDC& microSDC_;
  const std::shared_ptr<const MetadataProvider> metadata_;
  const std::shared_ptr<SubscriptionManager> subscriptionManager_;

  BICEPS::MM::SetValueResponse dispatch(const BICEPS::MM::SetValue& setValueRequest);
};
