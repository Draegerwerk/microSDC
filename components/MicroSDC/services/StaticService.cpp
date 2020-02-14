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

void StaticService::handleRequest(httpd_req_t* req, char*  /*message*/)
{
  ESP_LOGI(TAG, "Send response for GET request %s", uri_.c_str());
  httpd_resp_send(req, content_.c_str(), content_.length());
}
