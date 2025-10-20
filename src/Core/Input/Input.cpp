/**
 * @file Input.cpp
 * @author B.G. Smit
 * @brief Implements the input handling functionalities for the Weaver GUI Template.
 *
 * This file provides the implementation for the `Input` class, which abstracts
 * away platform-specific input handling (e.g., keyboard, mouse) using SDL2.
 * It offers methods to query key states, mouse button states, and mouse position,
 * as well as to control the cursor mode.
 * @copyright Copyright (c) 2025
 */
#include "Input.h"

#include <SDL2/SDL.h>

#include "../Canvas.h"

namespace Weaver {

/**
 * @brief Checks if a specific key is currently pressed down.
 * @param keycode The keycode of the key to check.
 * @return True if the key is down, false otherwise.
 */
bool Input::IsKeyDown(KeyCode keycode) {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  return state[static_cast<int>(keycode)] != 0;
}

/**
 * @brief Checks if a specific mouse button is currently pressed down.
 * @param button The mouse button to check.
 * @return True if the mouse button is down, false otherwise.
 */
bool Input::IsMouseButtonDown(MouseButton button) {
  Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
  return (mouseState & SDL_BUTTON(static_cast<int>(button))) != 0;
}

/**
 * @brief Gets the current mouse position.
 * @return A glm::vec2 representing the mouse's X and Y coordinates.
 */
glm::vec2 Input::GetMousePosition() {
  int x, y;
  SDL_GetMouseState(&x, &y);
  return {(float)x, (float)y};
}

/**
 * @brief Sets the cursor mode.
 * @param mode The desired cursor mode (Normal, Hidden, or Locked).
 */
void Input::SetCursorMode(CursorMode mode) {
  SDL_Window* windowHandle = Canvas::Get().GetWindowHandle();

  if (mode == CursorMode::Normal) {
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
  } else if (mode == CursorMode::Hidden) {
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
  } else if (mode == CursorMode::Locked) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
  }
}

}  // namespace Weaver
