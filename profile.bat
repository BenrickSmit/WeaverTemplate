@echo off
setlocal

$PROJECT_NAME = Select-String -Path "project_settings.cmake" -Pattern 'set$$PROJECT_NAME "([   

set "EXECUTABLE_NAME=%PROJECT_NAME%"
set "PROFILE_OUTPUT_DIR=profile"

echo --- Building Profile configuration ---
cmake --build --preset default-profile || goto :error

echo --- Running application for profiling ---
if not exist %PROFILE_OUTPUT_DIR% mkdir %PROFILE_OUTPUT_DIR%
pushd build_Profile\src\App\Profile
start /wait %EXECUTABLE_NAME%
popd

echo --- Analyzing gprof output (if gmon.out was generated) ---
if exist build_Profile\src\App\Profile\gmon.out (
    gprof build_Profile\src\App\Profile\%EXECUTABLE_NAME% build_Profile\src\App\Profile\gmon.out > %PROFILE_OUTPUT_DIR%\analysis.txt
    echo gprof analysis saved to %PROFILE_OUTPUT_DIR%\analysis.txt
) else (
    echo gmon.out not found. gprof analysis skipped.
    echo Ensure your compiler (e.g., MinGW-w64 GCC) supports -pg and the application runs to completion.
)

echo --- Profiling complete ---
goto :eof

:error
echo An error occurred during profiling.
exit /b 1