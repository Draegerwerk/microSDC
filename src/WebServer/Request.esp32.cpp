#include "Request.esp32.hpp"

RequestEsp32::RequestEsp32(httpd_req_t* req, char* message)
  : Request(message)
  , httpdReq_(req)
{
}

void RequestEsp32::sendResponse(const std::string& msg) const
{
  httpd_resp_send(httpdReq_, msg.c_str(), msg.length());
}
