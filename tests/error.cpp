#include "../error.hpp"

#include <thread>

double testc() { return 0; }

void tests() {}

int main() {
  apex::motorInfo<double> info{};
  info.threshold = 5;
  info.draws.push_back(testc);
  info.stops.push_back(tests);

  std::thread test(apex::checkForCurrentDraw<double>, &info);
  test.join();
}
