/**
 * @file Settings.h
 * @author B.G. Smit
 * @brief Defines global settings and constants for the Weaver GUI Template.
 *
 * This file provides a centralized location for application-wide settings,
 * such as file paths and directory names, ensuring consistency across the project.
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <SDL.h>
#include <imgui.h>

namespace Weaver {
namespace Settings {

/**
 * @brief The default path for the application's log file.
 */
const char* const LOG_FILE_PATH = "logs/weaver.log";

/**
 * @brief The default directory for storing application log files.
 */
const char* const LOG_DIRECTORY = "logs";

namespace Window {
/**
 * @brief The default width of the main application window.
 */
constexpr int DEFAULT_WIDTH = 1600;
/**
 * @brief The default height of the main application window.
 */
constexpr int DEFAULT_HEIGHT = 900;
/**
 * @brief The corner radius of the main application window.
 */
constexpr int CORNER_RADIUS = 10;
}  // namespace Window

namespace UI {
/**
 * @brief The minimum width of the control panel.
 */
constexpr float CONTROL_PANEL_MIN_WIDTH = 254.0f;
/**
 * @brief The minimum height of the control panel.
 */
constexpr float CONTROL_PANEL_MIN_HEIGHT = 870.0f;
/**
 * @brief The buffer size for text input fields.
 */
constexpr int TEXT_INPUT_BUFFER_SIZE = 256;
/**
 * @brief The history size for the framerate plot.
 */
constexpr int FRAME_RATE_HISTORY_SIZE = 100;
/**
 * @brief The multiplier for the framerate plot's max y-value.
 */
constexpr float FRAME_RATE_PLOT_MULTIPLIER = 1.5f;
/**
 * @brief The height of the plot widgets.
 */
constexpr float PLOT_HEIGHT = 80.0f;
/**
 * @brief The number of items in the bar chart.
 */
constexpr int BAR_CHART_ITEM_COUNT = 10;
/**
 * @brief The update interval for the bar chart in seconds.
 */
constexpr float BAR_CHART_UPDATE_INTERVAL = 0.5f;
/**
 * @brief The x-position offset for the menu bar buttons.
 */
constexpr float MENUBAR_BUTTON_OFFSET = 136.0f;
/**
 * @brief The color for the minimize button on hover.
 */
constexpr ImVec4 MINIMIZE_BUTTON_HOVER_COLOR = ImVec4(0.2f, 0.7f, 0.2f, 1.0f);
/**
 * @brief The color for the minimize button when active.
 */
constexpr ImVec4 MINIMIZE_BUTTON_ACTIVE_COLOR = ImVec4(0.1f, 0.5f, 0.1f, 1.0f);
/**
 * @brief The color for the maximize button on hover.
 */
constexpr ImVec4 MAXIMIZE_BUTTON_HOVER_COLOR = ImVec4(0.2f, 0.2f, 0.7f, 1.0f);
/**
 * @brief The color for the maximize button when active.
 */
constexpr ImVec4 MAXIMIZE_BUTTON_ACTIVE_COLOR = ImVec4(0.1f, 0.1f, 0.5f, 1.0f);
/**
 * @brief The color for the close button on hover.
 */
constexpr ImVec4 CLOSE_BUTTON_HOVER_COLOR = ImVec4(0.7f, 0.2f, 0.2f, 1.0f);
/**
 * @brief The color for the close button when active.
 */
constexpr ImVec4 CLOSE_BUTTON_ACTIVE_COLOR = ImVec4(0.5f, 0.1f, 0.1f, 1.0f);
}  // namespace UI

namespace Font {
/**
 * @brief The default font size for the Roboto Mono font.
 */
constexpr float ROBOTO_MONO_FONT_SIZE = 16.0f;
/**
 * @brief The default font size for the Material Symbols font.
 */
constexpr float MATERIAL_SYMBOLS_FONT_SIZE = 17.0f;
}  // namespace Font

namespace Rendering {
/**
 * @brief The number of bits per pixel for the window shape surface.
 */
constexpr int SHAPE_SURFACE_BPP = 32;
/**
 * @brief The binarization cutoff for the window shape.
 */
constexpr Uint8 SHAPE_BINARIZATION_CUTOFF = 255;
/**
 * @brief The size of the command buffer.
 */
constexpr uint32_t COMMAND_BUFFER_SIZE = 1000;
}  // namespace Rendering

} // namespace Settings
} // namespace Weaver
