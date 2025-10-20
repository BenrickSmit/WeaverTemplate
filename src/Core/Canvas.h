/**
 * @file Canvas.h
 * @author B.G. Smit
 * @brief Defines the core application canvas and window management for the Weaver GUI Template.
 *
 * This file provides the foundational `Canvas` class, which manages the application window,
 * event loop, layer stack, and basic rendering setup. It serves as the entry point
 * for the GUI application, orchestrating the various components and layers.
 *
 * @copyright Copyright (c) 2025
 */
#ifndef CANVAS_H
#define CANVAS_H

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Layer.h"

// #include "imgui.h"
#include <SDL.h>
#include <vulkan/vulkan.h>
// #include <vk_enum_string_helper.h>

/**
 * @brief Checks the result of a Vulkan operation and aborts on failure.
 * @param err The Vulkan result to check.
 */
void check_vk_result(VkResult err);

struct GLFWwindow;

namespace Weaver {

/**
 * @struct CanvasSpecification
 * @brief Defines the specifications for the application canvas.
 */
struct CanvasSpecification {
  std::string Name = "Weaver Interface App"; /**< The name of the application window. */
  uint32_t Width = 1600;                     /**< The width of the application window. */
  uint32_t Height = 900;                    /**< The height of the application window. */
  short CornerRadius = 12;                  /**< The corner radius of the application window. */
};

/**
 * @class Canvas
 * @brief The main application class, responsible for managing the window, layers, and rendering.
 */
class Canvas {
 public:
  /**
   * @brief Constructs a new Canvas object.
   * @param CanvasSpecification The specifications for the canvas.
   */
  explicit Canvas(CanvasSpecification& CanvasSpecification);
  /**
   * @brief Destroys the Canvas object.
   */
  ~Canvas();

  /**
   * @brief Gets the singleton instance of the Canvas.
   * @return A reference to the Canvas instance.
   */
  static Canvas& Get();

  /**
   * @brief Runs the main application loop.
   */
  void Run();
  /**
   * @brief Sets the callback function for the main menu bar.
   * @param menubarCallback The callback function.
   */
  void SetMenubarCallback(const std::function<void()>& menubarCallback) {
    m_MenubarCallback = menubarCallback;
  }

  /**
   * @brief Pushes a new layer onto the layer stack.
   * @tparam T The type of the layer to push.
   */
  template <typename T>
  void PushLayer() {
    static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
    m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
  }

  /**
   * @brief Pushes a new layer onto the layer stack.
   * @param layer The layer to push.
   */
  void PushLayer(const std::shared_ptr<Layer>& layer) {
    m_LayerStack.emplace_back(layer);
    layer->OnAttach();
  }

  /**
   * @brief Closes the application.
   */
  void Close();

  /**
   * @brief Minimizes the application window.
   */
  void Minimize();
  /**
   * @brief Toggles the application window between maximized and restored states.
   */
  void ToggleMaximize();

  /**
   * @brief Gets the current time in seconds.
   * @return The current time in seconds.
   */
  float GetTime();
  /**
   * @brief Gets the SDL window handle.
   * @return The SDL window handle.
   */
  SDL_Window* GetWindowHandle() const {
    return m_WindowHandle;
  }

  /**
   * @brief Gets the Vulkan instance.
   * @return The Vulkan instance.
   */
  static VkInstance GetInstance();
  /**
   * @brief Gets the physical device (GPU).
   * @return The physical device.
   */
  static VkPhysicalDevice GetPhysicalDevice();
  /**
   * @brief Gets the logical device.
   * @return The logical device.
   */
  static VkDevice GetDevice();

  /**
   * @brief Gets a command buffer from the command pool.
   * @param begin Whether to begin the command buffer.
   * @return The command buffer.
   */
  static VkCommandBuffer GetCommandBuffer(bool begin);
  /**
   * @brief Flushes a command buffer.
   * @param commandBuffer The command buffer to flush.
   */
  static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

  /**
   * @brief Submits a resource to be freed when the current frame is finished.
   * @param func The function to call to free the resource.
   */
  static void SubmitResourceFree(std::function<void()>&& func);

 private:
  /**
   * @brief Initializes the canvas.
   */
  void Init();
  /**
   * @brief Shuts down the canvas.
   */
  void Shutdown();
  /**
   * @brief Sets the shape of the window.
   */
  void SetWindowShape();

 private:
  CanvasSpecification m_Specification;
  SDL_Window* m_WindowHandle = nullptr;
  bool m_Running = false;
  bool m_IsFullscreen = false;
  bool m_IsMaximized = false;
  bool m_restore_in_progress = false;
  SDL_Rect m_SavedWindowRect;

  float m_TimeStep = 0.0f;
  float m_FrameTime = 0.0f;
  float m_LastFrameTime = 0.0f;

  std::vector<std::shared_ptr<Layer>> m_LayerStack;
  std::function<void()> m_MenubarCallback;
};

// Implemented by CLIENT
extern Canvas* CreateCanvas(int argc, char** argv);
}  // namespace Weaver

#endif