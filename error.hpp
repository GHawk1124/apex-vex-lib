#pragma once

#include <vector>

namespace apex {
template <typename RT> struct motorInfo {
  double threshold;
  typedef RT (*currentDraw)(void);
  std::vector<currentDraw> draws;

  typedef void (*stop)(void);
  std::vector<stop> stops;
};

template <typename RT> void checkForCurrentDraw(motorInfo<RT> *info) {
  // TODO: Make this only stop affected motors
  for (auto draw : info->draws) {
    if (draw() > info->threshold) {
      for (auto stop : info->stops) {
        stop();
      }
    }
  }
}
} // namespace apex
