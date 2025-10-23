# View Component Documentation

## Overview

The View component in Harmony Engine provides camera/viewport functionality for rendering scenes. Each scene has a singleton View component that controls how the scene is rendered to the screen.

## Key Features

- **Singleton per scene**: Each scene has exactly one View component stored in EnTT registry context (ctx)
- **Inherits from sf::View**: View now directly inherits from SFML's sf::View instead of composing it
- **Automatic initialization**: Views are automatically created with default values matching window size
- **Configurable**: Views can be configured via JSON configuration
- **Dynamic manipulation**: Views can be modified at runtime through the Scene API
- **Global component**: Stored in registry ctx, not tied to any entity

## How It Works

The View component is implemented as a global component stored in the EnTT registry's context (ctx). When a scene is rendered:

1. The view is retrieved from the registry context
2. The view is set on the render target before drawing any entities
3. All entities in the scene are drawn with the view's transformation applied
4. Each scene can have its own independent view stored in its own registry context

## Configuration

### Default View

If no view configuration is provided, a default view is created with:
- Center: (window_width/2, window_height/2)
- Size: (window_width, window_height)
- Rotation: 0
- Viewport: full screen (0, 0, 1, 1)

### Custom View Configuration

Add a `"view"` section to your scene configuration:

```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [2001, 2002],
      "view": {
        "center": {
          "x": 640,
          "y": 360
        },
        "size": {
          "width": 1280,
          "height": 720
        },
        "rotation": 0,
        "viewport": {
          "left": 0.0,
          "top": 0.0,
          "width": 1.0,
          "height": 1.0
        }
      }
    }
  }
}
```

### Configuration Options

#### Center (Required with warnings if missing)
- `center.x`: X coordinate of the view center
- `center.y`: Y coordinate of the view center

#### Size (Required with warnings if missing)
- `size.width`: Width of the view in world coordinates
- `size.height`: Height of the view in world coordinates

#### Rotation (Optional)
- `rotation`: Rotation angle in degrees

#### Viewport (Optional)
- `viewport.left`: Left position (0-1, normalized to window)
- `viewport.top`: Top position (0-1, normalized to window)
- `viewport.width`: Width (0-1, normalized to window)
- `viewport.height`: Height (0-1, normalized to window)

## Runtime API

### Getting the View

```cpp
// Get mutable reference to the view
auto& view = scene.getView();

// Get const reference to the view
const auto& view = scene.getView();
```

### Setting a New View

```cpp
Harmony::Utilities::Configuration viewConfig;
viewConfig.set<float>({"center", "x"}, 640.0f);
viewConfig.set<float>({"center", "y"}, 360.0f);
viewConfig.set<float>({"size", "width"}, 1280.0f);
viewConfig.set<float>({"size", "height"}, 720.0f);

scene.setView(viewConfig);
```

### Modifying the View

Since View now inherits from sf::View, you can use both the inherited methods and the helper methods:

```cpp
auto& view = scene.getView();

// Using inherited sf::View methods directly
view.setCenter(640.0f, 360.0f);
view.setSize(1280.0f, 720.0f);
view.setRotation(45.0f);

// Zoom in (makes everything appear larger)
view.zoom(0.5f);  // 50% of current size = 2x zoom

// Zoom out (makes everything appear smaller)
view.zoom(2.0f);  // 200% of current size = 0.5x zoom

// Move the view
view.move(100.0f, 50.0f);  // Move 100 units right, 50 units down

// Set viewport (split screen, picture-in-picture, etc.)
view.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f));  // Left half of screen

// Reset view
view.reset(sf::FloatRect(0.0f, 0.0f, 1280.0f, 720.0f));

// Using helper methods for array-based input
float center[2] = {640.0f, 360.0f};
view.setCenter(center);

float size[2] = {1280.0f, 720.0f};
view.setSize(size);

// Getting values
float x, y;
view.getCenter(x, y);

float width, height;
view.getSize(width, height);
```

### Using Tasks for View Management

```cpp
// Create a new view for a scene using a task
Harmony::Utilities::Configuration viewConfig;
viewConfig.set<float>({"center", "x"}, 640.0f);
viewConfig.set<float>({"center", "y"}, 360.0f);
viewConfig.set<float>({"size", "width"}, 1280.0f);
viewConfig.set<float>({"size", "height"}, 720.0f);

auto createViewTask = std::make_unique<Harmony::Tasks::CreateSceneViewTask>(sceneId, viewConfig);
engine.taskManagement->addTask(std::move(createViewTask));

// Delete a view from a scene using a task
auto deleteViewTask = std::make_unique<Harmony::Tasks::DeleteSceneViewTask>(sceneId);
engine.taskManagement->addTask(std::move(deleteViewTask));
```

