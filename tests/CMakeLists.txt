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
    ${PROJECT_NAME}Tests
    ${TEST_SOURCES}
)

set_property(TARGET ${PROJECT_NAME}Tests PROPERTY CTEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

target_include_directories(${PROJECT_NAME}Tests PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
    ${PROJECT_NAME}Tests
    GTest::gtest_main
    ${PROJECT_NAME}Core
)

# --------------------------------------------------------------------------
# SECTION: Test Discovery
# --------------------------------------------------------------------------
# This section automatically discovers and adds tests to the test suite.

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}Tests)
