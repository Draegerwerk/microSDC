#pragma once

#include "datamodel/BICEPS_ParticipantModel.hpp"

class MicroSDC;

class StateHandler
{
public:
  explicit StateHandler(std::string descriptorHandle);

  const std::string& getDescriptorHandle() const;

  virtual BICEPS::PM::MetricType getMetricType() const = 0;

  void setMicroSDC(MicroSDC* microSDC);

  template <class State>
  void updateState(std::shared_ptr<State> state);

private:
  MicroSDC* microSDC_{nullptr};
  std::string descriptorHandle_;
};

template <class MdState>
class MdStateHandler : public StateHandler
{
public:
  explicit MdStateHandler(std::string descriptorHandle)
    : StateHandler(descriptorHandle)
  {
  }

  virtual std::shared_ptr<MdState> getInitialState() const = 0;
};
