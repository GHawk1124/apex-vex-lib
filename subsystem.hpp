#pragma once

#include "stateMachine.hpp"

namespace apex {
class SubsystemData : public EventData {
public:
  int test;
};

class Subsystem : public StateMachine {
public:
  virtual void init();

  virtual void execute();

  virtual void shutdown();
};
} // namespace apex
