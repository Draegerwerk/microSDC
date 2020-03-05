#pragma once

#include "MicroSDC.hpp"
#include "SoapService.hpp"
#include "SubscriptionManager.hpp"
#include "dpws/MetadataProvider.hpp"
#include "datamodel/BICEPS_MessageModel.hpp"
#include <memory>

class SetService : public SoapService
{
public:
  SetService(const MicroSDC& microSDC, MetadataProvider metadata,
             std::shared_ptr<SubscriptionManager> subscriptionManager);
  std::string getURI() const override;
  void handleRequest(httpd_req* req, char* message) override;

private:
  const MicroSDC& microSDC_;
  const MetadataProvider metadata_;
  const std::shared_ptr<SubscriptionManager> subscriptionManager_;

  BICEPS::MM::SetValueResponse dispatch(const BICEPS::MM::SetValue& setValueRequest);
};
