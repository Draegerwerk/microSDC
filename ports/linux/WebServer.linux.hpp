#pragma once

#include "Log.hpp"
#include "Request.linux.hpp"
#include "WebServer/WebServer.hpp"
#include "rapidxml.hpp"
#include "server_https.hpp"
#include "services/ServiceInterface.hpp"
#include <future>

class ServiceInterface;

template <class SocketType>
class WebServerSimple : public WebServerInterface
{
public:
  explicit WebServerSimple();
  WebServerSimple(const WebServerSimple& other) = delete;
  WebServerSimple& operator=(const WebServerSimple& other) = delete;
  WebServerSimple(WebServerSimple&& other) = delete;
  WebServerSimple& operator=(WebServerSimple&& other) = delete;
  ~WebServerSimple() override;
  void start() override;
  void stop() override;

  void add_service(std::shared_ptr<ServiceInterface> service) override;

private:
  std::unique_ptr<SimpleWeb::Server<SocketType>> server_;
  std::thread server_thread_{};
};

template <class SocketType>
void WebServerSimple<SocketType>::start()
{
  // Start server and receive assigned port when server is listening for requests
  std::promise<std::uint16_t> server_port;
  server_thread_ = std::thread([this, &server_port]() {
    // Start server
    server_->start([&server_port](unsigned short port) { server_port.set_value(port); });
  });
  LOG(LogLevel::INFO, "Server listening on port " << server_port.get_future().get());
}

template <class SocketType>
void WebServerSimple<SocketType>::stop()
{
  server_->stop();
  LOG(LogLevel::INFO, "Server stopping...");
  server_thread_.join();
}

template <class SocketType>
WebServerSimple<SocketType>::~WebServerSimple<SocketType>()
{
  stop();
}

template <class SocketType>
void WebServerSimple<SocketType>::add_service(std::shared_ptr<ServiceInterface> service)
{
  const auto handler =
      [service](std::shared_ptr<typename SimpleWeb::Server<SocketType>::Response> response,
                std::shared_ptr<typename SimpleWeb::Server<SocketType>::Request> request) {
        try
        {
          service->handle_request(std::make_unique<RequestSimple<SocketType>>(response, request));
        }
        catch (rapidxml::parse_error& e)
        {
          LOG(LogLevel::ERROR, "Rapidxml Parse error: " << e.what());
        }
        catch (std::exception& e)
        {
          LOG(LogLevel::ERROR, "Error handling Request: std::exception: " << e.what());
        }
        catch (...)
        {
          LOG(LogLevel::ERROR, "Error while handling request!");
        }
      };
  server_->resource["^" + service->get_uri() + "$"]["GET"] = handler;
  server_->resource["^" + service->get_uri() + "$"]["POST"] = handler;
}
