#include "Canvas.h"

#include "Log.h"
#include "Themes.h"

//
// Adapted from Dear ImGui Vulkan example
//

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important note to the reader who wish to integrate imgui_impl_vulkan.cpp/.h in their own
// engine/app.
// - Common ImGui_ImplVulkan_XXX functions and structures are used to interface with
// imgui_impl_vulkan.cpp/.h.
//   You will use those if you want to use this rendering backend in your engine/app.
// - Helper ImGui_ImplVulkanH_XXX functions and structures are only used by this example (main.cpp)
// and by
//   the backend itself (imgui_impl_vulkan.cpp), but should PROBABLY NOT be used by your own
//   engine/app code.
// Read comments in imgui_impl_vulkan.h.

#include <SDL.h>
#include <SDL_shape.h>
#include <SDL_vulkan.h>
#include <stdio.h>   // printf, fprintf
#include <stdlib.h>  // abort
#include <vulkan/vulkan.h>

#include <glm/glm.hpp>
#include <iostream>

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_vulkan.h"
#include "imgui.h"

#define SHAPED_WINDOW_X 150
#define SHAPED_WINDOW_Y 150
#define SHAPED_WINDOW_DIMENSION 640

#define GLM_ENABLE_EXPERIMENTAL

// Volk headers
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
#define VOLK_IMPLEMENTATION
#include <volk.h>
#endif

#include <Windows.h>

// #define APP_USE_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define APP_USE_VULKAN_DEBUG_REPORT
#endif

extern bool g_CanvasRunning;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of
// testing and compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+
// requires linking with legacy_stdio_definitions.lib, which we do using this pragma. Your own
// project should not be affected, as you are likely to link with a newer binary of GLFW that is
// adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// #define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

// Data
static VkAllocationCallbacks* g_Allocator = nullptr;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t)-1;
static VkQueue g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;

uint32_t g_CommandBufferSize = 1000;

static ImGui_ImplVulkanH_Window g_MainWindowData;
static uint32_t g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;

// Per-frame-in-flight
static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)
static uint32_t s_CurrentFrameIndex = 0;

static Weaver::Canvas* s_Instance = nullptr;

static void DrawFilledCircle(SDL_Surface* surface, int x, int y, int radius, Uint32 color) {
  for (int dy = -radius; dy <= radius; dy++) {
    for (int dx = -radius; dx <= radius; dx++) {
      if (dx * dx + dy * dy <= radius * radius) {
        SDL_Rect rect = {x + dx, y + dy, 1, 1};
        SDL_FillRect(surface, &rect, color);
      }
    }
  }
}

static void FillRoundedRect(SDL_Surface* surface, SDL_Rect* rect, int radius, Uint32 color) {
  // Draw the four corner circles
  DrawFilledCircle(surface, rect->x + radius, rect->y + radius, radius, color);
  DrawFilledCircle(surface, rect->x + rect->w - radius - 1, rect->y + radius, radius, color);
  DrawFilledCircle(surface, rect->x + radius, rect->y + rect->h - radius - 1, radius, color);
  DrawFilledCircle(
      surface, rect->x + rect->w - radius - 1, rect->y + rect->h - radius - 1, radius, color);

  // Draw the two connecting rectangles
  SDL_Rect r = {rect->x + radius, rect->y, rect->w - 2 * radius, rect->h};
  SDL_FillRect(surface, &r, color);
  r = {rect->x, rect->y + radius, rect->w, rect->h - 2 * radius};
  SDL_FillRect(surface, &r, color);
}

void check_vk_result(VkResult err) {
  // TODO: Update to get the actual Vulkan Errors
  if (err == 0)
    return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0)
    abort();
}

