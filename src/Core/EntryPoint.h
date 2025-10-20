/**
 * @file EntryPoint.h
 * @author B.G. Smit
 * @brief Declares the main entry point and related functions for the Weaver GUI application.
 *
 * This file defines the `Main` function signature and declares the `CreateCanvas`
 * function, which is implemented by the client application to provide a custom `Canvas` instance.
 * It also declares a global flag `g_CanvasRunning` to control the application's main loop.
 * @copyright Copyright (c) 2025
 */
#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#pragma once

#include "Canvas.h"

/**
 * @brief Creates the application canvas. This function is implemented by the client.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return A pointer to the created Canvas object.
 */
extern Weaver::Canvas* Weaver::CreateCanvas(int argc, char** argv);

/**
 * @brief A global flag that controls the application's main loop.
 */
extern bool g_CanvasRunning;

namespace Weaver {

/**
 * @brief The main application entry point.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return The application exit code.
 */
int Main(int argc, char** argv);

}  // namespace Weaver

#endif
