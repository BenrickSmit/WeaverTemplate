/**
 * @file FileLogSink.h
 * @author B.G. Smit
 * @brief Declares a custom log sink for writing log messages to a file.
 *
 * This file defines the `FileLogSink` class, which inherits from `absl::LogSink`
 * to provide a mechanism for directing log output to a specified file.
 * It ensures thread-safe writing to the log file.
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <fstream>
#include <string>

#include "absl/log/log_sink.h"
#include "absl/synchronization/mutex.h"

namespace Weaver {

/**
 * @class FileLogSink
 * @brief A custom log sink that writes log messages to a file.
 */
class FileLogSink : public absl::LogSink {
 public:
  /**
   * @brief Constructs a new FileLogSink object.
   * @param filename The name of the log file.
   */
  explicit FileLogSink(const std::string& filename);
  /**
   * @brief Destroys the FileLogSink object.
   */
  ~FileLogSink() override;

  /**
   * @brief Sends a log entry to the log file.
   * @param entry The log entry to send.
   */
  void Send(const absl::LogEntry& entry) override;
  /**
   * @brief Flushes the log file.
   */
  void Flush() override;

 private:
  std::ofstream log_file_;
  absl::Mutex mutex_;
};

}  // namespace Weaver