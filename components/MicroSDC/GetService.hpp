#pragma once

#include "MicroSDC.hpp"
#include "SoapService.hpp"
#include "dpws/MetadataProvider.hpp"

class GetService : public SoapService
{
public:
  GetService(const MicroSDC& microSDC, const MetadataProvider& metadata);
  std::string getURI() const override;
  void handleRequest(httpd_req* req, char* message) override;

private:
  const MicroSDC& microSDC_;
  const MetadataProvider metadata_;
};
