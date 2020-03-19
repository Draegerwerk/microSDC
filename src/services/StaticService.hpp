#pragma once

#include "ServiceInterface.hpp"

class StaticService : public ServiceInterface
{
public:
  StaticService(std::string uri, std::string staticContent);

  std::string getURI() const override;
  void handleRequest(std::shared_ptr<Request> req) override;

private:
  const std::string content_;
  const std::string uri_;
};
