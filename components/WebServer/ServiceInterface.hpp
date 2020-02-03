#pragma once

#include "esp_http_server.h"
#include <string>

class ServiceInterface
{
public:
  ServiceInterface() = default;
  virtual ~ServiceInterface() = default;

  virtual std::string getURI() const = 0;
  virtual void handleRequest(httpd_req_t* req, char* message) = 0;
};
