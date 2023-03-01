#include "timer.h"

using namespace s21;

Timer::Timer(bool) { start(); };

void Timer::start() {
  m_start = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
  m_end = std::chrono::high_resolution_clock::now();
};

void Timer::showTime() {
  std::chrono::duration<float> duration = m_end - m_start;
  std::cout << "Time of execute: " << duration.count() << std::endl;
}

void Timer::showTime(const std::string& in_message) {
  std::chrono::duration<float> duration = m_end - m_start;
  std::cout << in_message << duration.count() << " sec" << std::endl;
}

float Timer::getTime() {
  std::chrono::duration<float> duration = m_end - m_start;
  return duration.count();
}
