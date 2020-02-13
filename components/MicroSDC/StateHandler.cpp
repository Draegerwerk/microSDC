#include "StateHandler.hpp"
#include "MicroSDC.hpp"
#include "esp_log.h"

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

// explicit instantiation
template void StateHandler::updateState<BICEPS::PM::NumericMetricState>(
    std::shared_ptr<BICEPS::PM::NumericMetricState> state);

template <class State>
void StateHandler::updateState(std::shared_ptr<State> state)
{
  if (microSDC_ == nullptr)
  {
    ESP_LOGE("StateHandler", "Cannot update state of unassigned state! Use addMdState!");
    return;
  }
  microSDC_->updateState(state);
}
