# Scene Draw Order Quick Reference

## Configuration

### Add drawOrder to Scene Config
```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [...]
    }
  }
}
```

## Common Draw Order Values

| Order | Purpose                |
|-------|------------------------|
| -20   | Far background         |
| -10   | Background             |
| 0     | Game world (default)   |
| 10    | Effects/particles      |
| 20    | UI elements            |
| 50    | Overlays/menus         |
| 100   | Debug info             |

## Tasks

### Create Scene (adds to current state)
```cpp
auto task = std::make_unique<Harmony::Tasks::CreateSceneTask>(sceneId);
engine.taskManagement->submit(std::move(task));
```

### Delete Scene (removes from current state)
```cpp
auto task = std::make_unique<Harmony::Tasks::DeleteSceneTask>(sceneId);
engine.taskManagement->submit(std::move(task));
```

### Control Scene Drawing
```cpp
// Hide scene
auto task = std::make_unique<Harmony::Tasks::DisableSceneDrawingTask>(sceneId);
engine.taskManagement->submit(std::move(task));

// Show scene
auto task = std::make_unique<Harmony::Tasks::EnableSceneDrawingTask>(sceneId);
engine.taskManagement->submit(std::move(task));
```

## Common Patterns

### Pause Menu
```cpp
// Show
auto pauseGame = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
auto showMenu = std::make_unique<Harmony::Tasks::CreateSceneTask>(menuSceneId);
engine.taskManagement->submit(std::move(pauseGame));
engine.taskManagement->submit(std::move(showMenu));

// Hide
auto hideMenu = std::make_unique<Harmony::Tasks::DeleteSceneTask>(menuSceneId);
auto resumeGame = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
engine.taskManagement->submit(std::move(hideMenu));
engine.taskManagement->submit(std::move(resumeGame));
```

### Layered Setup
```json
{
  "states": {
    "1": {
      "scenes": [100, 200, 300]
    }
  },
  "scenes": {
    "100": { "drawOrder": -10 },  // Background
    "200": { "drawOrder": 0 },    // Game
    "300": { "drawOrder": 20 }    // UI
  }
}
```

### Dynamic Layer Addition
```cpp
// Add weather effect overlay
auto task = std::make_unique<Harmony::Tasks::CreateSceneTask>(weatherSceneId);
engine.taskManagement->submit(std::move(task));

// Remove when done
auto removeTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(weatherSceneId);
engine.taskManagement->submit(std::move(removeTask));
```

## See Also
- `DRAW_ORDER_DOCUMENTATION.md` - Full documentation
- `DrawOrderExamples.cpp` - Detailed examples
- `DrawOrderExample.json` - Sample configuration
