#pragma once

#include "services/ServiceInterface.hpp"
#include <memory>

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
