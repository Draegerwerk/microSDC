#include "Request.esp.hpp"

RequestEsp32::RequestEsp32(httpd_req_t* req, std::string msg)
  : Request(std::move(msg))
  , httpdReq_(req)
{
}

void RequestEsp32::sendResponse(const std::string& msg) const
{
  httpd_resp_send(httpdReq_, msg.c_str(), msg.length());
}
