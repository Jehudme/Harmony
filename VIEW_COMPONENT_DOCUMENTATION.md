# View Component Documentation

## Overview

The View component in Harmony Engine provides camera/viewport functionality for rendering scenes. Each scene has a singleton View component that controls how the scene is rendered to the screen.

## Key Features

- **Singleton per scene**: Each scene has exactly one View component
- **Automatic initialization**: Views are automatically created with default values matching window size
- **Configurable**: Views can be configured via JSON configuration
- **Dynamic manipulation**: Views can be modified at runtime through the Scene API

## How It Works

The View component is implemented as a singleton per scene, stored directly in the Scene's implementation (not as an entity component). When a scene is rendered:

1. The view is set on the render target before drawing any entities
2. All entities in the scene are drawn with the view's transformation applied
3. Each scene can have its own independent view

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

```cpp
auto& view = scene.getView();

// Change center
view.setCenter(640.0f, 360.0f);

// Change size
view.setSize(1280.0f, 720.0f);

// Rotate the view
view.setRotation(45.0f);

// Zoom in (makes everything appear larger)
view.zoom(0.5f);  // 50% of current size = 2x zoom

// Zoom out (makes everything appear smaller)
view.zoom(2.0f);  // 200% of current size = 0.5x zoom

// Move the view
view.move(100.0f, 50.0f);  // Move 100 units right, 50 units down

// Set viewport (split screen, picture-in-picture, etc.)
view.setViewport(0.0f, 0.0f, 0.5f, 1.0f);  // Left half of screen

// Reset view
view.reset(640.0f, 360.0f, 1280.0f, 720.0f);  // Reset to default
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
- Stored in `Scene::SceneImpl` as `std::unique_ptr<Components::View>`
- Not tied to any entity
- One instance per scene

### Registration
- Registered with `HARMONY_REGISTER_COMPONENT(Harmony::Components::View, View)`
- Can be created through component system but primarily used as singleton

### Thread Safety
- Access through Scene methods is thread-safe (uses scene's entity mutex)
- View modifications during rendering are protected

### PImpl Pattern
- Implementation details hidden behind `ViewImpl` struct
- Encapsulates `sf::View` from SFML
- Allows changing implementation without affecting API

## Best Practices

1. **Initialize views in configuration** when possible for consistency
2. **Use getView() for runtime modifications** rather than recreating views
3. **Consider viewport coordinates** when designing split-screen or minimap features
4. **Zoom gradually** for smooth camera effects (use small zoom factors)
5. **Center view on important game elements** for better player experience

## Common Pitfalls

1. **Forgetting normalized viewport coordinates**: Viewport uses 0-1 range, not pixel coordinates
2. **Zooming too much**: Very small zoom factors can cause rendering issues
3. **Not resetting view**: Remember to reset view when switching scenes if needed
4. **Modifying view during entity iteration**: Always modify views between frames, not during entity updates

## See Also

- Transform Component: For entity-specific transformations
- Scene Management: For managing multiple scenes with different views
- DrawOrder Documentation: For controlling rendering order with multiple views
