cmake_minimum_required (VERSION 3.20)

# ==============================================================================
# 1. EXECUTE FIRST: GIT SUBMODULE UPDATE
# ==============================================================================
find_program(GIT_EXECUTABLE git)

if(GIT_EXECUTABLE AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    message(STATUS "PRE-BUILD: Updating git submodules...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --force
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE git_result
        OUTPUT_VARIABLE git_out
        ERROR_VARIABLE git_err
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )
    if(NOT git_result EQUAL "0")
        message(FATAL_ERROR "Git failed: ${git_err}")
    else()
        message(STATUS "Git Updated Successfully.")
    endif()
endif()

# ==============================================================================
# 2. VCPKG SETUP
# ==============================================================================
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

if (WIN32)
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg.exe")
    set(BOOTSTRAP_CMD "${VCPKG_ROOT}/bootstrap-vcpkg.bat")
else()
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg")
    set(BOOTSTRAP_CMD "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
endif()

if (EXISTS "${VCPKG_EXEC}")
    if ("${BOOTSTRAP_CMD}" IS_NEWER_THAN "${VCPKG_EXEC}")
        message(STATUS "Vcpkg scripts updated. Re-building vcpkg.exe...")
        execute_process(COMMAND "${BOOTSTRAP_CMD}" -disableMetrics WORKING_DIRECTORY "${VCPKG_ROOT}")
    endif()
else()
    message(STATUS "Bootstrapping vcpkg...")
    execute_process(COMMAND "${BOOTSTRAP_CMD}" -disableMetrics WORKING_DIRECTORY "${VCPKG_ROOT}")
endif()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# ==============================================================================
# 3. DEFINE PROJECT (CRITICAL: Must be before checking MSVC)
# ==============================================================================
project ("HarmonyProject")

# ==============================================================================
# 4. GLOBAL FIXES (Now correctly placed)
# ==============================================================================

# FIX: MSVC C++20 Strictness vs nlohmann/json
# We force include <cassert> ONLY for C++ files to prevent the JSON crash.
if(MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/FIcassert>)
endif()

# ==============================================================================
# 5. DEPENDENCIES
# ==============================================================================

find_package(Python COMPONENTS Interpreter REQUIRED)
set(PYTHON_EXECUTABLE "${Python_EXECUTABLE}" CACHE FILEPATH "Python executable" FORCE)

find_package(assimp CONFIG REQUIRED)
if(TARGET assimp::assimp)
    if(NOT TARGET assimp)
        add_library(assimp ALIAS assimp::assimp)
    endif()
endif()

# ==============================================================================
# 6. COMPILER SETTINGS
# ==============================================================================

set(R3D_ASSIMP_VENDORED OFF CACHE BOOL "Use system assimp" FORCE)
set(R3D_RAYLIB_VENDORED OFF CACHE BOOL "Use system raylib" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Enable C++ Latest features ONLY for C++ files (Avoids C8016 error in C files)
if(MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/std:c++latest>)
endif()

# ==============================================================================
# 7. ADD SUBDIRECTORIES
# ==============================================================================
add_subdirectory ("Harmony")
add_subdirectory ("Sandbox")