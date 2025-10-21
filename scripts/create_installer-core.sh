#!/bin/sh

set -e

# Check if cpack is installed
if ! command -v cpack &> /dev/null
then
    echo "Error: cpack is not found. Please install CMake to create the installer."
    exit 1
fi

echo "-- Building Release configuration before creating installer..."
./build.sh default-release
if [ $? -ne 0 ]; then
    echo "Release build failed. Cannot create installer."
    exit 1
fi

echo "-- Creating installer..."

# CPack uses the CMAKE_BUILD_TYPE to find the config file, so we specify Release
cpack --preset unix-package --config build_release/CPackConfig.cmake
if [ $? -ne 0 ]; then
    echo "CPack failed."
    exit 1
fi

echo "-- Installer created successfully. --"
exit 0
