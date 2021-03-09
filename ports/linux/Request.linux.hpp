#pragma once

#include "Log.hpp"
#include "WebServer/Request.hpp"
#include "server_https.hpp"

template <class SocketType>
class RequestSimple : public Request
{
public:
  explicit RequestSimple(std::shared_ptr<typename SimpleWeb::Server<SocketType>::Response> response,
                         std::shared_ptr<typename SimpleWeb::Server<SocketType>::Request> request);
  RequestSimple(const RequestSimple& other) = delete;
  RequestSimple(RequestSimple&& other) = delete;
  RequestSimple& operator=(const RequestSimple& other) = delete;
  RequestSimple& operator=(RequestSimple&& other) = delete;
  ~RequestSimple() override = default;

private:
  void send_response(const std::string& msg) const override;

  const std::shared_ptr<typename SimpleWeb::Server<SocketType>::Response> response_;
  const std::shared_ptr<const typename SimpleWeb::Server<SocketType>::Request> request_;
};

template <class SocketType>
RequestSimple<SocketType>::RequestSimple(
    std::shared_ptr<typename SimpleWeb::Server<SocketType>::Response> response,
    std::shared_ptr<typename SimpleWeb::Server<SocketType>::Request> request)
  : Request(request->content.string())
  , response_(std::move(response))
  , request_(std::move(request))
{
}

template <class SocketType>
void RequestSimple<SocketType>::send_response(const std::string& msg) const
{
  LOG(LogLevel::DEBUG, "Writing: \n" << msg);
  // response_->close_connection_after_response = true;
  response_->write(msg);
}
