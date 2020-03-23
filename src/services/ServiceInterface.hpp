#pragma once

#include <string>
#include <memory>

class Request;

/// @brief ServiceInterface defines an interface to arbitrary services attached to a WebServer for
/// handling requests
class ServiceInterface
{
public:
  /// @brief virtual default construtor for polymorphism
  virtual ~ServiceInterface() = default;

  /// @brief returns the URI of this service
  /// @return the URI
  virtual std::string getURI() const = 0;

  /// @brief handles any kind of incoming request
  /// @param req a pointer to the Request to be handled
  virtual void handleRequest(std::shared_ptr<Request> req) = 0;
};
