# --------------------------------------------------------------------------
# SECTION: Project Setup
# --------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.31)

include(project_settings.cmake)

project(
  ${PROJECT_NAME}
  DESCRIPTION "Base Weaver GUI project setup."
  VERSION ${PROJECT_VERSION}
  LANGUAGES CXX
)

# Set CXX Standard used
set(CMAKE_CXX_STANDARD ${PROJECT_CPP_VERSION})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Further Project Details
set(PROJECT_COMPANY_NAME ${PROJECT_COMPANY_NAME})
set(PROJECT_COMPANY_NAMESPACE ${PROJECT_COMPANY_NAMESPACE})  # Reverse domain name notation


# --------------------------------------------------------------------------
# SECTION: Build Configuration Setup
# --------------------------------------------------------------------------

# Add a 'Profile' build configuration
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Profile" CACHE STRING "Semicolon-separated list of supported configuration types." FORCE)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, e.g. Debug, Release, MinSizeRel, RelWithDebInfo, Profile." FORCE)

# Set profiling flags for the 'Profile' build type
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -pg")
    set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -pg")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # For MSVC, basic profiling can be enabled with /Zi for debug info and /link /PROFILE for linker
    # More advanced profiling usually involves Visual Studio's built-in profiler or external tools.
    set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /link /PROFILE")
endif()

# Set the output directory for the final executable based on the build type.
# This places the executable in a top-level 'deliverables' directory.

# For multi-configuration generators, the configuration name is appended automatically.
# We want to ensure this is lowercase.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/deliverables/$<LOWER_CASE:$<CONFIG>>")

# --------------------------------------------------------------------------
# SECTION: File Target Setup
# --------------------------------------------------------------------------

# Enable Vulkan - Required for Dear ImGUI
find_package(Vulkan REQUIRED)

add_subdirectory(packaging)
set(ABSL_PROPAGATE_CXX_STD ON)

# Populate
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(tests)


# --------------------------------------------------------------------------
# SECTION: General Package Information
# --------------------------------------------------------------------------
# These settings apply to all generators (Windows, Linux, etc.)

# CPack configuration
# This section configures CPack for creating installers.

# Include this module to handle system libraries.
include(InstallRequiredSystemLibraries)

# The name of the package (and the installer file).
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")

# The vendor or author of the package.
set(CPACK_PACKAGE_VENDOR "Weaver")

# A short description of the package.
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Weaver GUI Project")

# The version of the package.
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

# Contact email for the package maintainer.
set(CPACK_PACKAGE_CONTACT "support@weaver.com")

# Set the output directory for the generated installer.
set(INSTALLER_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/deliverables/${CMAKE_BUILD_TYPE}/installer")
set(CPACK_PACKAGE_DIRECTORY "${INSTALLER_OUTPUT_PATH}")

# Optional: Path to a license file to include in the installer.
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# Optional: Path to an icon for the installer itself.
# set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.ico")


# --- Generator-Specific Settings ---

if(WIN32)
    # Use the NSIS (Nullsoft Scriptable Install System) generator on Windows.
    set(CPACK_GENERATOR "NSIS")

    # Optional: The name that appears in the installer window.
    # set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME} ${PROJECT_VERSION}")

    # Optional: The default installation directory.
    # set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")

    # Optional: Icon for the installer.
    # set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.ico")

    # Optional: Icon for the uninstaller.
    # set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.ico")

    # Optional: Command to sign the installer. Uncomment and configure after obtaining a code signing certificate.
    # Replace "Your Certificate Name" with the subject name of your code signing certificate.
    # Ensure signtool.exe (from Windows SDK) is in your PATH or provide its full path.
    # set(CPACK_NSIS_SIGN_COMMAND "signtool.exe sign /a /t http://timestamp.digicert.com /d "Weaver GUI Project" "<FILE>"")

else()
    # On Linux, generate both DEB (for Debian/Ubuntu) and RPM (for Fedora/CentOS) packages.
    set(CPACK_GENERATOR "DEB;RPM")

    # --- Debian (DEB) Specific ---
    # Optional: Maintainer information for the Debian package.
    # set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your.email@example.com>")

    # Optional: A more detailed description for the Debian package.
    # set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A longer description of the Weaver GUI Project.")

    # --- Red Hat (RPM) Specific ---
    # Optional: The release number of the RPM package.
    # set(CPACK_RPM_PACKAGE_RELEASE "1")

    # Optional: A short summary for the RPM package.
    # set(CPACK_RPM_PACKAGE_SUMMARY "Weaver GUI Project")

endif()

# This command includes the CPack module, which makes the `cpack` command available.
include(CPack)
