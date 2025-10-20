/**
 * @file Timer.h
 * @author B.G. Smit
 * @brief Declares utility classes for timing operations.
 *
 * This file defines the `Timer` class for measuring elapsed time and the `ScopedTimer`
 * class for convenient, scope-based timing and logging of code execution durations.
 * These utilities are essential for performance profiling and debugging.
 * @copyright Copyright (c) 2025
 */
#ifndef TIMER_H
#define TIMER_H
#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <chrono>
#include <iostream>
#include <string>

namespace Weaver {

/**
 * @class Timer
 * @brief A simple timer class for measuring elapsed time.
 */
class Timer {
 public:
  /**
   * @brief Constructs a new Timer object and resets it.
   */
  Timer() {
    reset();
  }

  /**
   * @brief Resets the timer.
   */
  void reset() {
    m_starting_timer = std::chrono::high_resolution_clock::now();
  }

  /**
   * @brief Gets the elapsed time in seconds.
   * @return The elapsed time in seconds.
   */
  float elapsed() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::high_resolution_clock::now() - m_starting_timer)
               .count() *
           0.001f * 0.001f * 0.001f;
  }

  /**
   * @brief Gets the elapsed time in milliseconds.
   * @return The elapsed time in milliseconds.
   */
  float elapsed_ms() {
    return elapsed() * 1000.0f;
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_starting_timer;
};

/**
 * @class ScopedTimer
 * @brief A scoped timer that logs the elapsed time when it goes out of scope.
 */
class ScopedTimer {
 public:
  /**
   * @brief Constructs a new ScopedTimer object.
   * @param name The name of the timer.
   */
  ScopedTimer(const std::string& name) : m_scoped_timer_name(name) {}
  /**
   * @brief Destroys the ScopedTimer object and logs the elapsed time.
   */
  ~ScopedTimer() {
    float time = m_scoped_timer.elapsed_ms();
    std::cout << "[TIMER] " << m_scoped_timer_name << " - " << time << "ms\n";
  }

 private:
  std::string m_scoped_timer_name;
  Timer m_scoped_timer;
};

}  // namespace Weaver

#endif