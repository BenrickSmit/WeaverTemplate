#!/bin/bash

FILE="project_settings.cmake"
PROJECT_NAME=$(grep -oP 'set$$PROJECT_NAME "\K[^"]+' "$file")
echo "Project name: $PROJECT_NAME"

EXECUTABLE_NAME="$PROJECT_NAME" # Assuming no .exe extension on Linux
PROFILE_OUTPUT_DIR="deliverables/profile"

echo "--- Building Profile configuration ---"
cmake --build --preset default-profile || exit 1

echo "--- Running application for profiling ---"
mkdir -p "${PROFILE_OUTPUT_DIR}"
# Assuming the executable is in build_Profile/src/App/
# Adjust path if necessary based on actual build output
"build_Profile/src/App/${EXECUTABLE_NAME}"

echo "--- Analyzing gprof output (if gmon.out was generated) ---"
if [ -f "gmon.out" ]; then
    gprof "build_Profile/src/App/${EXECUTABLE_NAME}" gmon.out > "${PROFILE_OUTPUT_DIR}/analysis.txt"
    echo "gprof analysis saved to ${PROFILE_OUTPUT_DIR}/analysis.txt"
else
    echo "gmon.out not found. gprof analysis skipped."
    echo "Ensure your compiler (e.g., GCC/Clang) supports -pg and the application runs to completion."
fi

echo "--- Profiling complete ---"