/**
 * @file Input.h
 * @author B.G. Smit
 * @brief Declares the input handling interface for the Weaver GUI Template.
 *
 * This file defines the `Input` class, which provides a static interface
 * for querying keyboard and mouse input states, as well as controlling
 * the mouse cursor mode. It abstracts platform-specific input mechanisms.
 * @copyright Copyright (c) 2025
 */
#pragma once

#include "KeyCodes.h"

#include <glm/glm.hpp>

namespace Weaver {

/**
 * @class Input
 * @brief Provides static methods for querying input states and controlling the cursor.
 */
class Input {
 public:
  /**
   * @brief Checks if a specific key is currently pressed down.
   * @param keycode The keycode of the key to check.
   * @return True if the key is down, false otherwise.
   */
  static bool IsKeyDown(KeyCode keycode);

  /**
   * @brief Checks if a specific mouse button is currently pressed down.
   * @param button The mouse button to check.
   * @return True if the mouse button is down, false otherwise.
   */
  static bool IsMouseButtonDown(MouseButton button);

  /**
   * @brief Gets the current mouse position.
   * @return A glm::vec2 representing the mouse's X and Y coordinates.
   */
  static glm::vec2 GetMousePosition();

  /**
   * @brief Sets the cursor mode.
   * @param mode The desired cursor mode (Normal, Hidden, or Locked).
   */
  static void SetCursorMode(CursorMode mode);
};

}  // namespace Weaver
