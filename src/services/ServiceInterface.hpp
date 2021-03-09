#pragma once

#include <memory>
#include <string>

class Request;

/// @brief ServiceInterface defines an interface to arbitrary services attached to a WebServer for
/// handling requests
class ServiceInterface
{
public:
  ServiceInterface() = default;
  ServiceInterface(const ServiceInterface& other) = default;
  ServiceInterface(ServiceInterface&& other) = default;
  ServiceInterface& operator=(const ServiceInterface& other) = default;
  ServiceInterface& operator=(ServiceInterface&& other) = default;
  /// @brief virtual default construtor for polymorphism
  virtual ~ServiceInterface() = default;

  /// @brief returns the URI of this service
  /// @return the URI
  virtual std::string get_uri() const = 0;

  /// @brief handles any kind of incoming request
  /// @param req a pointer to the Request to be handled
  virtual void handle_request(std::unique_ptr<Request> req) = 0;
};
