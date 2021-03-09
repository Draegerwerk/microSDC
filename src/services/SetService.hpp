#pragma once

#include "SoapService.hpp"
#include "datamodel/BICEPS_MessageModel.hpp"
#include <memory>

class MicroSDC;
class SubscriptionManager;
class MetadataProvider;
namespace BICEPS::MM
{
  class SetValue;
} // namespace BICEPS::MM

/// @brief SetService implements the SDC Set service
class SetService : public SoapService
{
public:
  /// @brief constructs a new SetService from given metadata
  /// @param microSDC a reference to the MicroSDC instance holding this service
  /// @param metadata a pointer to the metadata describing configurational data
  /// @param subscriptionManager a pointer to the SubscriptionManager implementation
  SetService(MicroSDC* micro_sdc, std::shared_ptr<const MetadataProvider> metadata,
             std::shared_ptr<SubscriptionManager> subscription_manager);

  std::string get_uri() const override;
  void handle_request(std::unique_ptr<Request> req) override;

private:
  /// a reference to the microSDC instance holding this service
  MicroSDC* micro_sdc_;
  /// a pointer to the metadata
  const std::shared_ptr<const MetadataProvider> metadata_;
  /// a pointer to the SubscriptionManager implementation to maintain client subscriptions
  const std::shared_ptr<SubscriptionManager> subscription_manager_;

  /// @brief dispatches an incoming request to the respective handlers and processes it
  /// @param setValueRequest the SetValue request to dispatch
  /// @return the response to the request after processing completed
  BICEPS::MM::SetStringResponse dispatch(const BICEPS::MM::SetString& set_string_request);
  BICEPS::MM::SetValueResponse dispatch(const BICEPS::MM::SetValue& set_value_request);
};
