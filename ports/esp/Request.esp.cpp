#include "Request.esp.hpp"

RequestEsp32::RequestEsp32(httpd_req_t* req, std::string msg)
  : Request(std::move(msg))
  , httpd_req_(req)
{
}

void RequestEsp32::send_response(const std::string& msg) const
{
  httpd_resp_send(httpd_req_, msg.c_str(), msg.length());
}
