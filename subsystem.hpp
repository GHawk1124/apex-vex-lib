#pragma once

#include "stateMachine.hpp"

namespace apex {
class subsystem : public StateMachine {
public:
  void init();

  void execute();

  void shutdown();
};
} // namespace apex
