cmake_minimum_required(VERSION 3.20)
project(RaylibWindow)

# ========================================================
# Dependencies
# ========================================================

find_package(raylib CONFIG REQUIRED)

# ========================================================
# Source Files
# ========================================================

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "src/*.h")

# ========================================================
# Library Definition
# ========================================================

add_library(RaylibWindow STATIC 
    ${SOURCES} 
    ${HEADERS}
)

set_target_properties(RaylibWindow PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ========================================================
# Include Directories
# ========================================================

target_include_directories(RaylibWindow 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PRIVATE "${CMAKE_SOURCE_DIR}/Core/include"
)

# ========================================================
# Link Libraries
# ========================================================

target_link_libraries(RaylibWindow 
    PRIVATE 
        HarmonyCore
        raylib
)
