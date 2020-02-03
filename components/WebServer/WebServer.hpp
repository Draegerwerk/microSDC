#pragma once

#include "ServiceInterface.hpp"
#include "esp_err.h"
#include "esp_https_server.h"
#include <memory>
#include <vector>

class WebServer
{
public:
  WebServer(bool useTLS);
  esp_err_t start();
  esp_err_t stop();

  void addService(std::shared_ptr<ServiceInterface> service);

protected:
  httpd_handle_t server_{nullptr};
  httpd_ssl_config_t config_ = HTTPD_SSL_CONFIG_DEFAULT();
  std::vector<std::shared_ptr<ServiceInterface>> services_;
  static esp_err_t handlerCallback(httpd_req_t* req);
  void register_uri_handlers();
};
