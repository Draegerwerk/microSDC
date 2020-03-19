#include "StateHandler.hpp"

StateHandler::StateHandler(std::string descriptorHandle)
  : descriptorHandle_(std::move(descriptorHandle))
{
}

const std::string& StateHandler::getDescriptorHandle() const
{
  return descriptorHandle_;
}

void StateHandler::setMicroSDC(MicroSDC* microSDC)
{
  microSDC_ = microSDC;
}
