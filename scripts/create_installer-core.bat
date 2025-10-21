@echo off
setlocal

echo -- Building Release configuration before creating installer...
call build.bat default-release
if errorlevel 1 (
    echo Release build failed. Cannot create installer.
    exit /b 1
)

echo -- Creating installer from build_Release directory... 



where makensis >nul 2>nul

if %errorlevel% neq 0 goto :nsis_not_found



rem CPack uses the CMAKE_BUILD_TYPE to find the config file, so we specify Release

cpack --preset msvc-package --config "%~dp0..\build_Release\CPackConfig.cmake"

if %errorlevel% neq 0 goto :cpack_failed



echo -- Installer created successfully. --

exit /b 0



:nsis_not_found

echo Error: NSIS (Nullsoft Scriptable Install System) is not found.

echo Please install NSIS to create the installer.

echo You can download it from https://nsis.sourceforge.io/Download

exit /b 1



:cpack_failed

echo CPack failed.

exit /b 1
