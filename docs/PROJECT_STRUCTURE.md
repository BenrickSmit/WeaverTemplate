# Project Structure

This document outlines the architecture of the Weaver GUI Template project. The structure is heavily inspired by modern game engine design, promoting a clean separation of concerns between the core engine/framework and the final application logic.

## Core Philosophy

The project is divided into two main components:

-   **`Core` Library**: A static library that acts as the "engine." It contains all the foundational, reusable code, such as rendering, input handling, UI theming, and other low-level systems. It is designed to be completely independent of the final application's specific logic.
-   **`App` Executable**: The main application that consumes the `Core` library. It acts as the user-facing layer, defining the specific UI, application logic, and overall behavior by using the features provided by `Core`.

This separation ensures that the foundational engine can be developed and tested in isolation and can be potentially reused across different applications.

## Directory Layout

```
WeaverTemplate/
│
├── build/                # CMake build output (temporary)
├── deliverables/         # Final, distributable application binaries
├── docs/                 # All project documentation
├── scripts/              # Helper scripts for building, running, packaging
├── src/                  # Main source code
│   ├── App/              # The final application executable
│   └── Core/             # The core engine/framework static library
│
├── tests/                # Unit and integration tests
├── vendor/               # Third-party dependencies
│
├── .clang-format         # Code formatting rules
├── .clang-tidy           # Static analysis rules
├── CMakeLists.txt        # Root CMake build script
└── project_settings.cmake # Global project settings (name, version, etc.)
```

### `src/Core`

This is the heart of the application framework.

-   **Purpose**: To provide all the essential, reusable systems.
-   **Contents**:
    -   Window and input management (`Input`, `KeyCodes`).
    -   Business Logic (`Math`).
    -   Logging (`Log`).
    -   UI helpers (`Themes`).
-   **Output**: A static library named `Core`. It is linked against by the `App` executable.

### `src/App`

This is the entry point and the concrete implementation of the application.

-   **Purpose**: To use the `Core` library to build the final user-facing application.
-   **Contents**:
    -   `WeaverInterface.cpp`: Contains the `main` function and the primary application loop.
    -   Defines the specific ImGui windows, layouts, and logic for the application.
-   **Output**: The final executable (e.g., `App.exe`).

### `vendor/`

This directory handles all third-party dependencies.

-   **Mechanism**: It uses CMake's `FetchContent` module to download and configure dependencies at build time. This avoids the need to commit large binary files or libraries to the repository.
-   **Dependencies**: Manages libraries like `SDL2`, `ImGui`, `glm`, `googletest`, and `absl`.
-   **Configuration**: The `vendor/CMakeLists.txt` file declares all dependencies.

### `tests/`

This directory contains the test suite for the project.

-   **Framework**: Uses the `googletest` framework.
-   **Purpose**: To write unit and integration tests for the `Core` library and other components to ensure stability and correctness.
-   **Output**: A separate executable (e.g., `WeaverTests.exe`) for running tests.

### `docs/`

This directory contains all project-related documentation, including this file, dependency guides, and analysis documents.

### `scripts/`

Contains various helper scripts for common tasks like building all release configurations (`build_all_releases.bat`/`.sh`) or running the application (`run.bat`/`.sh`).

### Build System

The project uses **CMake** as its build system. The root `CMakeLists.txt` file orchestrates the entire build, including fetching dependencies, building the `Core` and `App` components, and setting up tests. Global settings like the project name and C++ standard are defined in `project_settings.cmake`.
