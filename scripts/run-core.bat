@echo off
set BUILD_TYPE=debug
if NOT "%1"=="" (set BUILD_TYPE=%1)

deliverables\%BUILD_TYPE%\Weaver.exe