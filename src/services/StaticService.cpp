#include "StaticService.hpp"
#include "Log.hpp"
#include "WebServer/Request.hpp"

StaticService::StaticService(std::string uri, std::string static_content)
  : content_(std::move(static_content))
  , uri_(std::move(uri))
{
}

std::string StaticService::get_uri() const
{
  return uri_;
}

void StaticService::handle_request(std::unique_ptr<Request> req)
{
  LOG(LogLevel::DEBUG, "Send response for GET request " << uri_);
  req->respond(content_);
}
