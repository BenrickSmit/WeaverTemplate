# --------------------------------------------------------------------------
# SECTION: Application Setup
# --------------------------------------------------------------------------

set(NAME "${PROJECT_NAME}")

# Define the application executable based on the platform.
# On Windows, it includes the icon resource file.
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 WeaverInterface.cpp icon.rc)
else()
    add_executable(${PROJECT_NAME} WeaverInterface.cpp)
endif()

# include(${PROJECT_SOURCE_DIR}/src/app/cmake/AppAssets.cmake)
# include(${PROJECT_SOURCE_DIR}/src/app/cmake/Packaging.cmake)

# Copy SDL2.dll to the target application's build folder on Windows.
if (WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2::SDL2>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif ()

# Find Vulkan package, required for Dear ImGui.
find_package(Vulkan REQUIRED)

# Set include directories for the application.
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/_deps/absl-src)

# Set C++ standard for the application.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_${PROJECT_CPP_VERSION})

# Link necessary libraries to the application.
target_link_libraries(${PROJECT_NAME} PRIVATE 
glm::glm
SDL2::SDL2
${PROJECT_NAME}Core
imgui
Vulkan::Vulkan
# quill::quill
)

# Copy assets directory to the output directory. This is crucial for the application to find
# runtime resources like fonts. The source code loads assets assuming they are in a folder
# named "assets" located in the same directory as the executable.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets
  $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

# --------------------------------------------------------------------------
# SECTION: Installation Rules
# --------------------------------------------------------------------------

# Install the application executable.
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Install Linux-specific resources (icon and .desktop file).
if (UNIX)
    install(FILES "${CMAKE_SOURCE_DIR}/assets/icons/BaseAppIcon.png"
            DESTINATION "share/icons/hicolor/scalable/apps"
            RENAME "${PROJECT_NAME}.png")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.desktop.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop"
        @ONLY
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop"
            DESTINATION "share/applications")
endif()