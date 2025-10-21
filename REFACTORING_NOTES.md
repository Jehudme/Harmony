# Harmony Engine - Header Refactoring

## Overview

This document describes the architectural improvements made to reduce external library dependencies in public headers and improve header hygiene.

## Key Improvements

### 1. PImpl Pattern Implementation

Several core classes now use the PImpl (Pointer to Implementation) idiom to hide implementation details:

#### Engine Class
- **Before**: Exposed `sf::RenderWindow`, `sf::Clock`, and `sf::Time` in the header
- **After**: Hidden behind `struct EngineImpl` in the .cpp file
- **API Change**: `getDeltaTime()` now returns `float` (seconds) instead of `sf::Time`

```cpp
// Old
sf::Time getDeltaTime() const noexcept;

// New
float getDeltaTime() const noexcept;
```

#### Scene Class
- **Before**: Public `entt::registry` member, inherited from `sf::Drawable`
- **After**: Hidden `entt::registry` in `struct SceneImpl`, removed `sf::Drawable` inheritance
- **API Change**: 
  - `update()` takes `float` instead of `sf::Time`
  - Uses `internalDraw(void*)` instead of `draw(sf::RenderTarget&, sf::RenderStates)`
  - Entity type changed from `entt::entity` to `EntityID` (type alias for `unsigned int`)

```cpp
// Old
void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
void update(const sf::Time deltaTime);
entt::entity createEntity(...);

// New
void internalDraw(void* renderTarget) const;
void update(float deltaTime);
EntityID createEntity(...);
```

#### Transform Component
- **Before**: Inherited from `sf::Transformable`
- **After**: Uses composition with hidden `sf::Transformable` in PImpl
- **API**: Now provides clean getter/setter methods without exposing SFML types

```cpp
// New API
void setPosition(float x, float y);
void getPosition(float& x, float& y) const;
void setRotation(float angle);
float getRotation() const;
// etc.
```

### 2. Forward Declarations

Headers now extensively use forward declarations instead of includes:

- `Engine.h`: Forward declares all management classes and Configuration
- `Scene.h`: Forward declares Configuration, Engine, and entt types
- `State.h`: Forward declares Configuration, Engine, and Scene

### 3. Header Includes Cleanup

Removed unnecessary includes from headers:
- `Scene.h`: Moved Logger.h and Configuration.h includes to .cpp file
- Added proper forward declarations for commonly used types

### 4. Type Aliases

Introduced type aliases to allow future implementation changes:

```cpp
namespace Harmony::Scenes {
    using EntityID = unsigned int;  // Can change ECS implementation later
}
```

## Current Limitations

Some components still expose SFML types due to architectural constraints:

1. **Shape Classes** (Rectangle, Circle):
   - Still inherit from SFML shape types
   - Documented as known limitation
   - Future refactoring needed to use composition + interfaces

2. **ComponentManagement**:
   - Still uses `entt::entity` in template parameters
   - Template nature makes abstraction challenging

3. **Scene.inl**:
   - Template implementations still require entt headers
   - Users including Scene.h indirectly get entt types

## Benefits

1. **Reduced Compilation Dependencies**: Headers include fewer external libraries
2. **Faster Compilation**: Less code to parse in headers
3. **Better Encapsulation**: Implementation details hidden from users
4. **API Stability**: Internal changes don't require recompiling client code
5. **Future-Proofing**: Easier to swap out SFML or Entt in the future

## Migration Guide

If you're using the engine, here are the API changes:

### Delta Time
```cpp
// Before
sf::Time dt = engine.getDeltaTime();
float seconds = dt.asSeconds();

// After
float seconds = engine.getDeltaTime();
```

### Entity IDs
```cpp
// Before
entt::entity entity = scene.createEntity(config);

// After
Harmony::Scenes::EntityID entity = scene.createEntity(config);
```

### Scene Updates
```cpp
// Before (if calling directly)
scene.update(deltaTime);  // deltaTime was sf::Time

// After
scene.update(deltaTimeSeconds);  // deltaTimeSeconds is float
```

## Future Work

1. **Complete Shape Refactoring**: 
   - Create `IShape` and `IDrawable` interfaces
   - Refactor Rectangle/Circle to use composition
   
2. **Hide Remaining Entt Types**:
   - Create internal EntityHandle wrapper
   - Move more template code to implementation headers

3. **Directory Restructure**:
   - Move public headers to `include/Harmony/`
   - Move backend code to `src/backend/sfml/`
   
4. **Build System Updates**:
   - Separate public/private include paths
   - Create backend target for SFML dependencies

## Testing

Since the project uses Visual Studio and requires Windows to build, comprehensive testing on Windows is recommended after pulling these changes.

## Credits

Refactored to follow modern C++ practices and reduce header dependencies for better compilation times and encapsulation.
