# Harmony Engine - API Quick Reference

## Overview

This guide provides a quick reference for the refactored Harmony Engine API. Most changes are internal (PImpl pattern), but some public APIs have changed.

## Include Structure

```cpp
#include <Harmony/pch.h>      // Precompiled header
#include <Harmony/Harmony.h>  // Main engine header
```

## Engine API

### Creating the Engine

```cpp
Harmony::Utilities::Configuration config;
config.load("configuration.json");

Harmony::Engine engine(config);
```

### Engine Control

```cpp
// Start the main loop
engine.start();

// Stop the engine
engine.stop();

// Check if running
bool running = engine.isRunning();

// Set target FPS
engine.setTargetFPS(60);

// Get delta time (in seconds as float)
float dt = engine.getDeltaTime();
```

### Accessing Managers

```cpp
// Task management
engine.taskManagement->submit(std::move(task));

// Scene management
auto scene = engine.sceneManagement->create(sceneId);

// State management
engine.stateManagement->push(stateId);
```

## Scene API

### Entity Management

```cpp
// Create entity
Harmony::Scenes::EntityID entityId = scene.createEntity(configuration);

// Destroy entity
scene.destroyEntity(entityId);

// Get component reference
auto& transform = scene.componentReference<Harmony::Components::Transform>(entityId);
auto& rectangle = scene.componentReference<Harmony::Components::Rectangle>(entityId);
```

### Scene Updates

```cpp
// Update is called internally by the engine
// If calling manually:
scene.update(deltaTimeInSeconds);  // float, not sf::Time
```

## Components

### Transform Component

```cpp
// Set position
transform.setPosition(100.0f, 200.0f);

// Get position
float x, y;
transform.getPosition(x, y);

// Set rotation (degrees)
transform.setRotation(45.0f);

// Get rotation
float angle = transform.getRotation();

// Set scale
transform.setScale(2.0f, 2.0f);

// Get scale
float sx, sy;
transform.getScale(sx, sy);

// Set origin
transform.setOrigin(50.0f, 50.0f);

// Get origin
float ox, oy;
transform.getOrigin(ox, oy);
```

### Rectangle Component

```cpp
// Rectangle is configured via Configuration system
// Still inherits from SFML (limitation to be addressed)
```

### Circle Component

```cpp
// Circle is configured via Configuration system
// Still inherits from SFML (limitation to be addressed)
```

## Configuration System

```cpp
Harmony::Utilities::Configuration config;

// Load from file
config.load("config.json");

// Save to file
config.save("output.json");

// Get value
auto width = config.get<unsigned int>({"window", "width"});
if (width.has_value()) {
    // Use width.value()
}

// Set value
config.set({"window", "width"}, 1920u);

// Get subsection
auto subsection = config.subsection({"entities", "player"});
if (subsection.has_value()) {
    // Use subsection.value()
}
```

## Logging

```cpp
// Initialize logger
Harmony::Utilities::Logger::initialize("Harmony.log");

// Log messages
HARMONY_TRACE("Trace message");
HARMONY_DEBUG("Debug message");
HARMONY_INFO("Info message");
HARMONY_WARN("Warning message");
HARMONY_ERROR("Error message");
HARMONY_CRITICAL("Critical message");

// Formatted logging
HARMONY_INFO("Player position: ({}, {})", x, y);

// Shutdown
Harmony::Utilities::Logger::shutdown();
```

## Task System

```cpp
class MyTask : public Harmony::Tasks::Task {
public:
    MyTask() : Task(/*priority=*/0, /*mode=*/SingleThreaded) {}
    
protected:
    void run() override {
        // Task implementation
        Harmony::Engine& engine = getEngine();
        // Use engine...
    }
};

// Submit task
engine.taskManagement->submit(std::make_unique<MyTask>());
```

## Exception Handling

```cpp
try {
    engine.start();
} catch (const Harmony::Exceptions::EngineError& e) {
    HARMONY_ERROR("Engine error: {}", e.what());
} catch (const Harmony::Exceptions::StartupStatesNotDefined& e) {
    HARMONY_ERROR("No startup states defined");
} catch (const std::exception& e) {
    HARMONY_ERROR("Unexpected error: {}", e.what());
}
```

## Configuration File Format

Example `configuration.json`:

```json
{
  "window": {
    "title": "Harmony Game",
    "width": 1280,
    "height": 720,
    "fps": 60
  },
  "startupStatesIds": [1],
  "states": {
    "1": {
      "scenes": [1]
    }
  },
  "scenes": {
    "1": {
      "entities": [1, 2]
    }
  },
  "entities": {
    "1": {
      "components": {
        "Transform": {
          "position": { "x": 100, "y": 100 },
          "rotation": 0,
          "scale": { "x": 1, "y": 1 },
          "origin": { "x": 0, "y": 0 }
        },
        "Rectangle": {
          "size": { "width": 100, "height": 50 },
          "fill_color": { "r": 255, "g": 0, "b": 0, "a": 255 },
          "outline_color": { "r": 0, "g": 0, "b": 0, "a": 255 },
          "outline_thickness": 2
        }
      }
    }
  }
}
```

## Best Practices

### 1. Use Forward Declarations

When declaring engine types in your code:

```cpp
// In your header:
namespace Harmony { class Engine; }
namespace Harmony::Scenes { class Scene; }

// Only include in .cpp files
#include <Harmony/Engine.h>
#include <Harmony/Scene.h>
```

### 2. Prefer EntityID Over Raw Types

```cpp
// Good
Harmony::Scenes::EntityID entity = scene.createEntity(config);

// Avoid (internal detail)
entt::entity entity = ...;  // Don't use entt types directly
```

### 3. Use Logger Macros

```cpp
// Good
HARMONY_INFO("Player spawned at ({}, {})", x, y);

// Avoid (bypasses configuration)
std::cout << "Player spawned" << std::endl;
```

### 4. Exception Safety

```cpp
// Always wrap engine operations
try {
    engine.start();
} catch (const std::exception& e) {
    HARMONY_ERROR("Fatal error: {}", e.what());
    return 1;
}
```

## Migration from Old API

### Delta Time

```cpp
// Old
sf::Time dt = engine.getDeltaTime();
float seconds = dt.asSeconds();

// New
float seconds = engine.getDeltaTime();
```

### Entity IDs

```cpp
// Old
entt::entity entity = scene.createEntity(config);

// New
Harmony::Scenes::EntityID entity = scene.createEntity(config);
```

### Scene Updates

```cpp
// Old (if calling directly)
scene.update(sf::seconds(1.0f/60.0f));

// New
scene.update(1.0f/60.0f);  // Just a float
```

## Known Limitations

1. **Shape Classes**: Rectangle and Circle still inherit from SFML types. This will be addressed in future updates.
2. **Template Exposure**: Scene.inl exposes entt types for template implementations.
3. **Direct SFML Access**: Some internal rendering still requires SFML knowledge.

## Getting Help

- See `REFACTORING_NOTES.md` for detailed changes
- See `ARCHITECTURE_ROADMAP.md` for planned improvements
- Check the source code for implementation details

## Version

This API reference is for Harmony Engine v1.0.0 with header refactoring updates (2024).
