/**
 * @file Log.h
 * @author B.G. Smit
 * @brief Declares the logging interface and macros for the Weaver GUI Template.
 *
 * This file provides macros for simplified logging using the Abseil logging library.
 * It defines custom formatting for log messages, including timestamps and function names,
 * to enhance debuggability and traceability within the application.
 * @copyright Copyright (c) 2025
 */
#pragma once

#include "FunctionPreprocessor.h"
#include "LogStatusCodes.h"
#include "absl/log/log.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"

// Required to add in more detailed information:
// [current timestamp] [calling function name] [message]
#define ABSEIL_PREFIX                                                                           \
  "[" << absl::FormatTime(absl::Now(), absl::LocalTimeZone()) << "] [" << CALLING_FUNCTION_NAME \
      << "] "

namespace Weaver {

/**
 * @class Log
 * @brief Provides an interface for initializing the logging system.
 */
class Log {
 public:
  /**
   * @brief Initializes the logging system.
   */
  static void Init();
};

}  // namespace Weaver

// Format of <NEW NAME> <FUNCTION> <PREFIX> <REAL_MESSAGE>
#define WEAVER_LOG_INFO LOG(INFO) << ABSEIL_PREFIX <<
#define WEAVER_LOG_WARN LOG(WARNING) << ABSEIL_PREFIX <<
#define WEAVER_LOG_ERROR LOG(ERROR) << ABSEIL_PREFIX <<
#define WEAVER_LOG_FATAL LOG(FATAL) << ABSEIL_PREFIX <<
