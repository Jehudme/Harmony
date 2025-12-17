# --- 1. Find Dependencies (Vcpkg installs these) ---

# --- 2. Gather Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "src/*.h")

# --- 3. Define the Executable ---
add_executable(HarmonyDev ${SOURCES} ${HEADERS})

# --- 4. Setup Includes ---
target_include_directories(HarmonyDev 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" 
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# --- 5. Link Standard Dependencies ---
target_link_libraries(HarmonyDev PRIVATE HarmonyCore RaylibRenderer)

# --- 6. Gestion des Ressources (Dynamique) ---
add_custom_command(TARGET HarmonyDev POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Copying resources from HarmonyDev to Build Root..."
)