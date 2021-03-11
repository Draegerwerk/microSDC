#pragma once

#include "MicroSDC.hpp"
#include "StateHandler.hpp"
#include "networking/NetworkConfig.hpp"

class SimpleDevice
{
public:
  /// @brief Implements a MdStateHandler for NumericStates
  class NumericStateHandler : public StateHandler
  {
  public:
    /// @brief constructs a new NumericStateHandler attached to a given descriptor state handle
    /// @param descriptorHandle the handle of the state's descriptor
    explicit NumericStateHandler(const std::string& descriptor_handle);

    std::shared_ptr<BICEPS::PM::AbstractState> get_initial_state() const override;

    BICEPS::MM::InvocationState request_state_change(const BICEPS::MM::AbstractSet& set) override;

    /// @param sets a new numeric value to the state handled by this handler and updates the mdib
    /// @param value the new value to set
    void set_value(double value);
  };

  explicit SimpleDevice();
  void set_network_config(std::unique_ptr<NetworkConfig> network_config);
  void init();
  void start_sdc();
  void run();

private:
  MicroSDC sdc_;

  std::shared_ptr<NumericStateHandler> pressure_state_handler_;
  std::shared_ptr<NumericStateHandler> temperature_state_handler_;
  std::shared_ptr<NumericStateHandler> humidity_state_handler_;
  // std::shared_ptr<NumericStateHandler> settable_state_handler_;
};
