cmake_minimum_required (VERSION 3.20)

# --- 1. SETUP PATHS ---
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

# --- 2. DETECT OS & SCRIPTS ---
if (WIN32)
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg.exe")
    set(BOOTSTRAP_CMD "${VCPKG_ROOT}/bootstrap-vcpkg.bat")
else()
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg")
    set(BOOTSTRAP_CMD "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
endif()

# --- 3. AUTO-BOOTSTRAP IF MISSING ---
if (NOT EXISTS "${VCPKG_EXEC}")
    message(STATUS "Bootstrapping vcpkg...")
    execute_process(COMMAND "${BOOTSTRAP_CMD}" -disableMetrics WORKING_DIRECTORY "${VCPKG_ROOT}")
endif()

# --- 4. FORCE VCPKG TOOLCHAIN ---
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# --- 5. DEFINE PROJECT ---
project ("HarmonyProject")

# --- 6. DEPENDENCIES ---

# FIX A: Python (Required for R3D)
find_package(Python COMPONENTS Interpreter REQUIRED)
set(PYTHON_EXECUTABLE "${Python_EXECUTABLE}" CACHE FILEPATH "Python executable" FORCE)

# FIX B: Link Assimp Manually
find_package(assimp CONFIG REQUIRED)

# Alias Assimp so R3D can see it
if(TARGET assimp::assimp)
    if(NOT TARGET assimp)
        add_library(assimp ALIAS assimp::assimp)
    endif()
endif()

# --- 7. R3D CONFIGURATION ---
# Use the System (Vcpkg) libraries.
set(R3D_ASSIMP_VENDORED OFF CACHE BOOL "Use system assimp" FORCE)
set(R3D_RAYLIB_VENDORED OFF CACHE BOOL "Use system raylib" FORCE)

# --- 8. STANDARD SETUP ---
# Change your standard setup to this:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# If using MSVC (Visual Studio), manually force the latest experimental features
if(MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/std:c++latest>)
endif()

add_subdirectory ("Harmony")
add_subdirectory ("Sandbox")