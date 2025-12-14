# Sandbox Application CMakeLists.txt

# Create the executable
add_executable(Sandbox main.cpp)

# Set C++ standard
set_target_properties(Sandbox PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Link to Harmony library
target_link_libraries(Sandbox
    PRIVATE
        Harmony
)

# Copy resources to build directory
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/properties.json"
        "$<TARGET_FILE_DIR:Sandbox>/properties.json"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/car.glb"
        "$<TARGET_FILE_DIR:Sandbox>/car.glb"
)