#ifdef APP_USE_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t object,
    size_t location,
    int32_t messageCode,
    const char* pLayerPrefix,
    const char* pMessage,
    void* pUserData) {
  (void)flags;
  (void)object;
  (void)location;
  (void)messageCode;
  (void)pUserData;
  (void)pLayerPrefix;  // Unused arguments
  fprintf(
      stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
  return VK_FALSE;
}
#endif  // APP_USE_VULKAN_DEBUG_REPORT

static bool IsExtensionAvailable(
    const ImVector<VkExtensionProperties>& properties, const char* extension) {
  for (const VkExtensionProperties& p : properties)
    if (strcmp(p.extensionName, extension) == 0)
      return true;
  return false;
}

static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice() {
  uint32_t gpu_count;
  VkResult err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
  check_vk_result(err);
  IM_ASSERT(gpu_count > 0);

  ImVector<VkPhysicalDevice> gpus;
  gpus.resize(gpu_count);
  err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.Data);
  check_vk_result(err);

  // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available.
  // This covers most common cases (multi-gpu/integrated+dedicated graphics). Handling more
  // complicated setups (multiple dedicated GPUs) is out of scope of this sample.
  for (VkPhysicalDevice& device : gpus) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      return device;
  }

  // Use first GPU (Integrated) is a Discrete one is not available.
  if (gpu_count > 0)
    return gpus[0];
  return VK_NULL_HANDLE;
}

static void SetupVulkan(ImVector<const char*> instance_extensions) {
  VkResult err;
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
  volkInitialize();
#endif

  // Create Vulkan Instance
  {
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Enumerate available extensions
    uint32_t properties_count;
    ImVector<VkExtensionProperties> properties;
    vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
    properties.resize(properties_count);
    err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
    check_vk_result(err);

    // Enable required extensions
    if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
      instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
    if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
      instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
#endif

    // Enabling validation layers
#ifdef APP_USE_VULKAN_DEBUG_REPORT
    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = layers;
    instance_extensions.push_back("VK_EXT_debug_report");
#endif

    // Create Vulkan Instance
    create_info.enabledExtensionCount = (uint32_t)instance_extensions.Size;
    create_info.ppEnabledExtensionNames = instance_extensions.Data;
    err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
    check_vk_result(err);
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
    volkLoadInstance(g_Instance);
#endif

    // Setup the debug report callback
#ifdef APP_USE_VULKAN_DEBUG_REPORT
    auto f_vkCreateDebugReportCallbackEXT =
        (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
            g_Instance, "vkCreateDebugReportCallbackEXT");
    IM_ASSERT(f_vkCreateDebugReportCallbackEXT != nullptr);
    VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
    debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
                            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    debug_report_ci.pfnCallback = debug_report;
    debug_report_ci.pUserData = nullptr;
    err =
        f_vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
    check_vk_result(err);
#endif
  }

  // Select Physical Device (GPU)
  g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

  // Select graphics queue family
  {
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, nullptr);
    VkQueueFamilyProperties* queues =
        (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
    for (uint32_t i = 0; i < count; i++)
      if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        g_QueueFamily = i;
        break;
      }
    free(queues);
    IM_ASSERT(g_QueueFamily != (uint32_t)-1);
  }

  // Create Logical Device (with 1 queue)
  {
    ImVector<const char*> device_extensions;
    device_extensions.push_back("VK_KHR_swapchain");

    // Enumerate physical device extension
    uint32_t properties_count;
    ImVector<VkExtensionProperties> properties;
    vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, nullptr);
    properties.resize(properties_count);
    vkEnumerateDeviceExtensionProperties(
        g_PhysicalDevice, nullptr, &properties_count, properties.Data);
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
      device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

    const float queue_priority[] = {1.0f};
    VkDeviceQueueCreateInfo queue_info[1] = {};
    queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info[0].queueFamilyIndex = g_QueueFamily;
    queue_info[0].queueCount = 1;
    queue_info[0].pQueuePriorities = queue_priority;
    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
    create_info.pQueueCreateInfos = queue_info;
    create_info.enabledExtensionCount = (uint32_t)device_extensions.Size;
    create_info.ppEnabledExtensionNames = device_extensions.Data;
    err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
    check_vk_result(err);
    vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
  }

  // Create Descriptor Pool
  // The example only requires a single combined image sampler descriptor for the font image and
  // only uses one descriptor set (for that) If you wish to load e.g. additional textures you may
  // need to alter pools sizes.
  {
    // VkDescriptorPoolSize pool_sizes[] = {
    //     {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1},
    // };
    // VkDescriptorPoolCreateInfo pool_info = {};
    // pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    // pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    // pool_info.maxSets = 1;
    // pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    // pool_info.pPoolSizes = pool_sizes;
    VkDescriptorPoolSize pool_sizes[] =
        // {
        //     {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100},
        //     {VK_DESCRIPTOR_TYPE_SAMPLER, 100},
        //     {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100},
        // };
        {{VK_DESCRIPTOR_TYPE_SAMPLER, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, g_CommandBufferSize},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, g_CommandBufferSize}};
    // VkDescriptorPoolCreateInfo pool_info = {};
    // pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    // pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    // pool_info.maxSets = 100;  // Increase to support multiple sets
    // pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    // pool_info.pPoolSizes = pool_sizes;
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = g_CommandBufferSize;
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
    check_vk_result(err);
  }
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(
    ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height) {
  wd->Surface = surface;

  // Check for WSI support
  VkBool32 res;
  vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
  if (res != VK_TRUE) {
    fprintf(stderr, "Error no WSI support on physical device 0\n");
    exit(-1);
  }

  // Select Surface Format
  const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM,
      VK_FORMAT_R8G8B8A8_UNORM,
      VK_FORMAT_B8G8R8_UNORM,
      VK_FORMAT_R8G8B8_UNORM};
  const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice,
      wd->Surface,
      requestSurfaceImageFormat,
      (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
      requestSurfaceColorSpace);

  // Select Present Mode
