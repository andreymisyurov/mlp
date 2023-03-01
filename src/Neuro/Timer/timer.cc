#include "timer.h"

auto Timer::Start() -> void {
  start_ = std::chrono::high_resolution_clock::now();
}

auto Timer::Stop() -> void {
  end_ = std::chrono::high_resolution_clock::now();
};

auto Timer::ShowTime() -> void {
  std::chrono::duration<float> duration = end_ - start_;
  std::cout << "Time of execute: " << duration.count() << std::endl;
}

auto Timer::getTime() -> float {
  std::chrono::duration<float> duration = end_ - start_;
  return duration.count();
}

auto Timer::ShowTime(std::string message) -> void {
  std::chrono::duration<float> duration = end_ - start_;
  std::cout << message << duration.count() << " sec" << std::endl;
}
