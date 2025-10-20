#!/bin/sh
BUILD_TYPE="Debug"
if [ "$1" != "" ]; then
    BUILD_TYPE="$1"
fi

deliverables/$BUILD_TYPE/App
