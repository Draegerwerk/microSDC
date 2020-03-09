#include "StaticService.hpp"

#include "esp_log.h"

static constexpr const char* TAG = "StaticService";

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
  ESP_LOGD(TAG, "Send response for GET request %s", uri_.c_str());
  req->respond(content_);
}
