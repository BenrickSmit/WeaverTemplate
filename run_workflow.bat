@echo off
echo Running full CMake workflow...
cmake --workflow --preset full-workflow
if %errorlevel% neq 0 (
    echo CMake workflow failed.
    exit /b %errorlevel%
)
echo CMake workflow completed successfully.