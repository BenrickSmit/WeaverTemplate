/**
 * @file FileLogSink.cpp
 * @author B.G. Smit
 * @brief Implements a custom log sink for writing log messages to a file.
 *
 * This file provides the implementation for the `FileLogSink` class, which extends
 * `absl::LogSink` to enable logging directly to a specified file. It ensures
 * that log messages are written safely and efficiently, with proper file handling.
 * @copyright Copyright (c) 2025
 */
#include "FileLogSink.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"

namespace Weaver {

/**
 * @brief Constructs a new FileLogSink object.
 * @param filename The name of the log file.
 */
FileLogSink::FileLogSink(const std::string& filename) {
  log_file_.open(filename, std::ios_base::out | std::ios_base::app);
  if (!log_file_.is_open()) {
    // Handle error: log to stderr if file cannot be opened
    fprintf(stderr, "Failed to open log file: %s\n", filename.c_str());
  }
}

/**
 * @brief Destroys the FileLogSink object.
 */
FileLogSink::~FileLogSink() {
  Flush();
  log_file_.close();
}

/**
 * @brief Sends a log entry to the log file.
 * @param entry The log entry to send.
 */
void FileLogSink::Send(const absl::LogEntry& entry) {
  absl::MutexLock lock(&mutex_);
  if (log_file_.is_open()) {
    log_file_ << entry.text_message_with_prefix_and_newline();
  }
}

/**
 * @brief Flushes the log file.
 */
void FileLogSink::Flush() {
  absl::MutexLock lock(&mutex_);
  if (log_file_.is_open()) {
    log_file_.flush();
  }
}

}  // namespace Weaver
