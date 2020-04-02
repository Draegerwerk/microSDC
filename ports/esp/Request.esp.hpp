#pragma once

#include "WebServer/Request.hpp"
#include "esp_https_server.h"

class RequestEsp32 : public Request
{
public:
  explicit RequestEsp32(httpd_req_t* req, std::string msg);

private:
  void sendResponse(const std::string& msg) const override;

  httpd_req_t* httpdReq_;
};
