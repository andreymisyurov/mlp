#ifndef SRC_TIMER_TIMER_H_
#define SRC_TIMER_TIMER_H_

#include <chrono>
#include <iostream>

class Timer {
 public:
  Timer(){};
  ~Timer(){};
  auto Start() -> void;
  auto Stop() -> void;
  auto ShowTime() -> void;
  auto ShowTime(std::string message) -> void;

 private:
  std::chrono::high_resolution_clock::time_point start_{};
  std::chrono::high_resolution_clock::time_point end_{};
};

#endif  // SRC_TIMER_TIMER_H_
