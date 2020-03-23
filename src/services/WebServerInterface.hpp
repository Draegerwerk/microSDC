#pragma once

#include <memory>

class ServiceInterface;

/// @brief WebServerInterface defines an interface to a WebServer handling HTTP(s) requests and
/// disptaching them to the respective registered services
class WebServerInterface
{
public:
  /// @brief starts and itinializes the WebServer
  virtual void start() = 0;

  /// @brief stops the webserver
  virtual void stop() = 0;

  /// @brief adds a service to the handlers
  /// @param service a pointer to the service to add
  virtual void addService(std::shared_ptr<ServiceInterface> service) = 0;
};
