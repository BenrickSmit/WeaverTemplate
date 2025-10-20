/**
 * @file Log.cpp
 * @author B.G. Smit
 * @brief Implements the logging utilities for the Weaver GUI Template.
 *
 * This file provides the implementation for the `Log` class, which initializes
 * and configures the Abseil logging library. It ensures that log messages are
 * directed to appropriate outputs and that a log directory is maintained.
 * @copyright Copyright (c) 2025
 */
#include "Log.h"

#include <filesystem>

#include "Common/Settings.h"  // Include Settings.h
#include "absl/flags/flag.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"

// Declare the log directory flag, which is defined by Abseil.
ABSL_FLAG(std::string, log_dir, "", "Directory to write log files to");

namespace Weaver {

/**
 * @brief Initializes the logging system.
 */
void Log::Init() {
  // Create a "logs" directory if it doesn't exist
  if (!std::filesystem::exists(Settings::LOG_DIRECTORY)) {
    std::filesystem::create_directory(Settings::LOG_DIRECTORY);
  }

  absl::InitializeLog();

  // Log all messages to stderr, which will be redirected to the log file.
  absl::SetStderrThreshold(absl::LogSeverity::kInfo);
}
}  // namespace Weaver