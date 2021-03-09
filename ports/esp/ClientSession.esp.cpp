#include "ClientSession.esp.hpp"
#include "Log.hpp"

#include "esp_http_client.h"

std::unique_ptr<ClientSessionInterface> ClientSessionFactory::produce(const std::string& address,
                                                                      const bool use_tls)
{
  return std::make_unique<ClientSessionEsp32>(address, use_tls);
}

ClientSessionEsp32::ClientSessionEsp32(std::string notify_to, const bool use_tls)
  : notify_to_(std::move(notify_to))
{
  extern const char serverCrtStart[] asm("_binary_server_crt_start");
  extern const char serverKeyStart[] asm("_binary_server_key_start");
  esp_http_client_config_t config{};
  config.url = notify_to_.c_str();
  if (use_tls)
  {
    config.client_cert_pem = serverCrtStart;
    config.client_key_pem = serverKeyStart;
    config.use_global_ca_store = true;
    config.skip_cert_common_name_check = true;
  }
  config.method = HTTP_METHOD_POST;
  config.transport_type = HTTP_TRANSPORT_OVER_TCP;
  session_ = esp_http_client_init(&config);
}

ClientSessionEsp32::~ClientSessionEsp32()
{
  auto err = esp_http_client_cleanup(session_);
  if (err == ESP_OK)
  {
    LOG(LogLevel::INFO, "Cleaned up client session for address " << notify_to_);
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
