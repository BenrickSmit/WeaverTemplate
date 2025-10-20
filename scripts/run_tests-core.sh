#!/bin/bash
set -e

echo "Building and running tests using default-debug preset..."

# Configure and build using the preset
cmake --preset default-debug
if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

cmake --build --preset default-debug
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Run tests
cd build_Debug
ctest --output-on-failure -C Debug
cd ..
