# Harmony Engine Library CMakeLists.txt

# Collect all source files
set(HARMONY_SOURCES
    Clock.cpp
    Color.cpp
    ComponentsHandler.cpp
    Configuration.cpp
    Engine.cpp
    Exceptions.cpp
    IRenderableComponent.cpp
    Logger.cpp
    ModelIDComponent.cpp
    ModelRenderableComponent.cpp
    ModelResource.cpp
    OptionalReference.cpp
    Resource.cpp
    ResourceHandler.cpp
    Scene.cpp
    ScriptComponent.cpp
    Stopwatch.cpp
    Task.cpp
    TasksHandler.cpp
    Time.cpp
    Timer.cpp
    TransformComponent.cpp
    Transformable.cpp
    View3DComponent.cpp
    WindowHandler.cpp
    pch.cpp
)

# Collect all header files
set(HARMONY_HEADERS
    Assert.h
    Clock.h
    Color.h
    ComponentsHandler.h
    Configuration.h
    Engine.h
    Exceptions.h
    IRenderableComponent.h
    Logger.h
    ModelIDComponent.h
    ModelRenderableComponent.h
    ModelResource.h
    OptionalReference.h
    RandomValue.h
    Resource.h
    ResourceHandler.h
    Scene.h
    ScriptComponent.h
    Stopwatch.h
    Task.h
    TasksHandler.h
    Time.h
    Timer.h
    TransformComponent.h
    Transformable.h
    Vector.h
    View3DComponent.h
    WindowHandler.h
    pch.h
    type_name.h
)

# Create the static library
add_library(Harmony STATIC ${HARMONY_SOURCES} ${HARMONY_HEADERS})

# Set C++ standard
set_target_properties(Harmony PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Specify include directories
target_include_directories(Harmony
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/External/r3d/include
        ${CMAKE_CURRENT_SOURCE_DIR}/External/r3d/external/raylib/src
)

# Link R3D
target_link_libraries(Harmony
    PUBLIC
        r3d
)

# Find and link dependencies - try CONFIG mode first, then fall back to MODULE mode
find_package(spdlog QUIET CONFIG)
if(NOT spdlog_FOUND)
    find_package(spdlog REQUIRED)
endif()

find_package(EnTT QUIET CONFIG)
if(NOT EnTT_FOUND)
    # EnTT is header-only, try to find it as a system package
    find_path(ENTT_INCLUDE_DIRS NAMES entt/entt.hpp PATHS /tmp/entt/src)
    if(ENTT_INCLUDE_DIRS)
        message(STATUS "Found EnTT headers at: ${ENTT_INCLUDE_DIRS}")
    else()
        message(STATUS "EnTT not found in standard paths, trying /tmp/entt/src")
        set(ENTT_INCLUDE_DIRS "/tmp/entt/src")
    endif()
endif()

find_package(nlohmann_json QUIET CONFIG)
if(NOT nlohmann_json_FOUND)
    find_package(nlohmann_json REQUIRED)
endif()

find_package(fmt QUIET CONFIG)
if(NOT fmt_FOUND)
    find_package(fmt REQUIRED)
endif()

# Link dependencies
if(TARGET spdlog::spdlog)
    target_link_libraries(Harmony PUBLIC spdlog::spdlog)
elseif(TARGET spdlog)
    target_link_libraries(Harmony PUBLIC spdlog)
endif()

if(TARGET EnTT::EnTT)
    target_link_libraries(Harmony PUBLIC EnTT::EnTT)
elseif(ENTT_INCLUDE_DIRS)
    target_include_directories(Harmony PUBLIC ${ENTT_INCLUDE_DIRS})
endif()

if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(Harmony PUBLIC nlohmann_json::nlohmann_json)
endif()

if(TARGET fmt::fmt)
    target_link_libraries(Harmony PUBLIC fmt::fmt)
elseif(TARGET fmt)
    target_link_libraries(Harmony PUBLIC fmt)
endif()
