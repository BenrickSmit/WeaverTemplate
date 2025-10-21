# Packaging

The app comes with packaging through [CPack](https://cmake.org/cmake/help/latest/module/CPack.html), creating installer
for macOS, Windows, and Linux.

General packaging settings are located under `packaging/`. Executable specific settings
in `src/app/cmake/Packaging.cmake` and OS specifics under `src/app/cmake/packaging/`.

## General packaging settings

General packaging settings are in `packaging/CMakeLists.txt`. It defines names, versions, metadata, where to build the
package to (`deliverables/<BUILD_TYPE>/`), and more.

## OS specific settings

### macOS

For macOS a `.dmg` (DragNDrop) file will be generated. Resources for the installer are located under `packaging/dmg/`,
containing the background image for the DMG view and an apple script to generate the custom DMG view.

Packaging settings for the application executable are in `src/app/cmake/packaging/Darwin.cmake`.

The final application build for Apple devices should be built via the `Xcode` generator with CMake.

```shell
cmake -GXcode -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -B build/xcode
cmake --build build/xcode --config Release
```

### Windows

On Windows a `.exe` is created via [NSIS](https://nsis.sourceforge.io/Main_Page). NSIS **needs to be installed** in
Windows to create the package. Resources for the installer are located under `packaging/nsis/`, containing installer
images and the uninstaller icon.

Packaging settings for the application executable are in `src/app/cmake/packaging/Windows.cmake`.

For Windows, installer texts (Welcome and Description) are dynamically generated from variables defined in `project_settings.cmake`. These are then used to configure `Welcome.txt` and `Description.txt` in the build directory. The Readme and License files are referenced directly from the project source directory.

```cmake
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_BINARY_DIR}/Welcome.txt)
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_BINARY_DIR}/Description.txt)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/Readme.md)
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/License)
```

### Linux

On Linux, `TGZ`, `.deb` (for Debian/Ubuntu), and `.rpm` (for Fedora/CentOS) packages can be created. The application icon and desktop entry are also configured for proper integration with the desktop environment.

Packaging settings for the application executable are in `src/app/cmake/packaging/Linux.cmake`.

## Distribution package creation

A release build is needed before creating the distribution package. Packages **for a system** are created **on the
system**.

### macOS

Xcode should be used to create the release build for the application distributable.

```shell
cmake -GXcode -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -B build/xcode
cmake --build build/xcode --config Release
cpack --config build/xcode/CPackConfig.cmake
```

**Attention:** Creating the package will open a Finder window a couple of times to set the DMG window properties. This
windows should be ignored and will auto-close.

### Windows & Linux

To create a package, simply run the full workflow script:

```sh
./run_workflow.sh # or run_workflow.bat on Windows
```

This will configure, build, test, and package the project, placing the installers in the `deliverables/<BUILD_TYPE>/` directory.

***

Next up: [Cmake Presets](CMakePresets.md)
