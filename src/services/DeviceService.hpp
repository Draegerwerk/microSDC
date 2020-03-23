#pragma once

#include "SoapService.hpp"
#include "dpws/MetadataProvider.hpp"

// TODO: doxygen
class DeviceService : public SoapService
{
public:
  explicit DeviceService(std::shared_ptr<const MetadataProvider> metadata);
  std::string getURI() const override;
  void handleRequest(std::shared_ptr<Request> req) override;

private:
  const std::shared_ptr<const MetadataProvider> metadata_;
};
