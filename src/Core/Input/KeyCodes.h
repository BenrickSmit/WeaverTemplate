/**
 * @file KeyCodes.h
 * @author B.G. Smit
 * @brief Defines key codes, mouse button codes, and cursor modes for input handling.
 *
 * This file provides platform-agnostic enumerations for keyboard keys, mouse buttons,
 * and cursor visibility/behavior, mapping them to SDL2's internal representations.
 * It also includes overloaded stream operators for convenient logging of these input codes.
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

#include <iostream>

namespace Weaver {

/**
 * @enum KeyCode
 * @brief Enumeration for keyboard key codes, mapped to SDL_Scancode values.
 */
typedef enum class KeyCode : uint16_t {
  // Based on SDL Scancode and Keycode mappings
  Space = SDL_SCANCODE_SPACE,
  Apostrophe = SDL_SCANCODE_APOSTROPHE,
  Comma = SDL_SCANCODE_COMMA,
  Minus = SDL_SCANCODE_MINUS,
  Period = SDL_SCANCODE_PERIOD,
  Slash = SDL_SCANCODE_SLASH,

  D0 = SDL_SCANCODE_0,
  D1 = SDL_SCANCODE_1,
  D2 = SDL_SCANCODE_2,
  D3 = SDL_SCANCODE_3,
  D4 = SDL_SCANCODE_4,
  D5 = SDL_SCANCODE_5,
  D6 = SDL_SCANCODE_6,
  D7 = SDL_SCANCODE_7,
  D8 = SDL_SCANCODE_8,
  D9 = SDL_SCANCODE_9,

  Semicolon = SDL_SCANCODE_SEMICOLON,
  Equal = SDL_SCANCODE_EQUALS,

  A = SDL_SCANCODE_A,
  B = SDL_SCANCODE_B,
  C = SDL_SCANCODE_C,
  D = SDL_SCANCODE_D,
  E = SDL_SCANCODE_E,
  F = SDL_SCANCODE_F,
  G = SDL_SCANCODE_G,
  H = SDL_SCANCODE_H,
  I = SDL_SCANCODE_I,
  J = SDL_SCANCODE_J,
  K = SDL_SCANCODE_K,
  L = SDL_SCANCODE_L,
  M = SDL_SCANCODE_M,
  N = SDL_SCANCODE_N,
  O = SDL_SCANCODE_O,
  P = SDL_SCANCODE_P,
  Q = SDL_SCANCODE_Q,
  R = SDL_SCANCODE_R,
  S = SDL_SCANCODE_S,
  T = SDL_SCANCODE_T,
  U = SDL_SCANCODE_U,
  V = SDL_SCANCODE_V,
  W = SDL_SCANCODE_W,
  X = SDL_SCANCODE_X,
  Y = SDL_SCANCODE_Y,
  Z = SDL_SCANCODE_Z,

  LeftBracket = SDL_SCANCODE_LEFTBRACKET,
  Backslash = SDL_SCANCODE_BACKSLASH,
  RightBracket = SDL_SCANCODE_RIGHTBRACKET,
  GraveAccent = SDL_SCANCODE_GRAVE,

  Escape = SDL_SCANCODE_ESCAPE,
  Enter = SDL_SCANCODE_RETURN,
  Tab = SDL_SCANCODE_TAB,
  Backspace = SDL_SCANCODE_BACKSPACE,
  Insert = SDL_SCANCODE_INSERT,
  Delete = SDL_SCANCODE_DELETE,
  Right = SDL_SCANCODE_RIGHT,
  Left = SDL_SCANCODE_LEFT,
  Down = SDL_SCANCODE_DOWN,
  Up = SDL_SCANCODE_UP,

  F1 = SDL_SCANCODE_F1,
  F2 = SDL_SCANCODE_F2,
  F3 = SDL_SCANCODE_F3,
  F4 = SDL_SCANCODE_F4,
  F5 = SDL_SCANCODE_F5,
  F6 = SDL_SCANCODE_F6,
  F7 = SDL_SCANCODE_F7,
  F8 = SDL_SCANCODE_F8,
  F9 = SDL_SCANCODE_F9,
  F10 = SDL_SCANCODE_F10,
  F11 = SDL_SCANCODE_F11,
  F12 = SDL_SCANCODE_F12,

  LeftShift = SDL_SCANCODE_LSHIFT,
  LeftControl = SDL_SCANCODE_LCTRL,
  LeftAlt = SDL_SCANCODE_LALT,
  RightShift = SDL_SCANCODE_RSHIFT,
  RightControl = SDL_SCANCODE_RCTRL,
  RightAlt = SDL_SCANCODE_RALT,
} Key;

/**
 * @enum KeyState
 * @brief Enumeration for the state of a key.
 */
enum class KeyState {
  None = -1, /**< No state. */
  Pressed,   /**< Key is pressed. */
  Held,      /**< Key is held down. */
  Released   /**< Key is released. */
};

/**
 * @enum CursorMode
 * @brief Enumeration for cursor visibility and behavior.
 */
enum class CursorMode {
  Normal = SDL_SYSTEM_CURSOR_ARROW, /**< Normal cursor. */
  Hidden = SDL_SYSTEM_CURSOR_WAIT,  /**< Hidden cursor. */
  Locked = SDL_SYSTEM_CURSOR_NO     /**< Locked cursor. */
};

/**
 * @enum MouseButton
 * @brief Enumeration for mouse button codes, mapped to SDL_BUTTON values.
 */
typedef enum class MouseButton : uint16_t {
  Left = SDL_BUTTON_LEFT,     /**< Left mouse button. */
  Right = SDL_BUTTON_RIGHT,   /**< Right mouse button. */
  Middle = SDL_BUTTON_MIDDLE, /**< Middle mouse button. */
} Button;

/**
 * @brief Overloads the stream insertion operator for KeyCode.
 * @param os The output stream.
 * @param keyCode The KeyCode to insert.
 * @return The output stream.
 */
inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) {
  os << static_cast<int32_t>(keyCode);
  return os;
}

/**
 * @brief Overloads the stream insertion operator for MouseButton.
 * @param os The output stream.
 * @param button The MouseButton to insert.
 * @return The output stream.
 */
inline std::ostream& operator<<(std::ostream& os, MouseButton button) {
  os << static_cast<int32_t>(button);
  return os;
}
}  // namespace Weaver
