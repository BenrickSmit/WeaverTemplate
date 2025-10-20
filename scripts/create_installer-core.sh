#!/bin/sh

echo "-- Building Release configuration before creating installer..."
./build.sh default-release
if [ $? -ne 0 ]; then
    echo "Release build failed. Cannot create installer."
    exit 1
fi

echo "-- Creating installer from build_release directory..."
cd build_release

# CPack uses the CMAKE_BUILD_TYPE to find the config file, so we specify Release
cpack --preset default-release
if [ $? -ne 0 ]; then
    echo "CPack failed."
    cd ..
    exit 1
fi

cd ..
echo "-- Installer created successfully. --"
exit 0
