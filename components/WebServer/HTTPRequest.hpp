#pragma once

#include "esp_http_server.h"
#include "services/Request.hpp"

class HTTPRequest : public Request
{
public:
  explicit HTTPRequest(httpd_req_t* req, char* message)
    : Request(message)
    , httpdReq_(req)
  {
  }

  void respond(const std::string& msg) const override;

private:
  httpd_req_t* httpdReq_;
};
