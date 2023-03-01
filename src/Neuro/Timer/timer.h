#ifndef SRC_TIMER_TIMER_H_
#define SRC_TIMER_TIMER_H_

#include <chrono>
#include <iostream>

namespace s21 {

class Timer {
 public:
  Timer() = default;
  explicit Timer(bool auto_start);
  ~Timer() = default;
  auto start() -> void;
  auto stop() -> void;
  auto showTime() -> void;
  auto showTime(const std::string &message) -> void;
  auto getTime() -> float;

 private:
  std::chrono::high_resolution_clock::time_point m_start{};
  std::chrono::high_resolution_clock::time_point m_end{};
};

} // namespace s21

#endif  // SRC_TIMER_TIMER_H_
