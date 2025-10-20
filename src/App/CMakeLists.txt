set(NAME "App")

if (WIN32)
    add_executable(${NAME} WIN32 WeaverInterface.cpp)
else()
    add_executable(${NAME} WeaverInterface.cpp)
endif()

# include(${PROJECT_SOURCE_DIR}/src/app/cmake/AppAssets.cmake)
# include(${PROJECT_SOURCE_DIR}/src/app/cmake/Packaging.cmake)

if (WIN32)
  # Copy DLL files on Windows to the target App build folder.
  add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2::SDL2>
    $<TARGET_FILE_DIR:${NAME}>)
endif ()

find_package(Vulkan REQUIRED)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_features(${NAME} PRIVATE cxx_std_${PROJECT_CPP_VERSION})
target_link_libraries(${NAME} PRIVATE 
glm::glm
SDL2::SDL2
Core
imgui
Vulkan::Vulkan
# quill::quill
)

# Copy assets directory to the output directory. This is crucial for the application to find
# runtime resources like fonts. The source code loads assets assuming they are in a folder
# named "assets" located in the same directory as the executable.
add_custom_command(TARGET ${NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets
  $<TARGET_FILE_DIR:${NAME}>/assets)


install(TARGETS ${NAME}
    RUNTIME DESTINATION bin
)