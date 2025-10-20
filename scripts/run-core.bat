@echo off
set BUILD_TYPE=Debug
if NOT "%1"=="" (set BUILD_TYPE=%1)

deliverables\%BUILD_TYPE%\App.exe
