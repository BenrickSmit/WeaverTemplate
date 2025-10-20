/**
 * @file test_math.cpp
 * @author B.G. Smit
 * @brief Unit tests for the math functions.
 * @copyright Copyright (c) 2025
 */
#include <gtest/gtest.h>

#include "Core/MathTest.h"

/**
 * @brief Tests the Add function.
 */
TEST(MathTest, AddInts) {
  EXPECT_EQ(Weaver::AddInts(2, 2), 4);
}
