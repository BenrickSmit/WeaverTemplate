@echo off
setlocal

set "PRESET_NAME=%1"

if not defined PRESET_NAME (
    echo No preset specified, defaulting to default-debug.
    set "PRESET_NAME=default-debug"
)

if /I "%PRESET_NAME%"=="--help" goto show_help
if /I "%PRESET_NAME%"=="-h" goto show_help
if "%PRESET_NAME%"=="/?" goto show_help

set "VALID_PRESETS=default-debug default-release default-profile ninja-debug ninja-release ninja-profile"
(echo %VALID_PRESETS% | findstr /I /C:"%PRESET_NAME%") > nul
if errorlevel 1 (
    echo Invalid preset: "%PRESET_NAME%"
    goto show_help
)

echo -- Building preset: %PRESET_NAME% --

echo -- Configuring...
cmake --preset %PRESET_NAME%
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

echo -- Building...
cmake --build --preset %PRESET_NAME%
if errorlevel 1 (
    echo CMake build failed.
    exit /b 1
)

echo -- Build for %PRESET_NAME% completed successfully. --
exit /b 0

:show_help
echo.
echo Usage: build.bat [preset_name]
echo.
echo Builds the project using the specified preset.
echo If no preset is provided, it defaults to 'default-debug'.
echo.
echo Available presets:
echo   default-debug
echo   default-release
echo   default-profile
echo   ninja-debug
echo   ninja-release
echo   ninja-profile
echo.
exit /b 1