#ifdef APP_UNLIMITED_FRAME_RATE
  VkPresentModeKHR present_modes[] = {
      VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
  wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
      g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
  // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

  // Create SwapChain, RenderPass, Framebuffer, etc.
  IM_ASSERT(g_MinImageCount >= 2);
  ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance,
      g_PhysicalDevice,
      g_Device,
      wd,
      g_QueueFamily,
      g_Allocator,
      width,
      height,
      g_MinImageCount);
}

static void CleanupVulkan() {
  vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef APP_USE_VULKAN_DEBUG_REPORT
  // Remove the debug report callback
  auto f_vkDestroyDebugReportCallbackEXT =
      (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
          g_Instance, "vkDestroyDebugReportCallbackEXT");
  f_vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif  // APP_USE_VULKAN_DEBUG_REPORT

  vkDestroyDevice(g_Device, g_Allocator);
  vkDestroyInstance(g_Instance, g_Allocator);
}

static void CleanupVulkanWindow() {
  ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);
}

static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data) {
  VkResult err;

  VkSemaphore image_acquired_semaphore =
      wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
  VkSemaphore render_complete_semaphore =
      wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  err = vkAcquireNextImageKHR(g_Device,
      wd->Swapchain,
      UINT64_MAX,
      image_acquired_semaphore,
      VK_NULL_HANDLE,
      &wd->FrameIndex);
  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    g_SwapChainRebuild = true;
    return;
  }
  check_vk_result(err);

  ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
  {
    err = vkWaitForFences(g_Device,
        1,
        &fd->Fence,
        VK_TRUE,
        UINT64_MAX);  // wait indefinitely instead of periodically checking
    check_vk_result(err);

    err = vkResetFences(g_Device, 1, &fd->Fence);
    check_vk_result(err);
  }
  {
    err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
    check_vk_result(err);
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
    check_vk_result(err);
  }
  {
    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = wd->RenderPass;
    info.framebuffer = fd->Framebuffer;
    info.renderArea.extent.width = wd->Width;
    info.renderArea.extent.height = wd->Height;
    info.clearValueCount = 1;
    info.pClearValues = &wd->ClearValue;
    vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
  }

  // Record dear imgui primitives into command buffer
  ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

  // Submit command buffer
  vkCmdEndRenderPass(fd->CommandBuffer);
  {
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &image_acquired_semaphore;
    info.pWaitDstStageMask = &wait_stage;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &fd->CommandBuffer;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = &render_complete_semaphore;

    err = vkEndCommandBuffer(fd->CommandBuffer);
    check_vk_result(err);
    err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
    check_vk_result(err);
  }
}

