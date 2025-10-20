@echo off
setlocal

echo -- Building Release configuration before creating installer...
call build.bat default-release
if errorlevel 1 (
    echo Release build failed. Cannot create installer.
    exit /b 1
)

echo -- Creating installer from build_Release directory... 
cd build_Release

rem CPack uses the CMAKE_BUILD_TYPE to find the config file, so we specify Release
cpack --preset default-release
if errorlevel 1 (
    echo CPack failed.
    cd ..
    exit /b 1
)

cd ..
echo -- Installer created successfully. --
exit /b 0
