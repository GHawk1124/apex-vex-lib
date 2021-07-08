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
  for (int i; i < info->draws.len; i++) {
    if (info->draws[i]() > info->threshold) {
      info->stops[i]();
    }
  }
}
} // namespace apex
