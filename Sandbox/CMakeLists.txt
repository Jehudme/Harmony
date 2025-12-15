add_executable(Sandbox src/main.cpp)

# Link to your Engine
target_link_libraries(Sandbox PRIVATE Harmony)

# Copy Assets to the output folder so the game can find them
# This copies the 'assets' folder from Sandbox/assets to out/build/.../assets
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# For Web Builds: Tell Emscripten to preload the assets
if (EMSCRIPTEN)
    set_target_properties(Sandbox PROPERTIES
            LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets"
    )
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()