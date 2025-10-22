# Scene Draw Order Feature Documentation

## Overview
The Harmony Engine now supports scene draw ordering, allowing you to control the rendering order of multiple scenes within a state. This enables layered rendering for backgrounds, game content, UI overlays, and more.

## Key Features

### 1. Draw Order Configuration
Each scene can now have a `drawOrder` property in its configuration. Scenes are rendered in ascending order (lower values first).

```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [...]
    },
    "1002": {
      "drawOrder": 10,
      "entities": [...]
    }
  }
}
```

### 2. Automatic Sorting
When a state renders its scenes, they are automatically sorted by their `drawOrder` value:
- Lower values are drawn first (background)
- Higher values are drawn last (foreground/overlay)
- Default value is 0 if not specified

### 3. Dynamic Scene Management
New tasks allow you to dynamically add and remove scenes from the current state:

#### CreateSceneTask
Creates a scene and automatically adds it to the current state.

```cpp
auto task = std::make_unique<Harmony::Tasks::CreateSceneTask>(sceneId);
engine.taskManagement->submit(std::move(task));
```

#### DeleteSceneTask
Removes a scene from the current state.

```cpp
auto task = std::make_unique<Harmony::Tasks::DeleteSceneTask>(sceneId);
engine.taskManagement->submit(std::move(task));
```

## API Changes

### Scene Class
- **New property**: `const int drawOrder` - The rendering order of the scene
- **Read from config**: `drawOrder` is read from scene configuration (defaults to 0)

### State Class
- **New method**: `void addScene(std::shared_ptr<Scene> scene)` - Add a scene to the state
- **New method**: `void removeScene(Utilities::UUID sceneId)` - Remove a scene from the state
- **Updated**: `internalDraw()` now sorts scenes by drawOrder before rendering

### StateManager Class
- **New method**: `std::shared_ptr<State> getCurrentState() const` - Get the current active state

### SceneTask
- **Updated**: `CreateSceneTask` now adds the created scene to the current state
- **New task**: `DeleteSceneTask` - Removes a scene from the current state

## Usage Examples

### Basic Layered Rendering
```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [2001]  // Background
    },
    "1002": {
      "drawOrder": 10,
      "entities": [2002]  // Game content
    },
    "1003": {
      "drawOrder": 20,
      "entities": [2003]  // UI overlay
    }
  }
}
```

### Dynamic Scene Creation
```cpp
// Add a new layer dynamically during gameplay
auto createTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(newSceneId);
engine.taskManagement->submit(std::move(createTask));

// Remove it later
auto deleteTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(newSceneId);
engine.taskManagement->submit(std::move(deleteTask));
```

### Pause Menu Overlay
```cpp
void ShowPauseMenu(Harmony::Engine& engine) {
    // Pause game updates
    auto pauseTask = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(pauseTask));
    
    // Show pause menu (with high drawOrder to appear on top)
    auto showMenuTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(pauseMenuSceneId);
    engine.taskManagement->submit(std::move(showMenuTask));
}

void HidePauseMenu(Harmony::Engine& engine) {
    // Remove pause menu
    auto hideMenuTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(pauseMenuSceneId);
    engine.taskManagement->submit(std::move(hideMenuTask));
    
    // Resume game
    auto resumeTask = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(resumeTask));
}
```

## Common Draw Order Conventions

Here are recommended drawOrder values for different layer types:

| Layer Type          | Draw Order Range | Description                           |
|---------------------|------------------|---------------------------------------|
| Parallax Background | -20 to -10       | Far background elements               |
| Background          | -10 to 0         | Background scenery                    |
| Game World          | 0 to 10          | Main game content                     |
| Effects/Particles   | 10 to 20         | Visual effects                        |
| UI Elements         | 20 to 50         | HUD, buttons, menus                   |
| Overlays            | 50 to 100        | Modal dialogs, pause menus            |
| Debug               | 100+             | Debug information, always on top      |

## Performance Considerations

- Scene sorting happens during each draw call, but the overhead is minimal for typical scene counts (< 20 scenes)
- Consider disabling drawing for hidden scenes using `DisableSceneDrawingTask` to save performance
- Use draw order strategically to minimize overdraw (opaque backgrounds at low orders)

## Migration Guide

### For Existing Scenes
If you have existing scene configurations without `drawOrder`:
1. The default value of 0 will be used
2. Scenes will render in the same order as before (by UUID order in the map)
3. Add `drawOrder` to your configurations to explicitly control rendering order

### Example Migration
Before:
```json
{
  "scenes": {
    "1001": {
      "entities": [2001]
    }
  }
}
```

After:
```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [2001]
    }
  }
}
```

## See Also
- `DrawOrderExamples.cpp` - Comprehensive examples of using draw order
- `DrawOrderExample.json` - Example configuration with multiple layers
- `TaskExamples.cpp` - General task system examples
