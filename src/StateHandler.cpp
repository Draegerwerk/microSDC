#include "StateHandler.hpp"

StateHandler::StateHandler(const StateHandlerKind k, std::string descriptorHandle)
  : kind_(k)
  , descriptorHandle_(std::move(descriptorHandle))
{
}

StateHandler::StateHandlerKind StateHandler::getKind() const
{
  return kind_;
}

const std::string& StateHandler::getDescriptorHandle() const
{
  return descriptorHandle_;
}

void StateHandler::setMicroSDC(MicroSDC* microSDC)
{
  microSDC_ = microSDC;
}
