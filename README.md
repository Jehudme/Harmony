# Harmony Engine

A modern, multi-threaded C++ game engine built with SFML and EnTT, featuring a powerful task system, entity-component-system architecture, and comprehensive resource management.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
  - [Running Your First Application](#running-your-first-application)
- [Configuration](#configuration)
  - [Creating configuration.json](#creating-configurationjson)
  - [Configuration Structure](#configuration-structure)
- [Core Components](#core-components)
  - [Engine](#engine)
  - [Input System](#input-system)
  - [Configuration System](#configuration-system)
  - [Logger](#logger)
  - [Task System](#task-system)
  - [Scene Management](#scene-management)
  - [State Management](#state-management)
  - [Resource Management](#resource-management)
  - [Component System](#component-system)
- [API Reference](#api-reference)
  - [Components](#components)
  - [Tasks](#tasks)
  - [Utilities](#utilities)
- [Examples](#examples)
- [Architecture](#architecture)
- [License](#license)

## Overview

Harmony is a modern game engine designed for C++ developers who want a clean, thread-safe, and extensible framework for building 2D games. The engine leverages:

- **SFML** for graphics, audio, and window management
- **EnTT** for high-performance entity-component-system (ECS)
- **nlohmann/json** for configuration management
- **spdlog** for fast, asynchronous logging

**Version:** 1.0.0

## Features

### Core Features

- ✅ **Multi-threaded Task System**: Execute operations across three modes (SingleThreaded, FastMultiThreaded, SlowMultiThreaded)
- ✅ **Entity-Component-System (ECS)**: High-performance entity management using EnTT
- ✅ **Input System**: Comprehensive input handling with virtual input support for keyboard, mouse, and custom events
- ✅ **Scene & State Management**: Organize your game with scenes and states, including draw order support
- ✅ **Resource Management**: Efficient loading and caching of textures, fonts, sounds, and music
- ✅ **Configuration-Driven**: JSON-based configuration for all game objects and settings
- ✅ **Advanced Logging**: Multi-level logging with file rotation and async writing
- ✅ **Thread-Safe**: All managers are designed for concurrent access
- ✅ **Scripting Support**: Attach custom behavior to entities via script components

### Component System

Built-in components include:
- **Transform**: Position, rotation, scale, and origin
- **Circle**: Circular shape rendering with texture support
- **Rectangle**: Rectangular shape rendering with texture support
- **Text**: Text rendering with font support
- **Script**: Custom behavior attachment
- **Sprite**: Efficient texture rendering with sprite sheet animation support
- **Camera**: View control and viewport management for scene rendering
- **Audio**: Sound and music playback attached to entities for spatial audio
- **ParticleEmitter**: Particle system for visual effects (explosions, fire, smoke, etc.)
- **PhysicsBody**: Basic physics simulation with velocity, forces, and mass
- **Collision**: Collision detection and response with shape and layer support
- **Timer**: Entity-specific timing, delays, and periodic events
- **StateMachine**: Entity state management with transition callbacks

See [NEW_COMPONENTS_DOCUMENTATION.md](NEW_COMPONENTS_DOCUMENTATION.md) for detailed documentation on the 8 new components.

### Task System

Over 90 pre-built task types for:
- Entity management (create, destroy, batch operations)
- Scene management (create, delete, enable/disable drawing/updating, reset)
- State management (push, pop, switch, clear)
- Resource management (load, unload, batch loading)
- Engine control (set FPS, stop engine, delayed actions)
- Component control (sprite animation, camera control, audio playback, particle effects, physics forces, collision, timers, state machines)
- Utility operations (callbacks, lambdas, conditionals, repeated actions)

## Getting Started

### Prerequisites

- **Operating System**: Windows (Visual Studio project included)
- **Compiler**: Visual Studio 2019 or later (C++20 support required)
- **Dependencies** (managed via vcpkg):
  - SFML 2.6+
  - EnTT 3.12+
  - nlohmann-json 3.11+
  - spdlog 1.11+
  - fmt 10.0+

### Building the Project

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Jehudme/Harmony.git
   cd Harmony
   ```

2. **Open the solution**:
   - Open `Harmony.sln` in Visual Studio

3. **Restore dependencies**:
   - Dependencies are managed via vcpkg configuration
   - Visual Studio should automatically restore packages

4. **Build the solution**:
   - Select your configuration (Debug or Release)
   - Build → Build Solution (Ctrl+Shift+B)

### Running Your First Application

1. **Set up your configuration file** (see [Configuration](#configuration) section below)

2. **Create a simple main.cpp**:
   ```cpp
   #include "pch.h"
   #include "Engine.h"
   #include "Configuration.h"
   #include "Logger.h"

   int main() {
       // Initialize the logger
       Harmony::Utilities::Logger::initialize("Harmony.log");

       // Load configuration
       Harmony::Utilities::Configuration configuration;
       configuration.load("configuration.json");

       // Create and start the engine
       Harmony::Engine engine(configuration);
       engine.start();

       return 0;
   }
   ```

3. **Run the application**:
   - Press F5 to run with debugging, or Ctrl+F5 to run without debugging

## Configuration

### Creating configuration.json

The `configuration.json` file is the heart of your Harmony application. It defines window settings, states, scenes, entities, and resources.

### Configuration Structure

Here's a complete example of a `configuration.json` file:

```json
{
  "window": {
    "title": "My Harmony Game",
    "width": 1280,
    "height": 720,
    "fps": 60
  },
  "startupStatesIds": [1],
  "states": {
    "1": {
      "scenes": [100, 101]
    }
  },
  "scenes": {
    "100": {
      "drawOrder": 0,
      "entities": [1000, 1001]
    },
    "101": {
      "drawOrder": 10,
      "entities": [2000]
    }
  },
  "entities": {
    "1000": {
      "name": "Player",
      "script": "PlayerScript",
      "components": {
        "Transform": {
          "position": {"x": 100, "y": 150},
          "rotation": 0,
          "scale": {"x": 1, "y": 1}
        },
        "Circle": {
          "texture": 5000,
          "width": 50,
          "height": 50,
          "fill_color": {"r": 255, "g": 255, "b": 255, "a": 255},
          "outline_color": {"r": 0, "g": 0, "b": 0, "a": 255},
          "outline_thickness": 2
        }
      }
    },
    "2000": {
      "name": "UIText",
      "components": {
        "Transform": {
          "position": {"x": 50, "y": 50},
          "rotation": 0,
          "scale": {"x": 1, "y": 1}
        },
        "Text": {
          "font": 6000,
          "string": "Score: 0",
          "character_size": 24,
          "style": 0,
          "outline_thickness": 0,
          "fill_color": {"r": 255, "g": 255, "b": 255, "a": 255},
          "outline_color": {"r": 0, "g": 0, "b": 0, "a": 255}
        }
      }
    }
  },
  "resources": {
    "texture": {
      "5000": {
        "filepath": "assets/player.png"
      }
    },
    "font": {
      "6000": {
        "filepath": "assets/arial.ttf"
      }
    },
    "sound": {
      "7000": {
        "filepath": "assets/jump.wav"
      }
    },
    "music": {
      "8000": {
        "filepath": "assets/background_music.ogg"
      }
    }
  }
}
```

### Configuration Sections Explained

#### Window Configuration

```json
"window": {
  "title": "Window Title",     // String: Window title text
  "width": 1280,                // Integer: Window width in pixels
  "height": 720,                // Integer: Window height in pixels
  "fps": 60                     // Integer: Target frames per second
}
```

#### Startup States

```json
"startupStatesIds": [1, 2]      // Array of state IDs to load on startup
```

#### States

States represent high-level game states (menu, gameplay, pause, etc.):

```json
"states": {
  "1": {
    "scenes": [100, 101, 102]   // Array of scene IDs in this state
  }
}
```

#### Scenes

Scenes contain entities and have a draw order for layered rendering:

```json
"scenes": {
  "100": {
    "drawOrder": 0,             // Integer: Rendering order (lower = background)
    "entities": [1000, 1001]    // Array of entity IDs in this scene
  }
}
```

**Draw Order Values:**
- `-20` to `-10`: Far backgrounds
- `0`: Default game world layer
- `10` to `20`: Effects and particles
- `30` to `50`: UI elements
- `100+`: Debug overlays

#### Entities

Entities are game objects composed of components:

```json
"entities": {
  "1000": {
    "name": "EntityName",       // String: Human-readable name
    "script": "ScriptName",     // Optional: Script component class name
    "components": {
      "ComponentType": { ... }  // Component configuration
    }
  }
}
```

#### Resources

Resources are loaded and cached by the engine:

```json
"resources": {
  "texture": {
    "5000": {"filepath": "path/to/texture.png"}
  },
  "font": {
    "6000": {"filepath": "path/to/font.ttf"}
  },
  "sound": {
    "7000": {"filepath": "path/to/sound.wav"}
  },
  "music": {
    "8000": {"filepath": "path/to/music.ogg"}
  }
}
```

**Resource Types:**
- `texture`: Image files (PNG, JPG, BMP, etc.)
- `font`: TrueType fonts (TTF)
- `sound`: Short audio clips (WAV, OGG)
- `music`: Background music (OGG, FLAC)

## Core Components

### Engine

The `Engine` class is the heart of Harmony, managing the game loop, rendering, and all subsystems.

**Key Methods:**

```cpp
// Constructor
Engine(Utilities::Configuration& configuration);

// Lifecycle
void start();                          // Start the game loop
void stop();                           // Stop the game loop
bool isRunning() const noexcept;      // Check if engine is running

// FPS Control
void setTargetFPS(unsigned int fps);
unsigned int getTargetFPS() const noexcept;

// Timing
float getDeltaTime() const noexcept;  // Get delta time in seconds
```

**Member Managers:**

```cpp
std::unique_ptr<Management::ResourceManager> resourceManager;
std::unique_ptr<Management::TaskManager> taskManagement;
std::unique_ptr<Management::SceneManager> sceneManagement;
std::unique_ptr<Management::StateManager> stateManagement;
std::unique_ptr<Management::ComponentManager> componentManagement;
std::unique_ptr<InputManager> inputManager;
```

### Input System

The `InputManager` class provides comprehensive input handling for keyboard, mouse, and supports virtual input injection for testing and automation.

**Key Features:**

- Custom input enums independent of SFML for flexibility
- Per-frame input state tracking (Pressed, Held, Released)
- Mouse position and wheel tracking
- Virtual input injection for simulating user input
- Clear event handling through switch-based processing

**Input Enums:**

```cpp
namespace Harmony::Input {
    // Keyboard keys: Key::A, Key::Escape, Key::F1, etc.
    enum class Key { ... };
    
    // Mouse buttons: MouseButton::Left, MouseButton::Right, etc.
    enum class MouseButton { ... };
    
    // Mouse wheel: MouseWheel::Vertical, MouseWheel::Horizontal
    enum class MouseWheel { ... };
    
    // Input states: InputState::Released, InputState::Pressed, InputState::Held
    enum class InputState { ... };
}
```

**Keyboard Query Functions:**

```cpp
// Check if a key was just pressed this frame
bool isKeyPressed(Input::Key key) const;

// Check if a key was just released this frame
bool isKeyReleased(Input::Key key) const;

// Check if a key is currently held down (pressed or held state)
bool isKeyHeld(Input::Key key) const;
```

**Mouse Query Functions:**

```cpp
// Check if a mouse button was just pressed this frame
bool isMouseButtonPressed(Input::MouseButton button) const;

// Check if a mouse button was just released this frame
bool isMouseButtonReleased(Input::MouseButton button) const;

// Check if a mouse button is currently held down
bool isMouseButtonHeld(Input::MouseButton button) const;

// Get current mouse position
void getMousePosition(int& x, int& y) const;
std::pair<int, int> getMousePosition() const;

// Get mouse wheel scroll delta for this frame
float getMouseWheelDelta(Input::MouseWheel wheel) const;
```

**Virtual Input Functions:**

```cpp
// Simulate keyboard input
void simulateKeyPress(Input::Key key);
void simulateKeyRelease(Input::Key key);

// Simulate mouse input
void simulateMouseButtonPress(Input::MouseButton button);
void simulateMouseButtonRelease(Input::MouseButton button);
void simulateMouseMove(int x, int y);
void simulateMouseWheelScroll(Input::MouseWheel wheel, float delta);

// Clear all input states (useful for state transitions)
void clearAllStates();
```

**Usage Examples:**

```cpp
// Example 1: Basic keyboard input in a script
void PlayerScript::onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    auto& transform = getScene().componentReference<Transform>(entityId);
    
    float speed = 200.0f * getScene().engine.getDeltaTime();
    float x, y;
    transform.getPosition(x, y);
    
    // Movement with WASD keys
    if (input->isKeyHeld(Harmony::Input::Key::W)) {
        y -= speed;
    }
    if (input->isKeyHeld(Harmony::Input::Key::S)) {
        y += speed;
    }
    if (input->isKeyHeld(Harmony::Input::Key::A)) {
        x -= speed;
    }
    if (input->isKeyHeld(Harmony::Input::Key::D)) {
        x += speed;
    }
    
    transform.setPosition(x, y);
    
    // Jump action on space press (only once per press)
    if (input->isKeyPressed(Harmony::Input::Key::Space)) {
        HARMONY_INFO("Jump!");
        // Trigger jump logic
    }
}

// Example 2: Mouse input
void onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    
    // Get mouse position
    auto [mouseX, mouseY] = input->getMousePosition();
    
    // Check for mouse clicks
    if (input->isMouseButtonPressed(Harmony::Input::MouseButton::Left)) {
        HARMONY_INFO("Left click at ({}, {})", mouseX, mouseY);
        // Handle click at position
    }
    
    // Check mouse wheel
    float wheelDelta = input->getMouseWheelDelta(Harmony::Input::MouseWheel::Vertical);
    if (wheelDelta != 0.0f) {
        HARMONY_INFO("Mouse wheel scrolled: {}", wheelDelta);
        // Zoom in/out based on wheel delta
    }
}

// Example 3: Virtual input for automated testing
void RunAutomatedTest(Harmony::Engine& engine) {
    auto& input = engine.inputManager;
    
    // Simulate player movement
    input->simulateKeyPress(Harmony::Input::Key::W);
    // ... wait some frames ...
    input->simulateKeyRelease(Harmony::Input::Key::W);
    
    // Simulate mouse click
    input->simulateMouseMove(100, 200);
    input->simulateMouseButtonPress(Harmony::Input::MouseButton::Left);
    // ... next frame ...
    input->simulateMouseButtonRelease(Harmony::Input::MouseButton::Left);
}

// Example 4: Input state clearing on state transition
void TransitionToNewState(Harmony::Engine& engine) {
    // Clear all input states to prevent carryover
    engine.inputManager->clearAllStates();
    
    // Switch to new state
    auto task = std::make_unique<Harmony::Tasks::SwitchStateTask>(newStateId);
    engine.taskManagement->submit(std::move(task));
}
```

**Input State Behavior:**

- **Pressed**: True only on the first frame when the input is activated
- **Held**: True for all subsequent frames while the input remains active
- **Released**: True only on the frame when the input is deactivated

This design allows distinguishing between "just pressed" (for one-time actions like jumping) and "held down" (for continuous actions like movement).

**Event Processing:**

The InputManager automatically processes all SFML events forwarded from the Engine's event loop. The Engine uses a clear switch statement to handle different event types:

```cpp
void Engine::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Forward all events to InputManager
        inputManager->handleEvent(event);
        
        // Handle engine-specific events
        switch (event.type) {
            case sf::Event::Closed:
                stop();
                break;
            case sf::Event::Resized:
                // Handle window resize
                break;
            // ... other event types ...
        }
    }
    
    // Update input states (transition Pressed to Held)
    inputManager->updateStates();
}
```

### Configuration System

The `Configuration` class provides thread-safe access to JSON configuration data.

**Key Methods:**

```cpp
// Loading and Saving
void load(const std::filesystem::path& filePath);
void save(const std::filesystem::path& filePath);
void merge(const Configuration& configuration);

// Accessing Values
template<typename T>
std::optional<T> get(const std::vector<std::string>& keys) const;

template<typename T>
void set(const std::vector<std::string>& keys, const T& value);

// Navigation
std::optional<Configuration> subsection(const std::vector<std::string>& keys) const;
std::vector<std::string> extractKeys(const std::vector<std::string>& keys) const;

// Debugging
void debugPrint() const;
```

**Usage Example:**

```cpp
Harmony::Utilities::Configuration config;
config.load("configuration.json");

// Get a value
auto width = config.get<int>({"window", "width"});
if (width) {
    std::cout << "Window width: " << *width << std::endl;
}

// Set a value
config.set<int>({"window", "fps"}, 144);

// Get a subsection
auto playerConfig = config.subsection({"entities", "1000"});
```

**Supported Types:**
- Primitives: `int`, `unsigned int`, `int64_t`, `uint64_t`, `float`, `double`, `bool`, `std::string`
- Vectors: `std::vector<T>` of any supported primitive type

### Logger

The `Logger` class provides fast, asynchronous logging with multiple levels.

**Initialization:**

```cpp
Harmony::Utilities::Logger::initialize(
    "Harmony.log",           // Log file path
    10 * 1024 * 1024,       // Max file size (10 MB)
    3,                       // Max number of files
    1 << 15,                // Queue size
    1                        // Worker threads
);
```

**Log Levels:**

```cpp
HARMONY_TRACE("Detailed trace message");
HARMONY_DEBUG("Debug information");
HARMONY_INFO("General information");
HARMONY_WARN("Warning message");
HARMONY_ERROR("Error message");
HARMONY_CRITICAL("Critical error");
```

**Formatted Logging:**

```cpp
HARMONY_INFO("Player health: {}, position: ({}, {})", health, x, y);
HARMONY_ERROR("Failed to load resource: {}", filename);
```

### Task System

The task system enables multi-threaded operations with priority-based execution.

**Task Modes:**

```cpp
enum Mode {
    SingleThreaded,        // Executes on main thread
    FastMultiThreaded,     // Executes on worker thread pool
    SlowMultiThreaded      // Executes on dedicated detached thread
};
```

**Creating and Submitting Tasks:**

```cpp
// Create a task
auto task = std::make_unique<Harmony::Tasks::CreateEntityTask>(sceneId, entityConfig);

// Submit to task manager
engine.taskManagement->submit(std::move(task));
```

**Task Priority:**
- `0`: Immediate execution (use sparingly)
- `1-49`: Critical priority
- `50-99`: High priority
- `100-149`: Normal priority (default for most tasks)
- `150-199`: Low priority
- `200+`: Very low priority

See the [Tasks](#tasks) section for a complete list of available tasks.

### Scene Management

Scenes represent levels, areas, or logical groupings of entities.

**Scene Class:**

```cpp
class Scene {
public:
    Engine& engine;
    const Utilities::UUID sceneId;
    const int drawOrder;  // Rendering order

    // Entity Management
    EntityID createEntity(const Utilities::Configuration& configuration);
    void destroyEntity(EntityID entityId);

    // Component Access
    template<typename Type>
    Type& componentReference(EntityID entityId);

    // Scene Control
    void enableDrawing();
    void disableDrawing();
    bool isDrawingEnabled() const noexcept;

    void enableUpdating();
    void disableUpdating();
    bool isUpdatingEnabled() const noexcept;

    void reset();        // Reset to initial configuration
    void initialize();   // Initialize entities
};
```

**Usage Example:**

```cpp
// Access a scene through the scene manager
auto scene = engine.sceneManagement->getScene(sceneId);

// Create an entity
auto entityId = scene->createEntity(entityConfig);

// Access a component
auto& transform = scene->componentReference<Harmony::Components::Transform>(entityId);
transform.setPosition(100.0f, 200.0f);

// Pause scene updates (useful for pause menus)
scene->disableUpdating();
```

### State Management

States organize scenes into logical game states (menu, gameplay, pause, etc.).

**State Class:**

```cpp
class State {
public:
    Engine& engine;

    // Scene Management
    void addScene(std::shared_ptr<Scene> scene);
    void removeScene(Utilities::UUID sceneId);
};
```

**State Stack:**

States are managed as a stack, allowing for easy push/pop operations:

```cpp
// Push a new state (e.g., open pause menu)
auto task = std::make_unique<Harmony::Tasks::PushState>(pauseStateId);
engine.taskManagement->submit(std::move(task));

// Pop current state (e.g., close pause menu)
auto task = std::make_unique<Harmony::Tasks::PopState>();
engine.taskManagement->submit(std::move(task));

// Switch states (pop current, push new)
auto task = std::make_unique<Harmony::Tasks::SwitchStateTask>(newStateId);
engine.taskManagement->submit(std::move(task));
```

### Resource Management

The resource manager handles loading, caching, and unloading of game assets.

**Resource Types:**
- Textures (`texture`)
- Fonts (`font`)
- Sounds (`sound`)
- Music (`music`)

**ResourceManager Class:**

```cpp
class ResourceManager {
public:
    // Get a loaded resource
    Resources::Resource& get(const std::string& type, const Utilities::UUID resourceId);

    // Load a resource
    void load(const std::string& type, const Utilities::UUID resourceId);

    // Unload resources
    void unload(const Utilities::UUID resourceId);
    void unloadAll();
};
```

**Usage Example:**

```cpp
// Load a texture
engine.resourceManager->load("texture", 5000);

// Get a loaded texture
auto& resource = engine.resourceManager->get("texture", 5000);
auto& texture = convert<Harmony::Resources::Texture>(resource);

// Unload when done
engine.resourceManager->unload(5000);
```

**Best Practices:**
- Load resources asynchronously using `LoadResourceTask` or `BatchLoadResourcesTask`
- Unload resources when transitioning between major game states
- Keep frequently used resources loaded

### Component System

Components add functionality to entities. All components are configured via JSON.

**Built-in Components:**
- `Transform`: Position, rotation, scale, origin
- `Circle`: Circular shape rendering
- `Rectangle`: Rectangular shape rendering
- `Text`: Text rendering
- `Script`: Custom behavior

See the [Components](#components) section for detailed component reference.

## API Reference

### Components

#### Transform Component

Controls entity position, rotation, and scale.

**Configuration:**

```json
"Transform": {
  "position": {"x": 100, "y": 200},
  "rotation": 45,
  "scale": {"x": 1.5, "y": 1.5},
  "origin": {"x": 0, "y": 0}
}
```

**API:**

```cpp
// Position
void setPosition(float x, float y);
void getPosition(float& x, float& y) const;

// Rotation (in degrees)
void setRotation(float angle);
float getRotation() const;

// Scale
void setScale(float x, float y);
void getScale(float& x, float& y) const;

// Origin (pivot point)
void setOrigin(float x, float y);
void getOrigin(float& x, float& y) const;
```

**Example:**

```cpp
auto& transform = scene.componentReference<Harmony::Components::Transform>(entityId);
transform.setPosition(100.0f, 200.0f);
transform.setRotation(45.0f);
transform.setScale(2.0f, 2.0f);
```

#### Circle Component

Renders a circular shape with optional texture.

**Configuration:**

```json
"Circle": {
  "texture": 5000,
  "width": 50,
  "height": 50,
  "fill_color": {"r": 255, "g": 0, "b": 0, "a": 255},
  "outline_color": {"r": 0, "g": 0, "b": 0, "a": 255},
  "outline_thickness": 2
}
```

**Properties:**
- `texture`: Optional texture resource ID
- `width`: Circle width in pixels
- `height`: Circle height in pixels
- `fill_color`: RGBA color values (0-255)
- `outline_color`: RGBA color values (0-255)
- `outline_thickness`: Outline width in pixels

#### Rectangle Component

Renders a rectangular shape with optional texture.

**Configuration:**

```json
"Rectangle": {
  "texture": 5001,
  "width": 100,
  "height": 50,
  "fill_color": {"r": 0, "g": 255, "b": 0, "a": 255},
  "outline_color": {"r": 0, "g": 0, "b": 0, "a": 255},
  "outline_thickness": 2
}
```

**Properties:** Same as Circle component

#### Text Component

Renders text with a specified font.

**Configuration:**

```json
"Text": {
  "font": 6000,
  "string": "Hello, World!",
  "character_size": 24,
  "style": 0,
  "outline_thickness": 1,
  "fill_color": {"r": 255, "g": 255, "b": 255, "a": 255},
  "outline_color": {"r": 0, "g": 0, "b": 0, "a": 255}
}
```

**Properties:**
- `font`: Font resource ID (required)
- `string`: Text to display
- `character_size`: Font size in pixels
- `style`: Text style (0=Regular, 1=Bold, 2=Italic, 4=Underlined, 8=StrikeThrough)
- `outline_thickness`: Outline width in pixels
- `fill_color`: Text color RGBA (0-255)
- `outline_color`: Outline color RGBA (0-255)

**Style Values (can be combined):**
- `0`: Regular
- `1`: Bold
- `2`: Italic
- `4`: Underlined
- `8`: StrikeThrough
- Example: `3` = Bold + Italic

#### Script Component

Adds custom behavior to entities. Must be subclassed.

**Creating a Custom Script:**

```cpp
#include "Script.h"

class PlayerScript : public Harmony::Components::Script {
public:
    PlayerScript(const Harmony::Utilities::Configuration& config, 
                 Harmony::Scenes::Scene& scene)
        : Script(config, scene) {}

    void onCreate() override {
        // Called when entity is created
        HARMONY_INFO("Player created!");
    }

    void onPreUpdate() override {
        // Called before scene update
        // Handle input, update logic
    }

    void onPostUpdate() override {
        // Called after scene update
        // Post-processing logic
    }

    void onDestroy() override {
        // Called when entity is destroyed
        HARMONY_INFO("Player destroyed!");
    }
};

// Register the script
HARMONY_REGISTER_SCRIPT(PlayerScript, "PlayerScript");
```

**Configuration:**

```json
"entities": {
  "1000": {
    "name": "Player",
    "script": "PlayerScript",
    "components": { ... }
  }
}
```

**Script Lifecycle:**
1. `onCreate()`: Called when entity is created
2. `onPreUpdate()`: Called every frame before scene update
3. `onPostUpdate()`: Called every frame after scene update
4. `onDestroy()`: Called when entity is destroyed

**Accessing Components from Script:**

```cpp
void onPreUpdate() override {
    auto& scene = getScene();
    auto& transform = scene.componentReference<Harmony::Components::Transform>(entityId);
    
    // Update position
    float x, y;
    transform.getPosition(x, y);
    transform.setPosition(x + 1.0f, y);
}
```

### Tasks

#### Entity Management Tasks

**CreateEntityTask**
- **Purpose**: Create a new entity in a scene
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto config = engine.configuration.subsection({"entities", "1000"}).value();
  auto task = std::make_unique<Harmony::Tasks::CreateEntityTask>(sceneId, config);
  engine.taskManagement->submit(std::move(task));
  ```

**DestroyEntityTask**
- **Purpose**: Destroy an entity
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::DestroyEntityTask>(sceneId, entityId);
  engine.taskManagement->submit(std::move(task));
  ```

**BatchCreateEntitiesTask**
- **Purpose**: Create multiple entities efficiently
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  std::vector<Harmony::Utilities::Configuration> configs;
  // ... fill configs ...
  auto task = std::make_unique<Harmony::Tasks::BatchCreateEntitiesTask>(sceneId, configs);
  engine.taskManagement->submit(std::move(task));
  ```

**BatchDestroyEntitiesTask**
- **Purpose**: Destroy multiple entities
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  std::vector<Harmony::Scenes::EntityID> entityIds = {1, 2, 3, 4};
  auto task = std::make_unique<Harmony::Tasks::BatchDestroyEntitiesTask>(sceneId, entityIds);
  engine.taskManagement->submit(std::move(task));
  ```

#### Scene Management Tasks

**CreateSceneTask**
- **Purpose**: Create and add a scene to current state
- **Priority**: 100
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::CreateSceneTask>(sceneId);
  engine.taskManagement->submit(std::move(task));
  ```

**DeleteSceneTask**
- **Purpose**: Remove a scene from current state
- **Priority**: 100
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::DeleteSceneTask>(sceneId);
  engine.taskManagement->submit(std::move(task));
  ```

**ResetSceneTask**
- **Purpose**: Reset scene to initial configuration
- **Priority**: 75
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::ResetSceneTask>(sceneId);
  engine.taskManagement->submit(std::move(task));
  ```

**EnableSceneDrawingTask / DisableSceneDrawingTask**
- **Purpose**: Show/hide a scene
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::DisableSceneDrawingTask>(sceneId);
  engine.taskManagement->submit(std::move(task));
  ```

**EnableSceneUpdatingTask / DisableSceneUpdatingTask**
- **Purpose**: Enable/disable scene updates (pause/resume)
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(sceneId);
  engine.taskManagement->submit(std::move(task));
  ```

#### State Management Tasks

**PushState**
- **Purpose**: Push a new state onto the stack
- **Priority**: 100
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::PushState>(stateId);
  engine.taskManagement->submit(std::move(task));
  ```

**PopState**
- **Purpose**: Pop current state from stack
- **Priority**: 100
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::PopState>();
  engine.taskManagement->submit(std::move(task));
  ```

**SwitchStateTask**
- **Purpose**: Switch to a different state (pop + push)
- **Priority**: 100
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::SwitchStateTask>(newStateId);
  engine.taskManagement->submit(std::move(task));
  ```

**ClearStatesTask**
- **Purpose**: Clear all states from stack
- **Priority**: 150
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::ClearStatesTask>();
  engine.taskManagement->submit(std::move(task));
  ```

#### Resource Management Tasks

**LoadResourceTask**
- **Purpose**: Load a resource asynchronously
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::LoadResourceTask>("texture", 5000);
  engine.taskManagement->submit(std::move(task));
  ```

**UnloadResourceTask**
- **Purpose**: Unload a resource
- **Priority**: 75
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::UnloadResourceTask>(5000);
  engine.taskManagement->submit(std::move(task));
  ```

**BatchLoadResourcesTask**
- **Purpose**: Load multiple resources efficiently
- **Priority**: 100
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  std::vector<std::pair<std::string, Harmony::Utilities::UUID>> resources = {
      {"texture", 5000},
      {"sound", 7000},
      {"font", 6000}
  };
  auto task = std::make_unique<Harmony::Tasks::BatchLoadResourcesTask>(resources);
  engine.taskManagement->submit(std::move(task));
  ```

**UnloadAllResourcesTask**
- **Purpose**: Unload all loaded resources
- **Priority**: 150
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::UnloadAllResourcesTask>();
  engine.taskManagement->submit(std::move(task));
  ```

#### Component Management Tasks

**AddComponentTask**
- **Purpose**: Add a component to an existing entity
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  Harmony::Utilities::Configuration componentConfig;
  // ... configure component ...
  auto task = std::make_unique<Harmony::Tasks::AddComponentTask>(
      sceneId, entityId, "ComponentName", componentConfig);
  engine.taskManagement->submit(std::move(task));
  ```

#### Engine Control Tasks

**SetFPS**
- **Purpose**: Change target FPS
- **Priority**: 0 (immediate)
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::SetFPS>(144);
  engine.taskManagement->submit(std::move(task));
  ```

**StopEngineTask**
- **Purpose**: Stop the engine
- **Priority**: 200
- **Mode**: FastMultiThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::StopEngineTask>();
  engine.taskManagement->submit(std::move(task));
  ```

**DelayedActionTask**
- **Purpose**: Execute an action after a delay
- **Priority**: 0
- **Mode**: SlowMultiThreaded
- **Usage:**
  ```cpp
  auto action = []() { 
      HARMONY_INFO("This runs after delay");
  };
  auto task = std::make_unique<Harmony::Tasks::DelayedActionTask>(
      action, std::chrono::milliseconds(5000));
  engine.taskManagement->submit(std::move(task));
  ```

#### Utility Tasks

**CallbackTask**
- **Purpose**: Execute a callback with engine access
- **Parameters**: callback function, priority (optional), mode (optional)
- **Usage:**
  ```cpp
  auto callback = [](Harmony::Engine& engine) {
      HARMONY_INFO("Engine is running: {}", engine.isRunning());
  };
  auto task = std::make_unique<Harmony::Tasks::CallbackTask>(callback);
  engine.taskManagement->submit(std::move(task));
  ```

**LambdaTask**
- **Purpose**: Execute a simple lambda
- **Parameters**: lambda function, priority (optional), mode (optional)
- **Usage:**
  ```cpp
  auto lambda = []() {
      HARMONY_INFO("Simple action executed");
  };
  auto task = std::make_unique<Harmony::Tasks::LambdaTask>(lambda);
  engine.taskManagement->submit(std::move(task));
  ```

**ConditionalTask**
- **Purpose**: Execute actions based on a condition
- **Parameters**: condition function, onTrue action, onFalse action (optional), priority (optional)
- **Usage:**
  ```cpp
  auto condition = [](Harmony::Engine& engine) {
      return engine.getDeltaTime() > 0.033f;
  };
  auto onTrue = [](Harmony::Engine& engine) {
      HARMONY_WARN("Performance issue detected");
  };
  auto task = std::make_unique<Harmony::Tasks::ConditionalTask>(
      condition, onTrue, nullptr, 50);
  engine.taskManagement->submit(std::move(task));
  ```

**RepeatedTask**
- **Purpose**: Execute an action multiple times
- **Parameters**: action, repeat count, delay between executions (optional)
- **Usage:**
  ```cpp
  auto action = [](Harmony::Engine& engine) {
      HARMONY_INFO("Repeated action");
  };
  auto task = std::make_unique<Harmony::Tasks::RepeatedTask>(
      action, 10, std::chrono::milliseconds(1000));
  engine.taskManagement->submit(std::move(task));
  ```

**LogMessageTask**
- **Purpose**: Log a message at a specific level
- **Priority**: 0
- **Mode**: SingleThreaded
- **Usage:**
  ```cpp
  auto task = std::make_unique<Harmony::Tasks::LogMessageTask>(
      "Important message", Harmony::Tasks::LogMessageTask::Level::Info);
  engine.taskManagement->submit(std::move(task));
  ```

### Utilities

#### RandomValue

Generate random values within a range.

```cpp
#include "RandomValue.h"

// Random int between min and max
int value = Harmony::Utilities::random_value<int>(0, 100);

// Random int with only min (max = type max)
int value2 = Harmony::Utilities::random_value<int>(50);

// Random int with full range
int value3 = Harmony::Utilities::random_value<int>();
```

**Supported Types:** Any integral type (int, unsigned int, int64_t, uint64_t, etc.)

## Examples

### Example 1: Spawning Enemies in Waves

```cpp
void SpawnEnemyWave(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId, int enemyCount) {
    std::vector<Harmony::Utilities::Configuration> enemyConfigs;
    
    for (int i = 0; i < enemyCount; ++i) {
        auto config = engine.configuration.subsection({"entities", "enemy_template"}).value();
        
        // Randomize position
        config.set<float>({"components", "Transform", "position", "x"}, 
                         static_cast<float>(Harmony::Utilities::random_value<int>(0, 1200)));
        config.set<float>({"components", "Transform", "position", "y"}, 
                         static_cast<float>(Harmony::Utilities::random_value<int>(0, 700)));
        
        enemyConfigs.push_back(config);
    }
    
    auto task = std::make_unique<Harmony::Tasks::BatchCreateEntitiesTask>(sceneId, enemyConfigs);
    engine.taskManagement->submit(std::move(task));
}
```

### Example 2: Level Transition with Loading Screen

```cpp
void TransitionToLevel(Harmony::Engine& engine, 
                      Harmony::Utilities::UUID nextSceneId,
                      Harmony::Utilities::UUID loadingStateId) {
    // Show loading screen
    auto showLoadingTask = std::make_unique<Harmony::Tasks::PushState>(loadingStateId);
    engine.taskManagement->submit(std::move(showLoadingTask));
    
    // Unload current resources
    auto unloadTask = std::make_unique<Harmony::Tasks::UnloadAllResourcesTask>();
    engine.taskManagement->submit(std::move(unloadTask));
    
    // Load next scene
    auto loadSceneTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(nextSceneId);
    engine.taskManagement->submit(std::move(loadSceneTask));
    
    // Hide loading screen after delay
    auto hideLoadingTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine]() {
            auto task = std::make_unique<Harmony::Tasks::PopState>();
            engine.taskManagement->submit(std::move(task));
        },
        std::chrono::milliseconds(2000)
    );
    engine.taskManagement->submit(std::move(hideLoadingTask));
}
```

### Example 3: Pause Menu System

```cpp
void OpenPauseMenu(Harmony::Engine& engine, 
                   Harmony::Utilities::UUID gameSceneId,
                   Harmony::Utilities::UUID pauseStateId) {
    // Pause game updates
    auto pauseTask = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(pauseTask));
    
    // Show pause menu
    auto pushTask = std::make_unique<Harmony::Tasks::PushState>(pauseStateId);
    engine.taskManagement->submit(std::move(pushTask));
}

void ClosePauseMenu(Harmony::Engine& engine, Harmony::Utilities::UUID gameSceneId) {
    // Close pause menu
    auto popTask = std::make_unique<Harmony::Tasks::PopState>();
    engine.taskManagement->submit(std::move(popTask));
    
    // Resume game updates
    auto resumeTask = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(resumeTask));
}
```

### Example 4: Auto-Save System

```cpp
void SetupAutoSave(Harmony::Engine& engine) {
    auto saveAction = [](Harmony::Engine& engine) {
        // Save game logic here
        HARMONY_INFO("Game auto-saved");
        // Example: engine.configuration.save("savegame.json");
    };
    
    // Auto-save every 5 minutes (60 times with 5-second delay for demo)
    auto autoSaveTask = std::make_unique<Harmony::Tasks::RepeatedTask>(
        saveAction,
        60,  // Repeat 60 times
        std::chrono::milliseconds(5000)  // Every 5 seconds
    );
    
    engine.taskManagement->submit(std::move(autoSaveTask));
}
```

### Example 5: Performance Monitor

```cpp
void MonitorPerformance(Harmony::Engine& engine) {
    auto checkTask = std::make_unique<Harmony::Tasks::ConditionalTask>(
        [](Harmony::Engine& engine) {
            // Check if frame time is too high (FPS dropping)
            return engine.getDeltaTime() > 0.033f;  // > 30 FPS
        },
        [](Harmony::Engine& engine) {
            HARMONY_WARN("Performance issue: High frame time detected");
            // Could adjust quality settings here
        },
        nullptr,
        50  // Medium priority
    );
    
    engine.taskManagement->submit(std::move(checkTask));
}
```

### Example 6: Projectile with Timeout

```cpp
void FireProjectile(Harmony::Engine& engine, 
                   Harmony::Utilities::UUID sceneId,
                   float x, float y) {
    // Create projectile configuration
    auto config = engine.configuration.subsection({"entities", "projectile"}).value();
    config.set<float>({"components", "Transform", "position", "x"}, x);
    config.set<float>({"components", "Transform", "position", "y"}, y);
    
    // Create projectile
    auto createTask = std::make_unique<Harmony::Tasks::CreateEntityTask>(sceneId, config);
    engine.taskManagement->submit(std::move(createTask));
    
    // Schedule destruction after 5 seconds
    auto destroyTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine, sceneId]() {
            // In real implementation, you'd need to track the entity ID
            HARMONY_INFO("Projectile auto-destroyed after timeout");
        },
        std::chrono::milliseconds(5000)
    );
    
    engine.taskManagement->submit(std::move(destroyTask));
}
```

## Architecture

### Design Principles

1. **PImpl Idiom**: Hide implementation details from public headers to reduce compilation dependencies
2. **Thread Safety**: All managers use mutexes for concurrent access
3. **Task-Based**: Operations are submitted as tasks for flexible execution
4. **Configuration-Driven**: Game content defined in JSON, not hard-coded
5. **Component-Based**: Entities composed of reusable components

### Thread Model

- **Main Thread**: Handles rendering, window events, and SingleThreaded tasks
- **Worker Pool**: Executes FastMultiThreaded tasks (typically CPU-bound operations)
- **Detached Threads**: Execute SlowMultiThreaded tasks (typically I/O-bound operations)

### Memory Management

- Smart pointers (`std::unique_ptr`, `std::shared_ptr`) for automatic memory management
- RAII principles throughout the codebase
- No manual `new`/`delete` in user code

### Project Structure

```
Harmony/
├── Harmony/                    # Engine library source
│   ├── Engine.h/cpp            # Main engine class
│   ├── Configuration.h/cpp     # Configuration system
│   ├── Logger.h/cpp            # Logging system
│   ├── Task.h/cpp              # Base task class
│   ├── *Task.h/cpp             # Specific task implementations
│   ├── Scene.h/cpp             # Scene management
│   ├── State.h/cpp             # State management
│   ├── *Management.h/cpp       # Manager classes
│   ├── Transform.h/cpp         # Transform component
│   ├── Circle.h/cpp            # Circle component
│   ├── Rectangle.h/cpp         # Rectangle component
│   ├── Text.h/cpp              # Text component
│   ├── Script.h/cpp            # Script base class
│   ├── Resource.h/cpp          # Resource base class
│   ├── Texture.h/cpp           # Texture resource
│   ├── Font.h/cpp              # Font resource
│   ├── Sound.h/cpp             # Sound resource
│   ├── Music.h/cpp             # Music resource
│   └── ...                     # Other components and utilities
├── Sandbox/                    # Example project
│   ├── main.cpp                # Entry point
│   └── configuration.json      # Example configuration
├── Harmony.sln                 # Visual Studio solution
├── LICENSE.txt                 # MIT License
└── README.md                   # This file
```

## Additional Resources

### Documentation Files

- **TASK_SYSTEM.md**: Comprehensive task system documentation
- **TASK_QUICK_REFERENCE.md**: Quick reference for task usage
- **TASK_SYSTEM_README.md**: Task system overview
- **DRAW_ORDER_DOCUMENTATION.md**: Scene draw order documentation
- **DRAW_ORDER_QUICK_REFERENCE.md**: Draw order quick reference
- **FEATURE_SUMMARY.md**: Summary of implemented features
- **IMPLEMENTATION_SUMMARY.md**: Technical implementation details

### Example Files

- **TaskExamples.cpp**: Task usage examples
- **DrawOrderExamples.cpp**: Draw order examples
- **DrawOrderExample.json**: Example configuration with draw orders
- **Sandbox/configuration.json**: Working example configuration

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for details.

---

**Harmony Engine v1.0.0** - Built with ❤️ for game developers
