#pragma once

#include "Log.hpp"
#include "MicroSDC.hpp"
#include "datamodel/BICEPS_ParticipantModel.hpp"


/// @brief Abstract class to handle states of this device. Exposes updateState() to update this
/// state in the microSDC mdib.
class StateHandler
{
public:
  enum class StateHandlerKind
  {
    NUMERIC_METRIC,
  };

  /// @brief Constructs a new StateHandler referring to a descriptor
  /// @param descriptorHandle the handle of the associated descriptor
  explicit StateHandler(StateHandlerKind k, std::string descriptorHandle);

  /// @brief gets the handle of the associated descriptor
  /// @return the descriptor's handle of this state
  const std::string& getDescriptorHandle() const;

  /// @brief returns the state type of this state. This is used to fake RTTI for dynamic
  /// subclassing.
  /// @return The state type of this state
  StateHandlerKind getKind() const;

  /// @brief sets the MicroSDC instantation, which handles this state
  /// @param microSDC the pointer MicroSDC object
  void setMicroSDC(MicroSDC* microSDC);

  /// @brief updates this state in the mdib of the holding MicroSDC object
  /// @tparam State infered state type of this state
  /// @param state the new state to update
  template <class State>
  void updateState(std::shared_ptr<State> state)
  {
    if (microSDC_ == nullptr)
    {
      LOG(LogLevel::ERROR, "Cannot update state of unassigned state! Use addMdState!");
      return;
    }
    microSDC_->updateState(state);
  }

private:
  const StateHandlerKind kind_;
  /// pointer to the holding MicroSDC object
  MicroSDC* microSDC_{nullptr};
  /// handle of the associated descriptor
  std::string descriptorHandle_;
};


/// @brief Implements a StateHandler of an arbitrary MdState
/// @tparam MdState the state type of this metric state
template <class MdState>
class MdStateHandler : public StateHandler
{
public:
  /// @brief constructs a new MdStateHandler referring to a descriptor
  /// @param descriptorHandle the handle of the associated descriptor
  explicit MdStateHandler(StateHandlerKind k, const std::string& descriptorHandle)
    : StateHandler(k, descriptorHandle)
  {
  }

  /// @brief gets the initial state object of this metric
  /// @return pointer to the MdState constructed
  virtual std::shared_ptr<MdState> getInitialState() const = 0;
};

/// @brief Implements a MdStateHandler for NumericStates
class NumericStateHandler : public MdStateHandler<BICEPS::PM::NumericMetricState>
{
public:
  /// @brief constructs a new NumericStateHandler attached to a given descriptor state handle
  /// @param descriptorHandle the handle of the state's descriptor
  explicit NumericStateHandler(const std::string& descriptorHandle)
    : MdStateHandler(StateHandlerKind::NUMERIC_METRIC, descriptorHandle)
  {
  }

  static bool classof(const StateHandler* other)
  {
    return other->getKind() == StateHandlerKind::NUMERIC_METRIC;
  }

  std::shared_ptr<BICEPS::PM::NumericMetricState> getInitialState() const override
  {
    auto state = std::make_shared<BICEPS::PM::NumericMetricState>(getDescriptorHandle());
    state->metricValue = std::make_optional<BICEPS::PM::NumericMetricValue>(
        BICEPS::PM::MetricQualityType{BICEPS::PM::MeasurementValidity::Vld});
    state->metricValue->value = 0;
    return state;
  }

  /// @param sets a new numeric value to the state handled by this handler and updates the mdib
  /// @param value the new value to set
  void setValue(double value)
  {
    auto state = getInitialState();
    state->metricValue->value = value;
    updateState(state);
  }
};
