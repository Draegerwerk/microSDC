#pragma once

#include "SoapService.hpp"

class MicroSDC;
class MetadataProvider;
class SubscriptionManager;

/// @brief StateEventService implements the SDC StateEventService
class StateEventService : public SoapService
{
public:
  /// @brief constructs a new StateEventService from given metadata
  /// @param microSDC a reference to the MicroSDC instance holding this service
  /// @param metadata a pointer to the metadata describing configurational data
  /// @param subscriptionManager a pointer to the SubscriptionManager implementation
  StateEventService(const MicroSDC& micro_sdc, std::shared_ptr<const MetadataProvider> metadata,
                    std::shared_ptr<SubscriptionManager> subscription_manager);

  std::string get_uri() const override;
  void handle_request(std::unique_ptr<Request> req) override;

private:
  /// a reference to the microSDC instance holding this service
  const MicroSDC& micro_sdc_;
  /// a pointer to the metadata
  const std::shared_ptr<const MetadataProvider> metadata_;
  /// a pointer to the SubscriptionManager implementation
  const std::shared_ptr<SubscriptionManager> subscription_manager_;
};