### Managing Global Components

The View component demonstrates the use of global components in the registry context. You can create your own global components using the same pattern:

```cpp
// Create a global component
auto& myGlobal = scene.createGlobalComponent<MyGlobalType>(constructorArgs...);

// Access a global component
if (auto* myGlobal = scene.getGlobalComponent<MyGlobalType>()) {
    // Use the global component
}

// Delete a global component
scene.deleteGlobalComponent<MyGlobalType>();
```

## Use Cases

### 1. Camera Following Player

```cpp
void PlayerScript::onPostUpdate() override {
    // Get player position
    auto& transform = scene_.componentReference<Transform>(entityId);
    float x, y;
    transform.getPosition(x, y);
    
    // Move view to follow player
    auto& view = scene_.getView();
    view.setCenter(x, y);
}
```

### 2. Zoom Effect

```cpp
void ZoomInEffect(Harmony::Scenes::Scene& scene) {
    auto& view = scene.getView();
    view.zoom(0.9f);  // Zoom in by 10%
}

void ZoomOutEffect(Harmony::Scenes::Scene& scene) {
    auto& view = scene.getView();
    view.zoom(1.1f);  // Zoom out by 10%
}
```

### 3. Split Screen

```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [2001],
      "view": {
        "center": {"x": 320, "y": 360},
        "size": {"width": 640, "height": 720},
        "viewport": {
          "left": 0.0,
          "top": 0.0,
          "width": 0.5,
          "height": 1.0
        }
      }
    },
    "1002": {
      "drawOrder": 0,
      "entities": [2002],
      "view": {
        "center": {"x": 960, "y": 360},
        "size": {"width": 640, "height": 720},
        "viewport": {
          "left": 0.5,
          "top": 0.0,
          "width": 0.5,
          "height": 1.0
        }
      }
    }
  }
}
```

### 4. Minimap

```json
{
  "scenes": {
    "1001": {
      "drawOrder": 0,
      "entities": [2001],
      "view": {
        "center": {"x": 640, "y": 360},
        "size": {"width": 1280, "height": 720}
      }
    },
    "1002": {
      "drawOrder": 100,
      "entities": [2002],
      "view": {
        "center": {"x": 640, "y": 360},
        "size": {"width": 2560, "height": 1440},
        "viewport": {
          "left": 0.75,
          "top": 0.0,
          "width": 0.25,
          "height": 0.25
        }
      }
    }
  }
}
```

## Implementation Details

### Storage
- Stored in EnTT registry context (`entt::registry::ctx()`) as a global component
- Not tied to any entity
- One instance per scene, accessible via `scene.getView()`

### Inheritance
- **View now inherits from sf::View**: Direct inheritance instead of composition
- No PImpl pattern needed - sf::View is now the base class
- All sf::View methods are directly accessible
- Additional helper methods provided for convenience (array-based setters/getters)

### Registration
- Registered with `HARMONY_REGISTER_COMPONENT(Harmony::Components::View, View)`
- Can be created through component system but primarily used as global component in ctx

### Thread Safety
- Access through Scene methods is thread-safe (uses scene's entity mutex)
- View modifications during rendering are protected

### Global Component Pattern
- Uses EnTT's context (ctx) for global component storage
- Allows singleton-like behavior without entity attachment
- Can be created/deleted using scene methods or tasks
- Pattern can be reused for other global components

### Tasks
- **CreateSceneViewTask**: Creates a View component in a scene's registry context
- **DeleteSceneViewTask**: Deletes a View component from a scene's registry context

## Best Practices

1. **Initialize views in configuration** when possible for consistency
2. **Use getView() for runtime modifications** rather than recreating views
3. **Consider viewport coordinates** when designing split-screen or minimap features
4. **Zoom gradually** for smooth camera effects (use small zoom factors)
5. **Center view on important game elements** for better player experience
6. **Use tasks for async view management** when creating/deleting views from other threads
7. **Leverage sf::View methods directly** - since View inherits from sf::View, you can use all SFML view functionality

## Common Pitfalls

1. **Forgetting normalized viewport coordinates**: Viewport uses 0-1 range, not pixel coordinates
2. **Zooming too much**: Very small zoom factors can cause rendering issues
3. **Not resetting view**: Remember to reset view when switching scenes if needed
4. **Modifying view during entity iteration**: Always modify views between frames, not during entity updates
5. **Using old getInternalView()**: This method has been removed - View is now sf::View directly

## See Also

- Transform Component: For entity-specific transformations
- Scene Management: For managing multiple scenes with different views
- DrawOrder Documentation: For controlling rendering order with multiple views
