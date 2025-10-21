set(NAME "${PROJECT_NAME}")

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 WeaverInterface.cpp icon.rc)
else()
    add_executable(${PROJECT_NAME} WeaverInterface.cpp)
endif()

# include(${PROJECT_SOURCE_DIR}/src/app/cmake/AppAssets.cmake)
# include(${PROJECT_SOURCE_DIR}/src/app/cmake/Packaging.cmake)

if (WIN32)
  # Copy DLL files on Windows to the target App build folder.
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2::SDL2>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif ()

find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/_deps/absl-src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_${PROJECT_CPP_VERSION})
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


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

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