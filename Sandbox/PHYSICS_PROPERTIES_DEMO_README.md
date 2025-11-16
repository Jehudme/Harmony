# Physics Properties Demo - Friction, Restitution, and Density

This demo showcases the enhanced physics system with support for fixture properties like friction, restitution (bounciness), density, and sensor flags.

## New Physics Parameters

### FixtureProperties Structure

The physics system now supports a `FixtureProperties` struct that allows you to configure:

- **density** (default: 1.0): Mass per unit area in kg/m². Higher values make objects heavier.
- **friction** (default: 0.2): Friction coefficient from 0.0 (frictionless/ice) to 1.0 (high friction).
- **restitution** (default: 0.0): Bounciness from 0.0 (no bounce) to 1.0 (perfect elastic bounce).
- **isSensor** (default: false): When true, the fixture detects collisions but doesn't physically interact.

## Demo Entities

### 1. Bouncy Ball (Red Circle - Left)
- **Color**: Red
- **Density**: 1.0
- **Friction**: 0.3
- **Restitution**: 0.9 (very bouncy!)
- **Behavior**: Bounces very high when hitting the ground

### 2. Sticky Ball (Blue Circle - Center-Left)
- **Color**: Blue
- **Density**: 1.0
- **Friction**: 1.0 (maximum)
- **Restitution**: 0.0 (no bounce)
- **Behavior**: Sticks to surfaces, doesn't bounce, resists sliding

### 3. Frictionless Box (Green Box - Center-Right)
- **Color**: Green
- **Density**: 1.0
- **Friction**: 0.0 (frictionless - like ice)
- **Restitution**: 0.5
- **Behavior**: Slides very easily on slopes and surfaces

### 4. Heavy Box (Brown Box - Right)
- **Color**: Brown
- **Density**: 10.0 (10x heavier!)
- **Friction**: 0.3
- **Restitution**: 0.2
- **Behavior**: Much heavier, pushes lighter objects aside

### 5. Ground Platform (Gray - Bottom)
- **Color**: Gray
- **Density**: N/A (static)
- **Friction**: 0.5 (medium)
- **Restitution**: 0.0
- **Type**: Static platform

### 6. Sloped Ground (Dark Gray - Bottom-Left)
- **Color**: Dark Gray
- **Density**: N/A (static)
- **Friction**: 0.2 (low - slippery slope)
- **Restitution**: 0.0
- **Type**: Static angled platform (20 degrees)

## API Usage Examples

### Using FixtureProperties

```cpp
#include <Harmony/PhysicsBody.h>

// Create properties with custom values
Harmony::Components::FixtureProperties props;
props.density = 2.0f;
props.friction = 0.8f;
props.restitution = 0.6f;
props.isSensor = false;

// Create a box fixture with these properties
physicsBody->createBoxFixture(50.0f, 50.0f, props);
```

### Legacy API (Still Supported)

```cpp
// Old API with density only still works
physicsBody->createBoxFixture(50.0f, 50.0f, 1.0f);
```

### Different Fixture Types

```cpp
// Circle with custom properties
FixtureProperties circleProps;
circleProps.restitution = 0.9f;  // Bouncy ball
physicsBody->createCircleFixture(25.0f, circleProps);

// Polygon with custom properties
std::vector<b2Vec2> trianglePoints = {
    b2Vec2(0.0f, -25.0f),
    b2Vec2(-25.0f, 25.0f),
    b2Vec2(25.0f, 25.0f)
};
FixtureProperties polyProps;
polyProps.friction = 0.5f;
physicsBody->createPolygonFixture(trianglePoints, polyProps);
```

## Physics Principles Demonstrated

### Friction
- **High friction** (1.0): Objects resist sliding, like rubber on concrete
- **Medium friction** (0.3-0.5): Normal surfaces
- **Low friction** (0.1-0.2): Slippery surfaces
- **No friction** (0.0): Ice-like surfaces

### Restitution (Bounciness)
- **0.0**: No bounce (clay, mud)
- **0.3-0.5**: Moderate bounce (wood, plastic)
- **0.7-0.9**: High bounce (rubber ball, basketball)
- **1.0**: Perfect elastic collision (theoretical)

### Density
- Affects mass: `mass = density × area`
- Higher density = heavier objects
- Heavy objects have more momentum
- Default density of 1.0 is a good baseline

## How to Run

1. Build the Sandbox project
2. Copy `physics_properties_demo_configuration.json` to the output directory
3. Rename it to `configuration.json`
4. Run the Sandbox executable
5. Watch the different physics behaviors!

## Expected Behavior

1. **Bouncy ball** (red) will bounce very high
2. **Sticky ball** (blue) will barely bounce and resist rolling
3. **Frictionless box** (green) will slide easily down the slope
4. **Heavy box** (brown) will fall and settle without bouncing much
5. Objects on the **sloped ground** will slide down based on their friction

## Technical Details

All fixture creation methods now have two overloads:
- **New API**: Accepts `FixtureProperties` struct with all parameters
- **Legacy API**: Accepts only `density` (uses default values for other properties)

The implementation ensures backward compatibility while providing enhanced control over physics behavior.
