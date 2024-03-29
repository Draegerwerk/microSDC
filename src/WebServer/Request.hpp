#pragma once

#include <memory>
#include <string>

namespace MESSAGEMODEL
{
  class Envelope;
} // namespace MESSAGEMODEL

/// @brief Request hold any information about a request a client sends to a server
class Request
{
public:
  /// @brief consturcts a new request based on a given message
  /// @param message the raw HTTP message content
  explicit Request(std::string msg);
  Request(const Request&) = delete;
  Request(Request&&) = delete;
  Request& operator=(const Request&) = delete;
  Request& operator=(Request&&) = delete;
  virtual ~Request() = default;


  /// @brief gets the parsed envelope inside this request
  /// @return shared pointer to the envelope
  const MESSAGEMODEL::Envelope& get_envelope();


  /// @brief enables direct access to raw string data
  /// @return const pointer to the raw data
  const char* data() const;


  /// @brief send a SOAP envelope as response to the requesting client
  /// @param responseEnvelope the SOAP envelope to send
  void respond(const MESSAGEMODEL::Envelope& response_envelope) const;

  /// @brief sends an actual response string to the requesting client
  /// @param msg the string to send
  virtual void respond(const std::string& msg) const;


private:
  /// @brief sends an actual response string to the requesting client
  /// @param msg the string to send
  virtual void send_response(const std::string& msg) const = 0;

  /// @brief parses the content of this request's raw message
  void parse();

  /// contains the parsed envelope of this request
  std::shared_ptr<MESSAGEMODEL::Envelope> envelope_{nullptr};
  /// the raw message string
  std::string message_;
};
