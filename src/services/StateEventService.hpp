#pragma once

#include "SoapService.hpp"
#include "SubscriptionManager.hpp"
#include "dpws/MetadataProvider.hpp"

class MicroSDC;

/// @brief StateEventService implements the SDC StateEventService
class StateEventService : public SoapService
{
public:
  /// @brief constructs a new StateEventService from given metadata
  /// @param microSDC a reference to the MicroSDC instance holding this service
  /// @param metadata a pointer to the metadata describing configurational data
  /// @param subscriptionManager a pointer to the SubscriptionManager implementation
  StateEventService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata,
                    std::shared_ptr<SubscriptionManager> subscriptionManager);

  std::string getURI() const override;
  void handleRequest(std::shared_ptr<Request> req) override;

private:
  /// a reference to the microSDC instance holding this service
  const MicroSDC& microSDC_;
  /// a pointer to the metadata
  const std::shared_ptr<const MetadataProvider> metadata_;
  /// a pointer to the SubscriptionManager implementation
  const std::shared_ptr<SubscriptionManager> subscriptionManager_;
};
