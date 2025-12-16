# --- 1. Find Dependencies (Vcpkg installs these) ---
find_package(raylib CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


# --- 2. Gather Source Files ---
# Note: For professional projects, listing files manually is safer,
# but GLOB is fine for migration.
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "src/*.h")

# --- 3. Define the Library ---
add_library(Harmony STATIC ${SOURCES} ${HEADERS})

# --- 4. Setup Includes ---
target_include_directories(Harmony
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" # Sandbox sees this
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"    # Internal use only
)

# --- 5. Link Standard Dependencies ---
target_link_libraries(Harmony PUBLIC
        raylib
        spdlog::spdlog
        EnTT::EnTT
        nlohmann_json::nlohmann_json
        fmt::fmt
)



# --- 6. R3D Integration (Desktop Only) ---
if (EMSCRIPTEN)
    message(STATUS "Building for WEB - R3D Disabled")
    target_compile_definitions(Harmony PUBLIC PLATFORM_WEB)
else()
    message(STATUS "Building for DESKTOP - R3D Enabled")
    target_compile_definitions(Harmony PUBLIC PLATFORM_DESKTOP)

    # Add R3D Submodule
    # Ensure R3D is located at Harmony/External/r3d
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/External/r3d/CMakeLists.txt")
        add_subdirectory(external/r3d)
        target_link_libraries(Harmony PRIVATE r3d) # R3D is internal to the engine
    else()
        message(WARNING "R3D submodule not found! Did you run 'git submodule update --init'?")
    endif()
endif()