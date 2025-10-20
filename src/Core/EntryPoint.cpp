/**
 * @file EntryPoint.cpp
 * @author B.G. Smit
 * @brief Implements the main entry point for the Weaver GUI application.
 *
 * This file contains the `Main` function, which initializes the logging system,
 * creates and runs the `Canvas` application, and handles application shutdown.
 * It also defines platform-specific entry points (`WinMain` for Windows, `main` for others).
 * @copyright Copyright (c) 2025
 */
#include "EntryPoint.h"

bool g_CanvasRunning = true;

#include "Common/Settings.h"
#include "Log.h"
#include "absl/flags/parse.h"

namespace Weaver {

/**
 * @brief The main application entry point.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return The application exit code.
 */
int Main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  Log::Init();

  WEAVER_LOG_INFO "This is an info message.";
  WEAVER_LOG_WARN "This is a warning message.";
  WEAVER_LOG_ERROR "This is an error message.";

  Weaver::Canvas* app = Weaver::CreateCanvas(argc, argv);
  app->Run();
  delete app;

  return 0;
}

}  // namespace Weaver

#ifdef _WIN32

#include <Windows.h>
#include <stdio.h>

/**
 * @brief The Windows-specific entry point for the application.
 * @param hInst The instance handle.
 * @param hInstPrev The previous instance handle.
 * @param cmdline The command line arguments.
 * @param cmdshow The command show state.
 * @return The application exit code.
 */
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
  freopen(Weaver::Settings::LOG_FILE_PATH, "w", stderr);
  return Weaver::Main(__argc, __argv);
}

#else
#include <stdio.h>

/**
 * @brief The main entry point for non-Windows platforms.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return The application exit code.
 */
int main(int argc, char** argv) {
  freopen(Weaver::Settings::LOG_FILE_PATH, "w", stderr);
  return Weaver::Main(argc, argv);
}
#endif