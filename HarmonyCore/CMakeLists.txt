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
add_library(HarmonyCore STATIC ${SOURCES} ${HEADERS} "include/Harmony/TaskDispatcher.h" "include/Harmony/ITask.h" "src/TaskDispatcher.cpp" "src/internal/PrivateAccess.h")

# --- 4. Setup Includes ---
target_include_directories(HarmonyCore 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" 
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# --- 5. Link Standard Dependencies ---
target_link_libraries(HarmonyCore PUBLIC glm::glm fmt::fmt EnTT::EnTT PRIVATE spdlog::spdlog nlohmann_json::nlohmann_json)

# --- 6. Gestion des Ressources (Dynamique) ---
add_custom_command(TARGET HarmonyCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Copying resources from HarmonyCore to Build Root..."
)