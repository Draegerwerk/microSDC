#pragma once

#include "SoapService.hpp"

class MetadataProvider;

/// @brief DeviceService implements the SDC Device service
class DeviceService : public SoapService
{
public:
  /// @brief constructs a new DeviceService from given metadata
  /// @param metadata a pointer to the metadata describing configurational data
  explicit DeviceService(std::shared_ptr<const MetadataProvider> metadata);

  std::string get_uri() const override;

  void handle_request(std::unique_ptr<Request> req) override;

private:
  /// a pointer to the metadata
  std::shared_ptr<const MetadataProvider> metadata_;
};
