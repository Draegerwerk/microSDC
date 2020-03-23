#pragma once

#include "SoapService.hpp"
#include "dpws/MetadataProvider.hpp"

class MicroSDC;

// TODO: doxygen
class GetService : public SoapService
{
public:
  GetService(const MicroSDC& microSDC, std::shared_ptr<const MetadataProvider> metadata);
  std::string getURI() const override;
  void handleRequest(std::shared_ptr<Request> req) override;

private:
  const MicroSDC& microSDC_;
  const std::shared_ptr<const MetadataProvider> metadata_;
};
