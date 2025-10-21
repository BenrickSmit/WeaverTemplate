@echo off
setlocal

echo Verifying staged files with clang-format and clang-tidy...

set "CLANG_FORMAT_FOUND="
where clang-format >nul 2>nul
if %errorlevel% equ 0 set "CLANG_FORMAT_FOUND=true"

set "CLANG_TIDY_FOUND="
where clang-tidy >nul 2>nul
if %errorlevel% equ 0 set "CLANG_TIDY_FOUND=true"

if not defined CLANG_FORMAT_FOUND (
    echo Error: clang-format is not found. Please install LLVM/Clang and ensure it's in your PATH.
    exit /b 1
)

if not defined CLANG_TIDY_FOUND (
    echo Error: clang-tidy is not found. Please install LLVM/Clang and ensure it's in your PATH.
    exit /b 1
)

rem Get list of staged files
for /f "delims=" %%i in ('git diff --cached --name-only --diff-filter=ACM') do (
    set "FILE=%%i"
    call :process_file "%%FILE%%"
    if %errorlevel% neq 0 (
        echo Pre-commit hook failed.
        exit /b 1
    )
)

echo Pre-commit hook passed.
exit /b 0

:process_file
set "CURRENT_FILE=%~1"

rem Check if the file is a C++ source or header in src/App or src/Core
set "CHECK_FILE=false"
echo "%CURRENT_FILE%" | findstr /r /c:"^src/App/.*\\.\(cpp\|h\|hpp\|cxx\|hxx\)$" >nul
if %errorlevel% equ 0 set "CHECK_FILE=true"
echo "%CURRENT_FILE%" | findstr /r /c:"^src/Core/.*\\.\(cpp\|h\|hpp\|cxx\|hxx\)$" >nul
if %errorlevel% equ 0 set "CHECK_FILE=true"

if "%CHECK_FILE%"=="false" goto :eof

    echo Checking %CURRENT_FILE%

    echo Running clang-format on %CURRENT_FILE%
    clang-format -Werror --dry-run --Wno-error=unknown "%CURRENT_FILE%"
    if %errorlevel% neq 0 (
        echo clang-format check failed for %CURRENT_FILE%. Please fix formatting issues.
        exit /b 1
    )

    echo Running clang-tidy on %CURRENT_FILE%
    rem clang-tidy requires a compilation database (compile_commands.json).
    rem Ensure you have run a full CMake build (e.g., by running build.bat) for the relevant configuration
    rem before committing, so that compile_commands.json is up-to-date in your build directory (e.g., build_debug).
    clang-tidy "%CURRENT_FILE%" -- -p build_debug
    if %errorlevel% neq 0 (
        echo clang-tidy check failed for %CURRENT_FILE%. Please fix linting issues.
        exit /b 1
    )
goto :eof
