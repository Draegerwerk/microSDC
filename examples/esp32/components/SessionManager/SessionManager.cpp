#include "SessionManager.hpp"
#include "Log.hpp"

#include "esp_http_client.h"

ClientSession::ClientSession(const std::string& notifyTo)
  : notifyTo_(notifyTo)
{
  extern const char serverCert_pem_start[] asm("_binary_serverCert_pem_start");
  extern const char serverKey_pem_start[] asm("_binary_serverKey_pem_start");
  esp_http_client_config_t config;
  config.url = notifyTo.c_str();
  config.host = nullptr;
  config.port = 0;
  config.username = nullptr;
  config.password = nullptr;
  config.auth_type = HTTP_AUTH_TYPE_NONE;
  config.path = nullptr;
  config.query = nullptr;
  config.cert_pem = nullptr;
  config.client_cert_pem = serverCert_pem_start;
  config.client_key_pem = serverKey_pem_start;
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

ClientSession::~ClientSession()
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

void ClientSession::send(const std::string& message) const
{
  esp_http_client_set_post_field(session_, message.c_str(), message.length());
  esp_err_t err = esp_http_client_perform(session_);
  if (err == ESP_OK)
  {
    LOG(LogLevel::DEBUG,
        "HTTPS Status = " << esp_http_client_get_status_code(session_)
                          << " , content_length = " << esp_http_client_get_content_length(session_));
  }
  else
  {
    LOG(LogLevel::ERROR, "Error perform http request " << esp_err_to_name(err));
  }
}

void SessionManager::createSession(const std::string& notifyTo)
{
  if (sessions_.count(notifyTo) != 0)
  {
    LOG(LogLevel::INFO, "Client session already exists");
    return;
  }
  sessions_.emplace(notifyTo, std::make_shared<ClientSession>(notifyTo));
}

void SessionManager::sendToSession(const std::string& notifyTo, const std::string& message)
{
  auto sessionIt = sessions_.find(notifyTo);
  if (sessionIt == sessions_.end())
  {
    LOG(LogLevel::ERROR, "Cannot find client session with address " << notifyTo);
    return;
  }
  LOG(LogLevel::INFO, "Sending to " << notifyTo);
  sessionIt->second->send(message);
}

void SessionManager::deleteSession(const std::string& notifyTo)
{
  sessions_.erase(notifyTo);
}
