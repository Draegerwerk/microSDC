#include "StaticService.hpp"
#include "Log.hpp"
#include "WebServer/Request.hpp"

StaticService::StaticService(std::string uri, std::string staticContent)
  : content_(std::move(staticContent))
  , uri_(std::move(uri))
{
}

std::string StaticService::getURI() const
{
  return uri_;
}

void StaticService::handleRequest(std::shared_ptr<Request> req)
{
  LOG(LogLevel::DEBUG, "Send response for GET request " << uri_);
  req->respond(content_);
}
