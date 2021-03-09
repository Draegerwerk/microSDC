#pragma once

#include "ServiceInterface.hpp"

/// @brief StaticService implements a service providing a static resource like wsdl descriptions
class StaticService : public ServiceInterface
{
public:
  /// @brief constructs a new StaticService with given uri and content
  /// @param uri the uri of this service
  /// @param staticContent the content to provide
  StaticService(std::string uri, std::string static_content);

  std::string get_uri() const override;
  void handle_request(std::unique_ptr<Request> req) override;

private:
  /// the content this service exposes and provides
  const std::string content_;
  /// the uri of this web service
  const std::string uri_;
};
