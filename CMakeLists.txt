cmake_minimum_required(VERSION 3.20)

# ==============================================================================
# 1. PRE-PROJECT SETUP (Git & Vcpkg)
# ==============================================================================

# --- A. Smart Git Submodule Update ---
find_package(Git QUIET)
set(VCPKG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

# Check if vcpkg appears empty (count files)
file(GLOB VCPKG_FILES "${VCPKG_DIR}/*")
list(LENGTH VCPKG_FILES VCPKG_FILE_COUNT)

if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    # Native Git Environment
    if(VCPKG_FILE_COUNT EQUAL 0)
        message(STATUS "Git: 'vcpkg' folder is empty. Initializing submodules...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_RESULT
        )
        if(NOT GIT_RESULT EQUAL "0")
            message(FATAL_ERROR "Git submodule update failed.")
        endif()
    endif()
elseif(VCPKG_FILE_COUNT EQUAL 0)
    # Remote Build / No Git Environment (Critical Error)
    message(FATAL_ERROR 
        "CRITICAL: 'vcpkg' folder is empty and no Git repository found.\n"
        "If you are using VS Remote Build, ensure submodules are updated LOCALLY first."
    )
endif()

# --- B. Vcpkg Bootstrapping ---
if(WIN32)
    set(VCPKG_EXEC "${VCPKG_DIR}/vcpkg.exe")
    set(VCPKG_BOOTSTRAP "${VCPKG_DIR}/bootstrap-vcpkg.bat")
else()
    set(VCPKG_EXEC "${VCPKG_DIR}/vcpkg")
    set(VCPKG_BOOTSTRAP "${VCPKG_DIR}/bootstrap-vcpkg.sh")
endif()

# Run bootstrap if vcpkg.exe is missing or older than the script
if(NOT EXISTS "${VCPKG_EXEC}" OR "${VCPKG_BOOTSTRAP}" IS_NEWER_THAN "${VCPKG_EXEC}")
    message(STATUS "Vcpkg: Bootstrapping executable...")
    execute_process(COMMAND "${VCPKG_BOOTSTRAP}" -disableMetrics WORKING_DIRECTORY "${VCPKG_DIR}")
endif()

# --- C. Toolchain Setup ---
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# ==============================================================================
# 2. PROJECT DEFINITION
# ==============================================================================
project("HarmonyProject")

# ==============================================================================
# 3. GLOBAL COMPILER SETTINGS
# ==============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Enables folders in VS Solution Explorer

# --- CRITICAL: Output Directory Setup ---
# Forces both the Engine (.exe) and Plugins (.dll) to build into the same /bin folder.
# This solves the issue of the Engine not finding the Plugin DLLs at runtime.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# This controls the build mode for EVERY sub-project
option(HARMONY_STATIC_LINK "Build fully static for Web/Release" OFF)

# Pass this CMake option to the C++ Code as a Preprocessor Definition
# This ensures '#ifdef HARMONY_STATIC_LINK' works in Base.h and your C++ files.
if(HARMONY_STATIC_LINK)
    add_compile_definitions(HARMONY_STATIC_LINK)
endif()

# ==============================================================================
# 4. EXTERNAL DEPENDENCIES (Common)
# ==============================================================================

# Python (Often needed for scripts/tools)
find_package(Python COMPONENTS Interpreter REQUIRED)

# Assimp (Asset Import Library)
find_package(assimp CONFIG REQUIRED)
# Fix for Assimp target naming inconsistencies in some Vcpkg versions
if(TARGET assimp::assimp AND NOT TARGET assimp)
    add_library(assimp ALIAS assimp::assimp)
endif()

# ==============================================================================
# 5. PROJECT SUBDIRECTORIES
# ==============================================================================

add_subdirectory("Core")

add_subdirectory("Harmony")
add_subdirectory("Plugins")