static void FramePresent(ImGui_ImplVulkanH_Window* wd) {
  if (g_SwapChainRebuild)
    return;
  VkSemaphore render_complete_semaphore =
      wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  VkPresentInfoKHR info = {};
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.waitSemaphoreCount = 1;
  info.pWaitSemaphores = &render_complete_semaphore;
  info.swapchainCount = 1;
  info.pSwapchains = &wd->Swapchain;
  info.pImageIndices = &wd->FrameIndex;
  VkResult err = vkQueuePresentKHR(g_Queue, &info);
  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    g_SwapChainRebuild = true;
    return;
  }
  check_vk_result(err);
  wd->SemaphoreIndex =
      (wd->SemaphoreIndex + 1) % wd->SemaphoreCount;  // Now we can use the next set of semaphores
}

namespace Weaver {

Canvas::Canvas(CanvasSpecification& specification) : m_Specification(specification) {
  s_Instance = this;

  Init();
}

Canvas ::~Canvas() {
  Shutdown();

  s_Instance = nullptr;
}

Canvas& Canvas ::Get() {
  return *s_Instance;
}

void Canvas::SetWindowShape() {
  SDL_Surface* shape = SDL_CreateRGBSurfaceWithFormat(

      0, m_Specification.Width, m_Specification.Height, 32, SDL_PIXELFORMAT_RGBA32);

  if (shape) {
    SDL_SetSurfaceBlendMode(shape, SDL_BLENDMODE_BLEND);

    SDL_FillRect(

        shape,
        NULL,

        SDL_MapRGBA(shape->format, 0, 0, 0, 0));  // Transparent background

    SDL_Rect rect = {
        0, 0, static_cast<int>(m_Specification.Width), static_cast<int>(m_Specification.Height)};
    FillRoundedRect(
        shape, &rect, m_Specification.CornerRadius, SDL_MapRGBA(shape->format, 255, 255, 255, 255));

    SDL_WindowShapeMode mode;
    mode.mode = ShapeModeBinarizeAlpha;
    mode.parameters.binarizationCutoff = 255;

    SDL_SetWindowShape(m_WindowHandle, shape, &mode);

    SDL_FreeSurface(shape);
  }
}

void Canvas::Init() {
  // Setup SDL
  WEAVER_LOG_INFO("Initializing SDL...");
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    WEAVER_LOG_FATAL("Failed to initialize SDL: %s", SDL_GetError());
    return;
  }
  WEAVER_LOG_INFO("SDL initialized successfully.");

