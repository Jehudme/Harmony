cmake_minimum_required(VERSION 3.20)
project(HarmonyCore)

# ========================================================
# Dependencies
# ========================================================

find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# ========================================================
# Source Files
# ========================================================

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "src/*.h")

# ========================================================
# Library Definition
# ========================================================

add_library(HarmonyCore STATIC 
    ${SOURCES} 
    ${HEADERS}
 "include/Harmony/Plugins/SystemsRegistry.h" "src/SystemsRegistry.cpp")

set_target_properties(HarmonyCore PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ========================================================
# Precompiled Headers
# ========================================================

target_precompile_headers(HarmonyCore PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)

# ========================================================
# Include Directories
# ========================================================

target_include_directories(HarmonyCore 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# ========================================================
# Link Libraries
# ========================================================

target_link_libraries(HarmonyCore 
    PUBLIC 
        glm::glm 
        fmt::fmt 
        EnTT::EnTT 
        spdlog::spdlog 
        nlohmann_json::nlohmann_json
)
