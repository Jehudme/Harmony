# Physics Demo - Box2D Integration

This demo showcases the physics integration with Box2D in the Harmony engine.

## How to Run

1. Build the Sandbox project in Visual Studio
2. Copy `physics_configuration.json` to the output directory and rename it to `configuration.json`
3. Run the Sandbox executable

## Demo Features

### Physics World
- Gravity is set to (0, 981) pixels/second² - simulating Earth-like gravity
- Physics simulation runs at 60 FPS with 6 velocity iterations and 2 position iterations

### Entities

#### Interactive Box (Blue)
- **Position**: Center top of screen (512, 200)
- **Control**: Use arrow keys to apply forces
  - ↑: Apply upward force
  - ↓: Apply downward force
  - ←: Apply leftward force
  - →: Apply rightward force
- Dynamic physics body with damping

#### Ground Platform (Gray)
- **Position**: Bottom of screen (512, 700)
- Static physics body - doesn't move
- Acts as a floor for falling objects

#### Falling Boxes (Orange, Green, Yellow)
- Three boxes starting at different positions and rotations
- Dynamic physics bodies that respond to gravity
- Will fall and collide with the ground and each other

## Physics Integration Details

The physics integration is implemented in `Scene.cpp`:

1. **Before Physics Step**: Transform component data (position, rotation) is copied to PhysicsBody
2. **Physics Simulation**: `PhysicsWorld::step()` is called to advance the simulation
3. **After Physics Step**: PhysicsBody data is copied back to Transform component

This ensures that:
- Visual transform and physics simulation stay synchronized
- Entities with both Transform and PhysicsBody components are automatically handled
- The design is extensible for additional physics components (fixtures, colliders, joints)

## Configuration Structure

The physics demo uses the following configuration structure:

```json
{
  "scenes": {
    "201": {
      "globalComponents": {
        "PhysicsWorld": {
          "gravity": {
            "x": 0,
            "y": 981
          }
        }
      }
    }
  },
  "entities": {
    "301": {
      "components": {
        "Transform": { ... },
        "Rectangle": { ... },
        "PhysicsBody": {
          "type": "dynamic",
          "position": { "x": 512, "y": 200 }
        }
      }
    }
  }
}
```

## Original Demo

The original Asteroid demo (using `configuration.json`) is still available and demonstrates manual physics-like movement without Box2D integration.
