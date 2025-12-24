cmake_minimum_required(VERSION 3.20)
project(HarmonyCore)

# --- 1. Find Dependencies (Vcpkg installs these) ---
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# --- 2. Gather Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "src/*.h")

# --- 3. Define the Library ---
# Note: Position Independent Code is required to link this Static Lib into a Shared DLL later
add_library(HarmonyCore STATIC ${SOURCES} ${HEADERS} "include/Harmony/PluginsFactoriesRegistry.h" "include/Harmony/PluginsRegistry.h" "include/Harmony/Exceptions.h")
set_target_properties(HarmonyCore PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Plugins will not inherit it (they should define their own).
target_precompile_headers(HarmonyCore PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Enables folders in VS Solution Explorer

# --- 4. Setup Includes ---
target_include_directories(HarmonyCore 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# --- 5. Link Standard Dependencies ---
# PUBLIC: Plugins that link HarmonyCore will also get access to these libs automatically
target_link_libraries(HarmonyCore 
    PUBLIC 
        glm::glm 
        fmt::fmt 
        EnTT::EnTT 
        spdlog::spdlog 
        nlohmann_json::nlohmann_json
)
