#pragma once

#include "datamodel/MessageModel.hpp"
#include <memory>


/// @brief Request hold any information about a request a client sends to a server
class Request
{
public:
  /// @brief consturcts a new request based on a given message
  /// @param message the raw HTTP message content
  explicit Request(char* message);


  /// @brief gets the parsed envelope inside this request
  /// @return shared pointer to the envelope
  const MESSAGEMODEL::Envelope& getEnvelope();


  /// @brief enables direct access to raw string data
  /// @return const pointer to the raw data
  const char* data() const;


  /// @brief send a SOAP envelope as response to the requesting client
  /// @param responseEnvelope the SOAP envelope to send
  void respond(const MESSAGEMODEL::Envelope& responseEnvelope) const;


  /// @brief sends an actual response string to the requesting client
  /// @param msg the string to send
  virtual void respond(const std::string& msg) const = 0;

private:
  /// @brief parses the content of this request's raw message
  void parse();

  /// contains the parsed envelope of this request
  std::shared_ptr<MESSAGEMODEL::Envelope> envelope_{nullptr};
  /// the raw message string
  char* message_;
};
