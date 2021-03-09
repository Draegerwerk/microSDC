#pragma once

#include "WebServer/WebServer.hpp"
#include "esp_https_server.h"
#include <vector>

class ServiceInterface;

class WebServerEsp32 : public WebServerInterface
{
public:
  explicit WebServerEsp32(bool use_tls);
  void start() override;
  void stop() override;

  void add_service(std::shared_ptr<ServiceInterface> service) override;

private:
  httpd_handle_t server_{nullptr};
  httpd_ssl_config_t config_ = HTTPD_SSL_CONFIG_DEFAULT();
  std::vector<std::shared_ptr<ServiceInterface>> services_;
  static esp_err_t handler_callback(httpd_req_t* req);
  void register_uri_handlers();
};