  // From 2.0.18: Enable native IME.
  WEAVER_LOG_INFO("Creating SDL shaped window...");
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return;
  }

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Create window with Vulkan graphics context
  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI |
                        SDL_WINDOW_BORDERLESS);

  // SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+Vulkan example", SDL_WINDOWPOS_CENTERED,
  // SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  m_WindowHandle = SDL_CreateShapedWindow(m_Specification.Name.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      m_Specification.Width,
      m_Specification.Height,
      window_flags);
  if (m_WindowHandle == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    WEAVER_LOG_FATAL("Failed to create SDL shaped window: %s", SDL_GetError());
    return;
  }
  WEAVER_LOG_INFO("SDL shaped window created successfully.");

  SetWindowShape();

  WEAVER_LOG_INFO("Retrieving Vulkan instance extensions...");
  ImVector<const char*> extensions;
  uint32_t extensions_count = 0;
  SDL_Vulkan_GetInstanceExtensions(m_WindowHandle, &extensions_count, nullptr);
  extensions.resize(extensions_count);
  SDL_Vulkan_GetInstanceExtensions(m_WindowHandle, &extensions_count, extensions.Data);
  WEAVER_LOG_INFO("Vulkan instance extensions retrieved. Calling SetupVulkan...");
  SetupVulkan(extensions);
  WEAVER_LOG_INFO("SetupVulkan completed.");

  // Create Window Surface
  WEAVER_LOG_INFO("Creating Vulkan surface...");
  VkSurfaceKHR surface;
  VkResult err;
  if (SDL_Vulkan_CreateSurface(m_WindowHandle, g_Instance, &surface) == 0) {
    printf("Failed to create Vulkan surface.\n");
    WEAVER_LOG_FATAL("Failed to create Vulkan surface.");
    return;
  }
  WEAVER_LOG_INFO("Vulkan surface created successfully.");

  WEAVER_LOG_INFO("Setting up Vulkan window...");
  // Create Framebuffers
  int w, h;
  SDL_GetWindowSize(m_WindowHandle, &w, &h);
  ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
  SetupVulkanWindow(wd, surface, w, h);
  WEAVER_LOG_INFO("Vulkan window setup completed.");

  s_AllocatedCommandBuffers.resize(wd->ImageCount);
  s_ResourceFreeQueue.resize(wd->ImageCount);

  // Setup Dear ImGui context
  WEAVER_LOG_INFO("Creating ImGui context...");
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  WEAVER_LOG_INFO("ImGui context created.");
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

  // Setup Dear ImGui style
  //   ImGui::StyleColorsDark();
  DarkMaterialTheme();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  WEAVER_LOG_INFO("Initializing ImGui SDL2 for Vulkan...");
  ImGui_ImplSDL2_InitForVulkan(m_WindowHandle);
  WEAVER_LOG_INFO("ImGui SDL2 for Vulkan initialized.");
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = g_Instance;
  init_info.PhysicalDevice = g_PhysicalDevice;
  init_info.Device = g_Device;
  init_info.QueueFamily = g_QueueFamily;
  init_info.Queue = g_Queue;
  init_info.PipelineCache = g_PipelineCache;
  init_info.DescriptorPool = g_DescriptorPool;
  init_info.RenderPass = wd->RenderPass;
  init_info.Subpass = 0;
  init_info.MinImageCount = g_MinImageCount;
  init_info.ImageCount = wd->ImageCount;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = g_Allocator;
  init_info.CheckVkResultFn = check_vk_result;
  WEAVER_LOG_INFO("Initializing ImGui Vulkan backend...");
  ImGui_ImplVulkan_Init(&init_info);
  WEAVER_LOG_INFO("ImGui Vulkan backend initialized.");

  // Load Fonts
  // The font files are loaded from the "assets" directory. This path is relative to the
  // executable. The `assets` directory is copied into the same directory as the executable by a
  // post-build command in the `src/App/CMakeLists.txt` file.
  WEAVER_LOG_INFO(
      "Loading Roboto Mono font from: assets/fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf");
  ImFont* robotoFont = io.Fonts->AddFontFromFileTTF(
      "assets/fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf", 16.0f);
  if (robotoFont == nullptr) {
    WEAVER_LOG_FATAL("Failed to load Roboto Mono font!");
    abort();
  }
  WEAVER_LOG_INFO("Roboto Mono font loaded successfully.");

  ImFontConfig config;
  config.MergeMode = true;
  config.PixelSnapH = true;

