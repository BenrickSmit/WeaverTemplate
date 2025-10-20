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

} // namespace Settings
} // namespace Weaver
