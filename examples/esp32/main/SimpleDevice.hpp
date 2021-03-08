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
    explicit NumericStateHandler(const std::string& descriptorHandle);
    NumericStateHandler(const NumericStateHandler& other) = delete;
    NumericStateHandler(NumericStateHandler&& other) = delete;
    NumericStateHandler& operator=(const NumericStateHandler& other) = delete;
    NumericStateHandler& operator=(NumericStateHandler&& other) = delete;
    ~NumericStateHandler() override = default;

    std::shared_ptr<BICEPS::PM::AbstractState> getInitialState() const override;

    /// @param sets a new numeric value to the state handled by this handler and updates the mdib
    /// @param value the new value to set
    void setValue(double value);
  };

  explicit SimpleDevice();
  void setNetworkConfig(std::unique_ptr<NetworkConfig> networkConfig);
  void init();
  void startSDC();
  void run();

private:
  MicroSDC sdc_;

  std::shared_ptr<NumericStateHandler> pressureStateHandler_;
  std::shared_ptr<NumericStateHandler> temperatureStateHandler_;
  std::shared_ptr<NumericStateHandler> humidityStateHandler_;
  std::shared_ptr<NumericStateHandler> settableStateHandler_;
};
