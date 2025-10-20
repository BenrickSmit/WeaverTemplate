#!/bin/bash
echo "Running full CMake workflow..."
cmake --workflow --preset full-workflow
if [ $? -ne 0 ]; then
    echo "CMake workflow failed."
    exit 1
fi
echo "CMake workflow completed successfully."