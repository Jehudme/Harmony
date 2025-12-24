# --- 3. Gather Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "src/*.h")

# --- 4. Define the Executable ---
add_executable(Harmony ${SOURCES} ${HEADERS})

# --- 5. Configure Preprocessor ---
if(HARMONY_STATIC_LINK)
    target_compile_definitions(Harmony PRIVATE HARMONY_STATIC_LINK)
endif()

# --- 6. Setup Includes ---
target_include_directories(Harmony 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# --- 7. Link Libraries ---
target_link_libraries(Harmony PRIVATE HarmonyCore)

if(NOT HARMONY_STATIC_LINK)
    target_link_libraries(Harmony PRIVATE HarmonyCore)
endif()

# --- 8. Gestion des Ressources (Dynamique) ---
add_custom_command(TARGET Harmony POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying resources from Harmony Runner to Build Root..."
)