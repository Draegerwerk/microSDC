#include "WebServer.linux.hpp"
#include "networking/NetworkConfig.hpp"

std::unique_ptr<WebServerInterface>
WebServerFactory::produce(const std::shared_ptr<const NetworkConfig>& network_config)
{
  if (network_config->is_using_tls())
  {
    return std::make_unique<WebServerSimple<SimpleWeb::HTTPS>>();
  }
  return std::make_unique<WebServerSimple<SimpleWeb::HTTP>>();
}

template <>
WebServerSimple<SimpleWeb::HTTPS>::WebServerSimple()
  : server_(std::make_unique<SimpleWeb::Server<SimpleWeb::HTTPS>>(
        "./certs/server.crt", "./certs/server.key", "./certs/ca.crt"))
{
  server_->config.port = 8080;
  server_->config.timeout_content = 0;
  server_->config.timeout_request = 0;
  server_->on_error = [](std::shared_ptr<SimpleWeb::Server<SimpleWeb::HTTPS>::Request> /*request*/,
                         const SimpleWeb::error_code& ec) {
    LOG(LogLevel::ERROR, "Error processing request: " << ec);
  };
}

template <>
WebServerSimple<SimpleWeb::HTTP>::WebServerSimple()
  : server_(std::make_unique<SimpleWeb::Server<SimpleWeb::HTTP>>())
{
  server_->config.port = 8080;
  // server_->config.timeout_content = 10;
  // server_->config.timeout_request = 10;
}
