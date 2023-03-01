#include "timer.h"

Timer::Timer(bool){
  start();
};

auto Timer::start() -> void {
  m_start = std::chrono::high_resolution_clock::now();
}

auto Timer::stop() -> void {
  m_end = std::chrono::high_resolution_clock::now();
};

auto Timer::showTime() -> void {
  std::chrono::duration<float> duration = m_end - m_start;
  std::cout << "Time of execute: " << duration.count() << std::endl;
}

auto Timer::showTime(const std::string& in_message) -> void {
  std::chrono::duration<float> duration = m_end - m_start;
  std::cout << in_message << duration.count() << " sec" << std::endl;
}

auto Timer::getTime() -> float {
  std::chrono::duration<float> duration = m_end - m_start;
  return duration.count();
}
