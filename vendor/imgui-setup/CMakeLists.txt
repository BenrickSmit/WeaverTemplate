
# --------------------------------------------------------------------------
# SECTION: Project Setup
# --------------------------------------------------------------------------
# Dear ImGui needs to be set up in a special way in this engine-style 
# architecture. This ensures it is available as needed in the /Core anad /App
FetchContent_MakeAvailable(imgui)

add_library(imgui
  ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui.h
  ${imgui_SOURCE_DIR}/imconfig.h ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_internal.h
  ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imstb_rectpack.h ${imgui_SOURCE_DIR}/imstb_textedit.h
  ${imgui_SOURCE_DIR}/imstb_truetype.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h 
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h 
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
  )

# --------------------------------------------------------------------------
# SECTION: Build Configuration Setup
# --------------------------------------------------------------------------


find_package(Vulkan REQUIRED)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_compile_features(imgui PRIVATE cxx_std_${PROJECT_CPP_VERSION})
target_link_libraries(imgui PUBLIC SDL2::SDL2 Vulkan::Vulkan)
