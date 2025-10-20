# Weaver GUI Project Setup

This project is a starter template for creating GUI applications with C++. It is built using C++, Vulkan, SDL2, and Dear ImGui.

## Features

*   **Customizable Theming:** The Dear ImGui theme can be easily customized.
*   **Advanced Logging:** The project uses Google's Abseil library for high-performance logging, enhanced with custom formats including timestamps and function names. It also includes gRPC-style status code macros. All log messages are written to timestamped files in the `logs` directory, making it ideal for a GUI application without a console.
*   **Automated Profiling:** Includes scripts to build a special 'Profile' configuration and run compiler-specific profiling tools, saving analysis in a dedicated `/profile` directory.
*   **One-Stop CMake Workflow:** A `CMakePresets.json` file defines a complete project workflow (configure, build release, run tests, package installer) executable with a single command.
*   **Integrated Testing:** The project is configured with Google Test for unit and integration testing.
*   **Layer-based Architecture:** The application is structured using a layer-based architecture, which makes it easy to add new features and separate concerns.
*   **CMake Build System:** The project uses CMake for building, which makes it easy to configure and build on different platforms.

## Project Structure

The project follows a clean, engine-inspired architecture that separates the core framework from the final application. For a detailed breakdown of the architecture, directories, and build system, please see the [Project Structure Documentation](docs/PROJECT_STRUCTURE.md).

## Getting Started

### Prerequisites

*   A C++ compiler that supports C++17. MSVC and Clang work easily.
*   CMake 3.15 or higher.
*   Vulkan SDK.
*   Reading the Documentation on the Project
    -   (Adding New Dependencies)[docs\NEW_DEPENDENCY_SETUP.md]
    -   (Project Structure)[docs\PROJECT_STRUCTURE.md]

### Tested Environments and Compilers

*   Windows 11
    - [x]   MSVC
    - [ ]   GCC (Fails with SDL2)
    - [x]   Clang
*   Linux
    - [ ]   MSVC
    - [ ]   GCC
    - [ ]   Clang

### Building, Running, and Packaging

This project includes several scripts in the root directory to automate common tasks. It is recommended to use these for all build, run, and packaging operations.

*   **To build the project:**
    ```sh
    # Build the default Release configuration
    ./build.sh # or build.bat on Windows

    # Build a specific configuration (Debug, Release, RelWithDebInfo)
    ./build.sh Debug
    ```
*   **To run the application (after building):**
    ```sh
    # Run the default Debug configuration
    ./run.sh # or run.bat on Windows

    # Run a specific configuration
    ./run.sh Release
    ```
*   **To run the tests:**
    ```sh
    ./run_tests.sh # or run_tests.bat on Windows
    ```
*   **To create an installer (after building Release):**
    ```sh
    ./create_installer.sh # or create_installer.bat on Windows
    ```
*   **To profile the application:**
    ```sh
    ./profile.sh # or profile.bat on Windows
    ```
*   **To run the full CMake workflow (configure, build, test, package):**
    ```sh
    ./run_workflow.sh # or run_workflow.bat on Windows
    ```

The final executables will be placed in the `deliverables` directory, organized by build type.

## How to Extend the Project

This project is designed to be a starter template. Here are some ways you can extend it:

*   **Update Project Details** You can update the project details with a unique name
*   **Extend the Core Code** You can extend the Core functionality to more advanced levels to achieve what you want
*   **Create New Layers:** You can create new application layers by subclassing the `Layer` class and adding them to the `Canvas`.
*   **Add New Tests:** Add new `.cpp` files to the `tests` directory to create new test suites. CMake will automatically discover and add them to the test runner.
*   **Use the Logger:** Include `src/Core/Log.h` and use the logging macros, for example: `WEAVER_LOG_INFO << "Hello from the logger!";`. It outputs to a local file from where the application is run.

## Contributing

### Adding to Core and App

To maintain code quality and rigidity, files added to the `Core` and `App` libraries must be manually listed in their respective `CMakeLists.txt` files. This ensures that every file is intentionally included in the build.

-   **Core:** To add a new file to the Core library, open `src/Core/CMakeLists.txt` and add the file path to the `add_library` command.
-   **App:** To add a new file to the App, open `src/App/CMakeLists.txt` and add the file path to the `add_executable` command.

### Adding Tests

For ease of development, the testing framework is set up to automatically discover and include any `.cpp` files added to the `tests` directory. This is achieved using `file(GLOB ...)` in `tests/CMakeLists.txt`.

To add a new test, simply create a new `.cpp` file in the `tests` directory and it will be automatically included in the test suite.

## License

This project is licensed under the Apache 2.0 License. See the [LICENSE](LICENSE) file for details.

---

## A Note on Scripts

The scripts in the root directory (e.g., `build.bat`, `run.sh`) are simple wrappers. The core logic for all build, run, and packaging operations is located in the `scripts/` directory in files named `*-core.bat` or `*-core.sh`. This approach keeps the root directory clean while allowing for easy modification of the core logic if needed.

---

## Inspiration

This GUI Class was inspired by the Walnut project by [Yan Chernikov](https://github.com/thecherno).
Ultimately, the project branched quite early based on what I wanted to do and
what his [Walnut](https://github.com/StudioCherno/Walnut) was able to do.

---

## Contributor

B.G. Smit
