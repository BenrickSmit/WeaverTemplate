# Core Library Documentation

This document provides an overview of the files in the `src/Core` directory of the Weaver GUI Template project.

## Files and Their Purpose

### `Canvas.h` / `Canvas.cpp`
- **Purpose:** This is the heart of the application. The `Canvas` class manages the main application window, initializes the Vulkan rendering context, and runs the main event loop. It is responsible for managing the layer stack, where different parts of the application's UI and logic reside.

### `EntryPoint.h` / `EntryPoint.cpp`
- **Purpose:** This file provides the main entry point for the application. It contains the `main` function (and `WinMain` for Windows) that starts the application, initializes the logging system, and creates and runs the `Canvas`.

### `Log.h` / `Log.cpp` / `FileLogSink.h` / `FileLogSink.cpp`
- **Purpose:** These files implement the logging system. `Log.h` and `Log.cpp` provide a simple interface for logging using the Abseil library. `FileLogSink.h` and `FileLogSink.cpp` define a custom log sink that directs log messages to a file.

### `Image.h` / `Image.cpp`
- **Purpose:** This class is responsible for loading and managing images as Vulkan textures. It uses the `stb_image.h` library to load image files from disk. This is essential for displaying images in the UI.

### `Layer.h`
- **Purpose:** This file defines the abstract `Layer` base class. Layers are used to separate different parts of the application, such as UI panels, rendering logic, or other functionalities. Layers are pushed onto the `Canvas`'s layer stack to be updated and rendered.

### `Themes.h` / `Themes.cpp`
- **Purpose:** These files contain functions for applying different visual themes to the Dear ImGui interface. This allows for easy customization of the application's look and feel.

### `Input/`
- **`Input.h` / `Input.cpp`:** Provides a static interface for querying keyboard and mouse input states. It abstracts the underlying SDL input handling.
- **`KeyCodes.h`:** Defines key codes, mouse button codes, and cursor modes for input handling, mapped from SDL.

### `Common/Settings.h`
- **Purpose:** A centralized location for application-wide settings and constants, such as file paths and directory names.

### Utility Files
- **`FunctionPreprocessor.h`:** A utility header that provides a cross-compiler macro (`CALLING_FUNCTION_NAME`) for retrieving the name of the current function, which is used in logging.
- **`IconsMaterialDesign.h`:** Contains definitions for a large set of Material Design icons, allowing them to be easily used in the UI with ImGui.
- **`LogStatusCodes.h`:** Defines macros for logging with gRPC-style status codes (e.g., `WEAVER_LOG_CANCELLED`), which helps in standardizing error and status reporting.
- **`Random.h` / `Random.cpp`:** A utility class for generating random numbers.
- **`Timer.h`:** Provides `Timer` and `ScopedTimer` classes for measuring execution time, which is useful for performance profiling.
- **`stb_image/stb_image.h`:** A single-header image loading library used by `Image.cpp` to load various image formats.

### `MathTest.h`
- **Purpose:** This file contains a simple `AddInts` function. Although named `MathTest.h`, it is included as part of the `Core` library and used in the tests (`tests/test_math.cpp`). A more appropriate name for this file might be `Math.h`. It is not unused.
