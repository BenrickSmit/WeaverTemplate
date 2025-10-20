/**
 * @file Random.cpp
 * @author B.G. Smit
 * @brief Implements a utility class for generating random numbers.
 *
 * This file provides the implementation for the `Random` class, offering
 * static methods for generating pseudo-random numbers using a Mersenne Twister engine.
 * It is designed for various applications requiring random data, such as simulations or procedural
 * generation.
 * @copyright Copyright (c) 2025
 */
#include "Random.h"
#include <cmath>
using namespace std;

#define GLM_ENABLE_EXPERIMENTAL

namespace Weaver {

/**
 * @brief Thread-local random engine.
 */
thread_local std::mt19937 Random::s_RandomEngine;
/**
 * @brief Uniform integer distribution.
 */
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

}  // namespace Weaver