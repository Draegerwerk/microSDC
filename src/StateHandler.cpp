#include "StateHandler.hpp"

StateHandler::StateHandler(std::string descriptor_handle)
  : descriptor_handle_(std::move(descriptor_handle))
{
}

void StateHandler::update_state(const std::shared_ptr<BICEPS::PM::AbstractState>& state)
{
  if (micro_sdc_ == nullptr)
  {
    LOG(LogLevel::ERROR, "Cannot update state of unassigned state! Use add_md_state!");
    return;
  }
  micro_sdc_->update_state(state);
}

const std::string& StateHandler::get_descriptor_handle() const
{
  return descriptor_handle_;
}

void StateHandler::set_micro_sdc(MicroSDC* micro_sdc)
{
  micro_sdc_ = micro_sdc;
}
