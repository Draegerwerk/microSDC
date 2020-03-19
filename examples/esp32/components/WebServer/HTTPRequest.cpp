#include "HTTPRequest.hpp"

void HTTPRequest::respond(const std::string& msg) const
{
  httpd_resp_send(httpdReq_, msg.c_str(), msg.length());
}
