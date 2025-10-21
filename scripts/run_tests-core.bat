@echo off

set "BUILD_CONFIG=%1"
if "%BUILD_CONFIG%"=="" (
    set "BUILD_CONFIG=debug"
)

echo "Building and running tests using %BUILD_CONFIG% preset..."

REM Configure and build using the preset
cmake --preset default-%BUILD_CONFIG%
if %errorlevel% neq 0 (
    echo "CMake configuration failed!"
    exit /b %errorlevel%
)

cmake --build --preset default-%BUILD_CONFIG%
if %errorlevel% neq 0 (
    echo "Build failed!"
    exit /b %errorlevel%
)

REM Run tests
"%~dp0..\deliverables\%BUILD_CONFIG%\WeaverTests.exe"