# --------------------------------------------------------------------------
# SECTION: Test Executable
# --------------------------------------------------------------------------
# This section defines the test executable and links the necessary
# testing framework.

# For ease of test creation, we automatically glob all .cpp files in this
# directory. This is a departure from the manual file listing in Core and App,
# which is intentional to maintain rigidity and code quality in the main
# application logic.
file(GLOB TEST_SOURCES "*.cpp")

add_executable(
    WeaverTests
    ${TEST_SOURCES}
)

target_include_directories(WeaverTests PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
    WeaverTests
    GTest::gtest_main
    Core
)

# --------------------------------------------------------------------------
# SECTION: Test Discovery
# --------------------------------------------------------------------------
# This section automatically discovers and adds tests to the test suite.

include(GoogleTest)
gtest_discover_tests(WeaverTests)
