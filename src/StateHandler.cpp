#include "StateHandler.hpp"

StateHandler::StateHandler(std::string descriptorHandle)
  : descriptorHandle_(std::move(descriptorHandle))
{
}

void StateHandler::updateState(const std::shared_ptr<BICEPS::PM::AbstractState>& state)
{
  if (microSDC_ == nullptr)
  {
    LOG(LogLevel::ERROR, "Cannot update state of unassigned state! Use addMdState!");
    return;
  }
  microSDC_->updateState(state);
}

const std::string& StateHandler::getDescriptorHandle() const
{
  return descriptorHandle_;
}

void StateHandler::setMicroSDC(MicroSDC* microSDC)
{
  microSDC_ = microSDC;
}
