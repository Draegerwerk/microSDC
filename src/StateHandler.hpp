#pragma once

#include "Casting.hpp"
#include "Log.hpp"
#include "MicroSDC.hpp"
#include "datamodel/BICEPS_ParticipantModel.hpp"


/// @brief Abstract class to handle states of this device. Exposes updateState() to update this
/// state in the microSDC mdib.
class StateHandler
{
public:
  /// @brief Constructs a new StateHandler referring to a descriptor
  /// @param descriptorHandle the handle of the associated descriptor
  explicit StateHandler(std::string descriptor_handle);
  StateHandler(const StateHandler&) = delete;
  StateHandler(StateHandler&&) = delete;
  StateHandler& operator=(const StateHandler&) = delete;
  StateHandler& operator=(StateHandler&&) = delete;
  virtual ~StateHandler() = default;

  /// @brief gets the handle of the associated descriptor
  /// @return the descriptor's handle of this state
  const std::string& get_descriptor_handle() const;

  /// @brief returns the state type of this state. This is used to fake RTTI for dynamic
  /// subclassing.
  /// @return The state type of this state
  // StateHandlerKind getKind() const;

  /// @brief sets the MicroSDC instantation, which handles this state
  /// @param microSDC the pointer MicroSDC object
  void set_micro_sdc(MicroSDC* micro_sdc);

  /// @brief updates this state in the mdib of the holding MicroSDC object
  /// @tparam State infered state type of this state
  /// @param state the new state to update
  void update_state(const std::shared_ptr<BICEPS::PM::AbstractState>& state);

  /// @brief gets the initial state object of this metric
  /// @return pointer to the MdState constructed
  virtual std::shared_ptr<BICEPS::PM::AbstractState> get_initial_state() const = 0;

private:
  /// pointer to the holding MicroSDC object
  MicroSDC* micro_sdc_{nullptr};
  /// handle of the associated descriptor
  std::string descriptor_handle_;
};
