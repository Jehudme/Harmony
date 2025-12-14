# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Harmony")

# Configure R3D to use vendored dependencies
set(R3D_ASSIMP_VENDORED ON CACHE BOOL "Use vendored assimp for R3D")
set(R3D_RAYLIB_VENDORED ON CACHE BOOL "Use vendored raylib for R3D")
set(R3D_BUILD_EXAMPLES OFF CACHE BOOL "Build R3D examples")
set(R3D_BUILD_DOCS OFF CACHE BOOL "Build R3D documentation")

# Include R3D subdirectory
add_subdirectory("Harmony/External/r3d")

# Include Harmony library
add_subdirectory("Harmony")

# Include Sandbox application
add_subdirectory("Sandbox")
