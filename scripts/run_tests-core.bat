@echo off

echo "Building and running tests using default-debug preset..."

REM Configure and build using the preset
cmake --preset default-debug
if %errorlevel% neq 0 (
    echo "CMake configuration failed!"
    exit /b %errorlevel%
)

cmake --build --preset default-debug
if %errorlevel% neq 0 (
    echo "Build failed!"
    exit /b %errorlevel%
)

REM Run tests
cd build_Debug
ctest --output-on-failure -C Debug
cd ..