#include "IconsMaterialDesign.h"
  static const ImWchar icon_ranges[] = {ICON_MIN_MD, ICON_MAX_MD, 0};

  WEAVER_LOG_INFO(
      "Loading Material Symbols font from: "
      "assets/fonts/Material_Symbols/Material_Symbols_Rounded/"
      "MaterialSymbolsRounded-VariableFont_FILL,GRAD,opsz,wght.ttf");
  ImFont* materialSymbolsFont = io.Fonts->AddFontFromFileTTF(
      "assets/fonts/Material_Symbols/Material_Symbols_Rounded/"
      "MaterialSymbolsRounded-VariableFont_FILL,GRAD,opsz,wght.ttf",
      17.0f,
      &config,
      icon_ranges);
  if (materialSymbolsFont == nullptr) {
    WEAVER_LOG_FATAL("Failed to load Material Symbols font!");
    abort();
  }
  WEAVER_LOG_INFO("Material Symbols font loaded successfully.");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr,
  // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr); Load default font ImFontConfig
  // fontConfig; fontConfig.FontDataOwnedByAtlas = false; ImFont *robotoFont =
  // io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f,
  // &fontConfig);
  // io.FontDefault = robotoFont;
}

void Canvas::Shutdown() {
  for (auto& layer : m_LayerStack)
    layer->OnDetach();

  m_LayerStack.clear();

  // Cleanup
  VkResult err = vkDeviceWaitIdle(g_Device);
  check_vk_result(err);

  // Free resources in queue
  for (auto& queue : s_ResourceFreeQueue) {
    for (auto& func : queue)
      func();
  }
  s_ResourceFreeQueue.clear();

  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  CleanupVulkanWindow();
  CleanupVulkan();

  SDL_DestroyWindow(m_WindowHandle);
  SDL_Quit();

  g_CanvasRunning = false;
}

void Canvas::Run() {
  m_Running = true;

  ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImGuiIO& io = ImGui::GetIO();

  // New Main Loop
  bool done = false;
  while (!done && m_Running) {
    // Poll and handle events (inputs, window resize, etc.)
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(m_WindowHandle))
          done = true;
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
          g_SwapChainRebuild = true;
        if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
          for (auto& layer : m_LayerStack)
            layer->OnMinimize();
        }
        if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
          for (auto& layer : m_LayerStack)
            layer->OnMaximize();
        }
        if (event.window.event == SDL_WINDOWEVENT_RESTORED) {
          for (auto& layer : m_LayerStack)
            layer->OnRestored();
        }
      }
    }

    for (auto& layer : m_LayerStack)
      layer->OnUpdate(m_TimeStep);

    // Resize swap chain?
    if (g_SwapChainRebuild) {
      int width, height;
      SDL_GetWindowSize(m_WindowHandle, &width, &height);

      if (width > 0 && height > 0) {
        m_Specification.Width = width;
        m_Specification.Height = height;
        WEAVER_LOG_INFO("Setting window shape...");
        SetWindowShape();
        WEAVER_LOG_INFO("Window shape set successfully.");

        ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance,
            g_PhysicalDevice,
            g_Device,
            &g_MainWindowData,
            g_QueueFamily,
            g_Allocator,
            width,
            height,
            g_MinImageCount);
        g_MainWindowData.FrameIndex = 0;

        // Notify layers of resize
        for (auto& layer : m_LayerStack) {
          layer->OnResize(width, height);
        }

        s_AllocatedCommandBuffers.clear();
        s_AllocatedCommandBuffers.resize(g_MainWindowData.ImageCount);
        s_ResourceFreeQueue.clear();
        s_ResourceFreeQueue.resize(g_MainWindowData.ImageCount);

        vkDeviceWaitIdle(g_Device);
        g_SwapChainRebuild = false;
      }
    }

    if (m_restore_in_progress && !g_SwapChainRebuild) {
      for (auto& layer : m_LayerStack)
        layer->OnRestored();
      m_restore_in_progress = false;
    }

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
      static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

      ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
      if (m_MenubarCallback)
        window_flags |= ImGuiWindowFlags_MenuBar;

      const ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, ImGui::GetStyle().ChildRounding);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("DockSpace Demo", nullptr, window_flags);
      ImGui::PopStyleVar();

      ImGui::PopStyleVar(2);

      ImGuiIO& io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("VulkanAppDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
      }

      if (m_MenubarCallback) {
        if (ImGui::BeginMenuBar()) {
          m_MenubarCallback();
          if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            int x, y;
            SDL_GetWindowPosition(m_WindowHandle, &x, &y);
            ImGuiIO& io = ImGui::GetIO();
            SDL_SetWindowPosition(m_WindowHandle, x + io.MouseDelta.x, y + io.MouseDelta.y);
          }
          ImGui::EndMenuBar();
        }
      }

      for (auto& layer : m_LayerStack)
        layer->OnUIRender();

      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImDrawData* main_draw_data = ImGui::GetDrawData();
    const bool main_is_minimized =
        (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
    wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
    wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
    wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
    wd->ClearValue.color.float32[3] = clear_color.w;
    if (!main_is_minimized)
      FrameRender(wd, main_draw_data);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }

    if (!main_is_minimized)
      FramePresent(wd);

    float time = GetTime();
    m_FrameTime = time - m_LastFrameTime;
    m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
    m_LastFrameTime = time;
  }
}

