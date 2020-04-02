#include "ClientSession.esp.hpp"
#include "Log.hpp"

#include "esp_http_client.h"

std::unique_ptr<ClientSessionInterface> ClientSessionFactory::produce(const std::string& address)
{
  return std::make_unique<ClientSessionEsp32>(address);
}

ClientSessionEsp32::ClientSessionEsp32(std::string notifyTo)
  : notifyTo_(std::move(notifyTo))
{
  extern const char server_crt_start[] asm("_binary_server_crt_start");
  extern const char server_key_start[] asm("_binary_server_key_start");
  esp_http_client_config_t config;
  config.url = notifyTo_.c_str();
  config.host = nullptr;
  config.port = 0;
  config.username = nullptr;
  config.password = nullptr;
  config.auth_type = HTTP_AUTH_TYPE_NONE;
  config.path = nullptr;
  config.query = nullptr;
  config.cert_pem = nullptr;
  config.client_cert_pem = server_crt_start;
  config.client_key_pem = server_key_start;
  config.method = HTTP_METHOD_POST;
  config.timeout_ms = 0;
  config.disable_auto_redirect = false;
  config.max_redirection_count = 0;
  config.event_handler = nullptr;
  config.transport_type = HTTP_TRANSPORT_OVER_TCP;
  config.buffer_size = 0;
  config.buffer_size_tx = 0;
  config.user_data = nullptr;
  config.is_async = false;
  config.use_global_ca_store = true;
  config.skip_cert_common_name_check = true;
  session_ = esp_http_client_init(&config);
}

ClientSessionEsp32::~ClientSessionEsp32()
{
  auto err = esp_http_client_cleanup(session_);
  if (err == ESP_OK)
  {
    LOG(LogLevel::INFO, "Cleaned up client session for address " << notifyTo_);
  }
  else
  {
    LOG(LogLevel::ERROR, "Error cleaning up client session " << esp_err_to_name(err));
  }
}

void ClientSessionEsp32::send(const std::string& message)
{
  esp_http_client_set_post_field(session_, message.c_str(), message.length());
  esp_err_t err = esp_http_client_perform(session_);
  if (err == ESP_OK)
  {
    LOG(LogLevel::DEBUG, "HTTPS Status = " << esp_http_client_get_status_code(session_)
                                           << " , content_length = "
                                           << esp_http_client_get_content_length(session_));
  }
  else
  {
    LOG(LogLevel::ERROR, "Error perform http request " << esp_err_to_name(err));
  }
}

