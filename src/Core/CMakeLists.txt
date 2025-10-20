# --------------------------------------------------------------------------
# SECTION: Core Library Definition
# --------------------------------------------------------------------------
# This section defines the Core library, which contains the essential
# functionalities of the application.

set(NAME "Core")

add_library(${NAME} STATIC
  "Canvas.cpp"
  "Canvas.h"
  "EntryPoint.cpp"
  "EntryPoint.h"
  "Image.h"
  "Image.cpp"
  "Layer.h"
  "Random.cpp"
  "Random.h"
  "Timer.h"
  "Themes.cpp"
  "Log.cpp"
  "Log.h"
  "FunctionPreprocessor.h"
  "MathTest.h"


  "Input/Input.cpp"
  "Input/Input.h"
  "Input/KeyCodes.h"

  "stb_image/stb_image.h"
   )

# --------------------------------------------------------------------------
# SECTION: Dependencies
# --------------------------------------------------------------------------
# Find and link required dependencies for the Core library.

find_package(Vulkan REQUIRED)

# --------------------------------------------------------------------------
# SECTION: Build Configuration
# --------------------------------------------------------------------------
# Configure include directories and link libraries for the Core library.

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${NAME}
  PUBLIC 
  glm::glm
  SDL2::SDL2 
  imgui
  Vulkan::Vulkan
  absl::log
  absl::log_initialize
  absl::flags_parse
  absl::flags_config
  absl::time
  )