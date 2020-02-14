#pragma once

#include "MicroSDC.hpp"
#include "SoapService.hpp"
#include "dpws/MetadataProvider.hpp"

class SetService : public SoapService
{
public:
  SetService(const MicroSDC& microSDC, MetadataProvider metadata);
  std::string getURI() const override;
  void handleRequest(httpd_req* req, char* message) override;

private:
  const MicroSDC& microSDC_;
  const MetadataProvider metadata_;
};
