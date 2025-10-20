# --------------------------------------------------------------------------
# SECTION: Dependency Management
# --------------------------------------------------------------------------
# This file handles the fetching and configuration of all third-party
# dependencies (vendors) required by the project.

include(FetchContent)

# --------------------------------------------------------------------------
# SECTION: Dependency Declarations
# --------------------------------------------------------------------------
# Declaration of all external libraries (vendors) using FetchContent.

# Dear ImGUI: A graphical user interface library for C++.
FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG 368123ab06b2b573d585e52f84cd782c5c006697 # Branch: docking, date: 07.11.2024, 07:58 GMT+1
)

# SDL2: A cross-platform development library for low-level access to hardware.
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-2.30.9
)

# GLM: OpenGL Mathematics, a header-only C++ mathematics library.
FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG 1.0.1
)

# GTest: Google's C++ testing framework.
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Abseil: A collection of C++ library code designed to supplement the C++ standard library.
FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20240116.2
)

# --------------------------------------------------------------------------
# SECTION: Dependency Configuration
# --------------------------------------------------------------------------
# Configuration settings for the fetched dependencies.

# For Windows: Prevent gtest from overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Enable testing capabilities for the project.
enable_testing()

# Set CMake policies for dependencies.
# For SDL2 to be able to override options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Configure SDL2 build options.
set(SDL2_DISABLE_SDL2MAIN ON)  # Prevent SDL2 from adding SDL_main
set(SDL2_BUILD_SHARED_LIBS ON)  # Build SDL2 as a shared library
set(SDL2_INSTALL ON)  # Ensure SDL2 configures for installation/export

# Include the setup for Dear ImGui.
add_subdirectory(imgui-setup)

# --------------------------------------------------------------------------
# SECTION: Make Dependencies Available
# --------------------------------------------------------------------------
# Make the fetched content available to the project.

FetchContent_MakeAvailable(imgui SDL2 glm googletest absl)

# Output source and binary directories for debugging purposes.
message(STATUS ">> SDL2 source directory: ${SDL2_SOURCE_DIR}")
message(STATUS ">> SDL2 binary directory: ${SDL2_BINARY_DIR}")
