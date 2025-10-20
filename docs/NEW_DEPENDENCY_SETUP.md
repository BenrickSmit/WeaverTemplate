# How to Add a New Library to the `Core` Project

This guide details the step-by-step process for integrating a new third-party library into the `Core` library of this project. The system uses CMake's `FetchContent` feature, which automates the downloading and configuration of dependencies.

We will use the integration of the `Abseil` logging library as a practical example.

## The 4-Step Process

Integrating a new dependency involves four main steps, all of which are handled within the CMake build system:

1.  **Declare the Dependency:** In `vendor/CMakeLists.txt`, specify where to download the library from.
2.  **Make the Dependency Available:** In `vendor/CMakeLists.txt`, instruct CMake to download and prepare the library.
3.  **Export Include Directories:** In the library's own `CMakeLists.txt` (e.g., `src/Core/CMakeLists.txt`), ensure it makes its headers available to other libraries that depend on it.
4.  **Link the Dependency:** In `src/Core/CMakeLists.txt`, connect the new library to our `Core` library.

---

### Step 1: Declare the Dependency in `vendor/CMakeLists.txt`

The first step is to tell CMake where to find the source code for the new library. 

- **Action:** Open `vendor/CMakeLists.txt` and add a `FetchContent_Declare` block for your new library.
- **Details:** You need to provide the Git repository URL and a specific version (a Git tag or commit hash) to ensure the dependency version is locked and consistent.

**Example (`Abseil`):**

```cmake
# In vendor/CMakeLists.txt

FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20240116.2
)
```

---

### Step 2: Make the Dependency Available in `vendor/CMakeLists.txt`

After declaring the dependency, you must instruct `FetchContent` to actually download and configure it.

- **Action:** In the same `vendor/CMakeLists.txt` file, find the `FetchContent_MakeAvailable` command.
- **Details:** Add the name of your new library to the list within this command.

**Example (`Abseil`):**

```cmake
# In vendor/CMakeLists.txt, after all declarations

FetchContent_MakeAvailable(imgui SDL2 glm googletest absl)
```

---

### Step 3: Export Include Directories in `src/Core/CMakeLists.txt`

For other libraries (like `App`) to be able to use headers from `Core`, the `Core` library must export its include directory.

- **Action:** Open `src/Core/CMakeLists.txt`.
- **Details:** Add a `target_include_directories` command and specify the `PUBLIC` keyword. This ensures that any target linking against `Core` also gets its include paths.

**Example (`Core` library):**

```cmake
# In src/Core/CMakeLists.txt

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```

---

### Step 4: Link the `Core` Library to the Dependency

The final step is to link the `Core` library against the new dependency. This makes the library's headers and compiled code available to `Core`.

- **Action:** Open `src/Core/CMakeLists.txt`.
- **Details:** Find the `target_link_libraries` command for the `Core` target and add the official CMake target names for your new library. For modern libraries, these are typically in the format `library_name::library_name`. Abseil is modular, so you link against the specific components you need.

**Example (`Abseil` logging):**

```cmake
# In src/Core/CMakeLists.txt

target_link_libraries(Core
  PUBLIC 
  # ... other libraries
  absl::log
  absl::log_initialize
)
```

---

By following these steps, you can cleanly integrate new dependencies into the project. The key is to ensure that libraries correctly declare their dependencies and export their own include directories so that other parts of the project can use them.