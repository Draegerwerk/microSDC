#pragma once

#include "SoapService.hpp"
#include "dpws/MetadataProvider.hpp"

class GetService : public SoapService
{
public:
  GetService(const MetadataProvider& metadata);
  std::string getURI() const override;
  void handleRequest(httpd_req* req, char* message) override;

private:
  const MetadataProvider metadata_;
  int randomValue;
};
