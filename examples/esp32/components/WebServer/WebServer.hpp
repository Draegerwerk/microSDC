#pragma once

#include "services/WebServerInterface.hpp"
#include "esp_https_server.h"
#include <vector>

class WebServer : public WebServerInterface
{
public:
  explicit WebServer(bool useTLS);
  void start() override;
  void stop() override;

  void addService(std::shared_ptr<ServiceInterface> service) override;

private:
  httpd_handle_t server_{nullptr};
  httpd_ssl_config_t config_ = HTTPD_SSL_CONFIG_DEFAULT();
  std::vector<std::shared_ptr<ServiceInterface>> services_;
  static esp_err_t handlerCallback(httpd_req_t* req);
  void registerUriHandlers();
};
