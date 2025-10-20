/**
 * @file Random.h
 * @author B.G. Smit
 * @brief Declares a utility class for generating random numbers.
 *
 * This file defines the `Random` class, which provides static methods for
 * initializing a random number generator and producing various types of random
 * values, including unsigned integers, floats, and 3D vectors. It is useful
 * for simulations, procedural content generation, and other stochastic processes.
 * @copyright Copyright (c) 2025
 */
#ifndef RANDOM_H
#define RANDOM_H

#pragma once

#include <glm/glm.hpp>
#include <limits>
#include <random>

namespace Weaver {

/**
 * @class Random
 * @brief A utility class for generating pseudo-random numbers.
 */
class Random {
 public:
  /**
   * @brief Initializes the random number generator with a seed from `std::random_device`.
   */
  static void Init() {
    s_RandomEngine.seed(std::random_device()());
  }

  /**
   * @brief Generates a random unsigned integer.
   * @return A random unsigned integer.
   */
  static uint32_t UInt() {
    return s_Distribution(s_RandomEngine);
  }

  /**
   * @brief Generates a random unsigned integer within a specified range.
   * @param min The minimum value (inclusive).
   * @param max The maximum value (inclusive).
   * @return A random unsigned integer within the specified range.
   */
  static uint32_t UInt(uint32_t min, uint32_t max) {
    return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
  }

  /**
   * @brief Generates a random float between 0.0f and 1.0f.
   * @return A random float.
   */
  static float Float() {
    return (float)s_Distribution(s_RandomEngine) / UINT32_MAX;
  }

  /**
   * @brief Generates a random 3D vector with components between 0.0f and 1.0f.
   * @return A random 3D vector.
   */
  static glm::vec3 Vec3() {
    return glm::vec3(Float(), Float(), Float());
  }

  /**
   * @brief Generates a random 3D vector with components within a specified range.
   * @param min The minimum value for each component.
   * @param max The maximum value for each component.
   * @return A random 3D vector within the specified range.
   */
  static glm::vec3 Vec3(float min, float max) {
    return glm::vec3(
        Float() * (max - min) + min, Float() * (max - min) + min, Float() * (max - min) + min);
  }

  /**
   * @brief Generates a random 3D vector within a unit sphere.
   * @return A random 3D vector within a unit sphere.
   */
  static glm::vec3 InUnitSphere() {
    return glm::normalize(Vec3(-1.0f, 1.0f));
  }

 private:
  thread_local static std::mt19937 s_RandomEngine;
  static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

}  // namespace Weaver

#endif