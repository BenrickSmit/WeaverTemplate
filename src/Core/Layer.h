/**
 * @file Layer.h
 * @author B.G. Smit
 * @brief Defines the base class for application layers in the Weaver GUI
 * Template.
 *
 * This file provides the `Layer` abstract base class, which serves as a
 * fundamental building block for organizing application logic and UI
 * components. Layers can be pushed onto a `Canvas` stack, allowing for modular
 * and extensible application design.
 * @copyright Copyright (c) 2025
 */
#ifndef LAYER_H
#define LAYER_H

#pragma once

namespace Weaver {

/**
 * @class Layer
 * @brief An abstract base class for application layers.
 * @details Layers are used to separate different parts of the application, such as UI, rendering, and logic.
 */
class Layer {
 public:
  /**
   * @brief Virtual destructor for the Layer class.
   */
  virtual ~Layer() = default;

  /**
   * @brief Called when the layer is attached to the layer stack.
   */
  virtual void OnAttach() {}
  /**
   * @brief Called when the layer is detached from the layer stack.
   */
  virtual void OnDetach() {}

  /**
   * @brief Called every frame to update the layer's state.
   * @param ts The time step since the last frame.
   */
  virtual void OnUpdate(float ts) {}
  /**
   * @brief Called every frame to render the layer's UI.
   */
  virtual void OnUIRender() {}
  /**
   * @brief Called when the window is resized.
   * @param width The new width of the window.
   * @param height The new height of the window.
   */
  virtual void OnResize(uint32_t width, uint32_t height) {}

  /**
   * @brief Called when the window is minimized.
   */
  virtual void OnMinimize() {}
  /**
   * @brief Called when the window is maximized.
   */
  virtual void OnMaximize() {}
  /**
   * @brief Called when the window is restored.
   */
  virtual void OnRestored() {}
};

}  // namespace Weaver

#endif