void Canvas::Close() {
  m_Running = false;
}

float Canvas::GetTime() {
  return (float)SDL_GetTicks();
}

void Canvas::Minimize() {
  SDL_MinimizeWindow(m_WindowHandle);
}

void Canvas::ToggleMaximize() {
  if (m_IsMaximized) {
    // Restore
    SDL_SetWindowPosition(m_WindowHandle, m_SavedWindowRect.x, m_SavedWindowRect.y);
    SDL_SetWindowSize(m_WindowHandle, m_SavedWindowRect.w, m_SavedWindowRect.h);
    m_IsMaximized = false;
    m_restore_in_progress = true;
  } else {
    // Maximize
    for (auto& layer : m_LayerStack)
      layer->OnMaximize();
    SDL_GetWindowPosition(m_WindowHandle, &m_SavedWindowRect.x, &m_SavedWindowRect.y);
    SDL_GetWindowSize(m_WindowHandle, &m_SavedWindowRect.w, &m_SavedWindowRect.h);

    int display_index = SDL_GetWindowDisplayIndex(m_WindowHandle);
    SDL_Rect display_bounds;
    SDL_GetDisplayUsableBounds(display_index, &display_bounds);

    SDL_SetWindowPosition(m_WindowHandle, display_bounds.x, display_bounds.y);
    SDL_SetWindowSize(m_WindowHandle, display_bounds.w, display_bounds.h);
    m_IsMaximized = true;
  }
}

VkInstance Canvas::GetInstance() {
  return g_Instance;
}

VkPhysicalDevice Canvas::GetPhysicalDevice() {
  return g_PhysicalDevice;
}

VkDevice Canvas::GetDevice() {
  return g_Device;
}

VkCommandBuffer Canvas::GetCommandBuffer(bool begin) {
  ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

  VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

  VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
  cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufAllocateInfo.commandPool = command_pool;
  cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufAllocateInfo.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);
  check_vk_result(err);

  if (begin) {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    check_vk_result(err);
  }

  return command_buffer;
}

void Canvas::FlushCommandBuffer(VkCommandBuffer commandBuffer) {
  const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

  VkSubmitInfo end_info = {};
  end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  end_info.commandBufferCount = 1;
  end_info.pCommandBuffers = &commandBuffer;
  auto err = vkEndCommandBuffer(commandBuffer);
  check_vk_result(err);

  // Create fence to ensure that the command buffer has finished executing
  VkFenceCreateInfo fenceCreateInfo = {};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = 0;
  VkFence fence;
  err = vkCreateFence(g_Device, &fenceCreateInfo, nullptr, &fence);
  check_vk_result(err);

  err = vkQueueSubmit(g_Queue, 1, &end_info, fence);
  check_vk_result(err);

  err = vkWaitForFences(g_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
  check_vk_result(err);

  vkDestroyFence(g_Device, fence, nullptr);
}

void Canvas::SubmitResourceFree(std::function<void()>&& func) {
  s_ResourceFreeQueue[s_CurrentFrameIndex].emplace_back(func);
}
}  // namespace Weaver