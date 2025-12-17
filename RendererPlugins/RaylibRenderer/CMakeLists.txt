# --- 1. Find Dependencies (Vcpkg installs these) ---
find_package(raylib CONFIG REQUIRED)

# --- 2. Gather Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "src/*.h")

set(R3D_ASSIMP_VENDORED OFF CACHE BOOL "Use system assimp" FORCE)
set(R3D_RAYLIB_VENDORED OFF CACHE BOOL "Use system raylib" FORCE)

# --- 3. Define the Library ---
add_library(RaylibRenderer STATIC ${SOURCES} ${HEADERS})

# --- 4. Setup Includes ---
target_include_directories(RaylibRenderer 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" 
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# --- 5. Link Standard Dependencies ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/r3d/CMakeLists.txt")
    add_subdirectory(external/r3d)
    target_link_libraries(RaylibRenderer PRIVATE r3d) # R3D is internal to the engine
else()
    message(WARNING "R3D submodule not found! Did you run 'git submodule update --init'?")
endif()

target_link_libraries(RaylibRenderer PRIVATE raylib r3d HarmonyCore)

# --- 6. Gestion des Ressources (Dynamique) ---
add_custom_command(TARGET RaylibRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Copying resources from RaylibRenderer to Build Root..."
)