cmake_minimum_required (VERSION 3.20)

# ==============================================================================
# 1. SMART GIT UPDATE (Checks if 'vcpkg' folder is empty)
# ==============================================================================
find_package(Git QUIET)

set(VCPKG_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

# Check if the vcpkg folder is empty or missing content
# We look for *any* file inside the folder.
file(GLOB VCPKG_CONTENT "${VCPKG_SUBMODULE_DIR}/*")
list(LENGTH VCPKG_CONTENT VCPKG_CONTENT_COUNT)

# Logic: "Is it empty?"
if(VCPKG_CONTENT_COUNT EQUAL 0)
    set(VCPKG_IS_EMPTY TRUE)
else()
    set(VCPKG_IS_EMPTY FALSE)
endif()

if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    # --- SCENARIO A: Native Build (Windows/Local Linux) ---
    # If the folder is empty OR Cache is missing, we run the update.
    
    if(NOT VCPKG_IS_EMPTY AND GIT_INITIALIZED AND NOT FORCE_SUBMODULE_UPDATE)
        message(STATUS "Git: Optimization Active - Skipping update (Submodules detected).")
    else()
        message(STATUS "Git: Repository detected or 'vcpkg' is empty. Updating submodules...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE git_result
            OUTPUT_VARIABLE git_out
            ERROR_VARIABLE git_err
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
        )

        if(NOT git_result EQUAL "0")
            message(FATAL_ERROR "Git failed: ${git_err}")
        else()
            message(STATUS "Git: Submodules updated successfully.")
            set(GIT_INITIALIZED ON CACHE INTERNAL "Git state")
        endif()
    endif()

else()
    # --- SCENARIO B: Remote Build (VS Remote / CLion Remote) ---
    # The .git folder is missing, so we can't run git commands.
    
    if(NOT VCPKG_IS_EMPTY)
        message(STATUS "Git: Remote build detected. Vcpkg files found. Proceeding.")
    else()
        # CRITICAL WARNING: The folder is empty, but we can't fix it here.
        message(WARNING "----------------------------------------------------------------")
        message(WARNING "CRITICAL ERROR: 'vcpkg' folder is empty on the remote machine!")
        message(WARNING "Visual Studio did not copy the submodule files because they were missing locally.")
        message(WARNING "SOLUTION: Run 'git submodule update --init --recursive' on WINDOWS and rebuild.")
        message(WARNING "----------------------------------------------------------------")
        message(FATAL_ERROR "Stopping build because dependencies are missing.")
    endif()
endif()

# ==============================================================================
# 2. VCPKG SETUP
# ==============================================================================
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

if (WIN32)
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg.exe")
    set(BOOTSTRAP_CMD "${VCPKG_ROOT}/bootstrap-vcpkg.bat")
else()
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg")
    set(BOOTSTRAP_CMD "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
endif()

if (NOT EXISTS "${VCPKG_EXEC}" OR "${BOOTSTRAP_CMD}" IS_NEWER_THAN "${VCPKG_EXEC}")
    message(STATUS "Bootstrapping vcpkg...")
    execute_process(COMMAND "${BOOTSTRAP_CMD}" -disableMetrics WORKING_DIRECTORY "${VCPKG_ROOT}")
endif()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# ==============================================================================
# 3. VCPKG INSTALL (Runs ONLY if Cache is Missing/Cleaned)
# ==============================================================================
if(VCPKG_INITIALIZED)
    set(VCPKG_MANIFEST_INSTALL OFF)
    message(STATUS "Vcpkg: Optimization Active - Skipping dependency check (Cache exists).")
else()
    set(VCPKG_MANIFEST_INSTALL ON)
    message(STATUS "Vcpkg: Cache missing. Enabling dependency check...")
    set(VCPKG_INITIALIZED ON CACHE INTERNAL "Vcpkg dependencies checked")
endif()

# ==============================================================================
# 4. DEFINE PROJECT 
# ==============================================================================
project ("HarmonyProject")

# ==============================================================================
# 5. DEPENDENCIES
# ==============================================================================

find_package(Python COMPONENTS Interpreter REQUIRED)
set(PYTHON_EXECUTABLE "${Python_EXECUTABLE}" CACHE FILEPATH "Python executable" FORCE)

find_package(assimp CONFIG REQUIRED)
if(TARGET assimp::assimp)
    if(NOT TARGET assimp)
        add_library(assimp ALIAS assimp::assimp)
    endif()
endif()

# ==============================================================================
# 6. COMPILER SETTINGS
# ==============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ==============================================================================
# 7. ADD SUBDIRECTORIES
# ==============================================================================

add_subdirectory ("HarmonyDev")
add_subdirectory ("HarmonyCore")
add_subdirectory ("RendererPlugins")