#pragma once

#include "SoapService.hpp"
#include "datamodel/MessageModel.hpp"
#include "dpws/MetadataProvider.hpp"
#include "esp_http_server.h"

class DeviceService : public SoapService
{
public:
  DeviceService(const MetadataProvider& metadata);
  void handleRequest(httpd_req_t* req, char* message) override;
  std::string getURI() const override;

private:
  const MetadataProvider metadata_;
};
