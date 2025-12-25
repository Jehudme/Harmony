cmake_minimum_required(VERSION 3.20)

# ========================================================
# Pre-Project Setup (Git & Vcpkg)
# ========================================================

find_package(Git QUIET)
set(VCPKG_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

file(GLOB VCPKG_FILES "${VCPKG_DIRECTORY}/*")
list(LENGTH VCPKG_FILES VCPKG_FILE_COUNT)

if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
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
    message(FATAL_ERROR 
        "CRITICAL: 'vcpkg' folder is empty and no Git repository found.\n"
        "If you are using VS Remote Build, ensure submodules are updated LOCALLY first."
    )
endif()

# ========================================================
# Vcpkg Bootstrapping
# ========================================================

if(WIN32)
    set(VCPKG_EXECUTABLE "${VCPKG_DIRECTORY}/vcpkg.exe")
    set(VCPKG_BOOTSTRAP "${VCPKG_DIRECTORY}/bootstrap-vcpkg.bat")
else()
    set(VCPKG_EXECUTABLE "${VCPKG_DIRECTORY}/vcpkg")
    set(VCPKG_BOOTSTRAP "${VCPKG_DIRECTORY}/bootstrap-vcpkg.sh")
endif()

if(NOT EXISTS "${VCPKG_EXECUTABLE}" OR "${VCPKG_BOOTSTRAP}" IS_NEWER_THAN "${VCPKG_EXECUTABLE}")
    message(STATUS "Vcpkg: Bootstrapping executable...")
    execute_process(COMMAND "${VCPKG_BOOTSTRAP}" -disableMetrics WORKING_DIRECTORY "${VCPKG_DIRECTORY}")
endif()

# ========================================================
# Toolchain Setup
# ========================================================

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIRECTORY}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# ========================================================
# Project Definition
# ========================================================

project("HarmonyProject")

# ========================================================
# Global Compiler Settings
# ========================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ========================================================
# Output Directory Configuration
# ========================================================

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ========================================================
# Build Options
# ========================================================

option(HARMONY_STATIC_LINK "Build fully static for Web/Release" OFF)

if(HARMONY_STATIC_LINK)
    add_compile_definitions(HARMONY_STATIC_LINK)
endif()

# ========================================================
# External Dependencies
# ========================================================

find_package(Python COMPONENTS Interpreter REQUIRED)
find_package(assimp CONFIG REQUIRED)

if(TARGET assimp::assimp AND NOT TARGET assimp)
    add_library(assimp ALIAS assimp::assimp)
endif()

# ========================================================
# Project Subdirectories
# ========================================================

add_subdirectory("Harmony")
add_subdirectory("HarmonyCore")
add_subdirectory("HarmonyPlugins")