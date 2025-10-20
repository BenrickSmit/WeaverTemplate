#!/bin/sh

# Default to default-debug preset if no argument is provided
if [ -z "$1" ]; then
    echo "No preset specified, defaulting to default-debug."
    PRESET_NAME="default-debug"
else
    PRESET_NAME=$1
fi

# Handle help argument
if [ "$PRESET_NAME" = "--help" ] || [ "$PRESET_NAME" = "-h" ]; then
    echo ""
    echo "Usage: ./build.sh [preset_name]"
    echo ""
    echo "Builds the project using the specified preset."
    echo "If no preset is provided, it defaults to 'default-debug'."
    echo ""
    echo "Available presets:"
    echo "  default-debug"
    echo "  default-release"
    echo "  default-profile"
    echo "  ninja-debug"
    echo "  ninja-release"
    echo "  ninja-profile"
    echo ""
    exit 1
fi

# Validate preset name
case "$PRESET_NAME" in
    default-debug|default-release|default-profile|ninja-debug|ninja-release|ninja-profile)
        # Valid preset, continue
        ;;
    *)
        echo "Invalid preset: \"$PRESET_NAME\""
        echo "Use './build.sh --help' for available options."
        exit 1
        ;;
esac

echo "-- Building preset: $PRESET_NAME --"

echo "-- Configuring..."
cmake --preset "$PRESET_NAME"
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi

echo "-- Building..."
cmake --build --preset "$PRESET_NAME"
if [ $? -ne 0 ]; then
    echo "CMake build failed."
    exit 1
fi

echo "-- Build for $PRESET_NAME completed successfully. --"
exit 0
