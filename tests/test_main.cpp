/**
 * @file test_main.cpp
 * @author B.G. Smit
 * @brief Main entry point for the Google Test framework.
 *
 * This file contains the main function to run all defined Google Tests.
 * It also includes an example test case to demonstrate basic testing functionality.
 * @copyright Copyright (c) 2025
 */
#include <gtest/gtest.h>

/**
 * @brief Example test case that always passes.
 * @details This test serves as a basic demonstration of the Google Test framework setup.
 */
TEST(ExampleTests, AlwaysPasses) {
    EXPECT_TRUE(true);
}
