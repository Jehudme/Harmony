# Architecture Recommendations for Full Abstraction

This document outlines the recommended approach for completing the abstraction of external dependencies (SFML, Entt) from the Harmony engine.

## Current Status

### ✅ Completed
- Engine class uses PImpl to hide SFML window, clock, and time
- Scene class uses PImpl to hide entt::registry
- State and StateManagement hide sf::Drawable
- Transform uses composition instead of inheritance
- EntityID type alias introduced
- Forward declarations used extensively
- Header includes minimized

### ⚠️ Partial
- Shape classes (Rectangle, Circle) still inherit from SFML types
- Scene.inl exposes entt types in templates
- ComponentManagement still uses entt::entity directly

### ❌ Not Started
- Directory reorganization (include/ and src/ structure)
- Abstraction interfaces (IWindow, IRenderer, IDrawable)
- Backend isolation
- Build system configuration for private dependencies

## Recommended Next Steps

### Phase 1: Core Abstractions (High Priority)

#### 1.1 Create Engine Abstractions

Create abstract interfaces for core engine functionality:

```cpp
// include/Harmony/core/IWindow.h
namespace Harmony::Core {
    class IWindow {
    public:
        virtual ~IWindow() = default;
        virtual bool isOpen() const = 0;
        virtual void close() = 0;
        virtual void display() = 0;
        virtual void clear() = 0;
        // etc.
    };
}

// include/Harmony/core/IRenderTarget.h
namespace Harmony::Core {
    class IDrawable;
    
    class IRenderTarget {
    public:
        virtual ~IRenderTarget() = default;
        virtual void draw(const IDrawable& drawable) = 0;
    };
}

// include/Harmony/core/IDrawable.h
namespace Harmony::Core {
    class IDrawable {
    public:
        virtual ~IDrawable() = default;
        virtual void draw(IRenderTarget& target) const = 0;
    };
}
```

#### 1.2 Implement SFML Backend

```cpp
// src/backend/sfml/SFMLWindow.h
#include "Harmony/core/IWindow.h"
#include <SFML/Graphics.hpp>

namespace Harmony::Backend::SFML {
    class SFMLWindow : public Core::IWindow {
        sf::RenderWindow window_;
        // Implementation
    };
}
```

### Phase 2: Component System Refactoring (Medium Priority)

#### 2.1 Create Component Interfaces

```cpp
// include/Harmony/components/IComponent.h
namespace Harmony::Components {
    class IComponent {
    public:
        virtual ~IComponent() = default;
    };
    
    class IRenderable : public IComponent {
    public:
        virtual void render(Core::IRenderTarget& target) = 0;
    };
    
    class ITransformable : public IComponent {
    public:
        virtual void setPosition(float x, float y) = 0;
        virtual void getPosition(float& x, float& y) const = 0;
        // etc.
    };
}
```

#### 2.2 Refactor Shape Classes

Replace inheritance with composition:

```cpp
// include/Harmony/components/Rectangle.h
namespace Harmony::Components {
    class Rectangle : public IRenderable, public ITransformable {
    public:
        Rectangle(const Utilities::Configuration& config);
        
        // IRenderable
        void render(Core::IRenderTarget& target) override;
        
        // ITransformable
        void setPosition(float x, float y) override;
        // etc.
        
        // Rectangle-specific
        void setSize(float width, float height);
        
    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
```

### Phase 3: ECS Abstraction (Lower Priority)

#### 3.1 Hide Entt Behind Facade

```cpp
// include/Harmony/ecs/Registry.h
namespace Harmony::ECS {
    class Entity {
        uint32_t id_;
    public:
        Entity(uint32_t id) : id_(id) {}
        uint32_t id() const { return id_; }
    };
    
    class Registry {
    public:
        Entity createEntity();
        void destroyEntity(Entity entity);
        
        template<typename Component>
        Component& addComponent(Entity entity, std::unique_ptr<Component> component);
        
        template<typename Component>
        Component& getComponent(Entity entity);
        
    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
```

### Phase 4: Directory Restructuring

Proposed structure:

```
Harmony/
├── include/
│   └── Harmony/
│       ├── Harmony.h          # Main include
│       ├── core/
│       │   ├── Engine.h
│       │   ├── IWindow.h
│       │   ├── IRenderTarget.h
│       │   └── IDrawable.h
│       ├── ecs/
│       │   ├── Entity.h
│       │   └── Registry.h
│       ├── components/
│       │   ├── IComponent.h
│       │   ├── Transform.h
│       │   ├── Rectangle.h
│       │   └── Circle.h
│       ├── scenes/
│       │   ├── Scene.h
│       │   └── State.h
│       └── utilities/
│           ├── Configuration.h
│           └── Logger.h
├── src/
│   ├── core/
│   │   └── Engine.cpp
│   ├── ecs/
│   │   └── Registry.cpp
│   ├── components/
│   │   ├── Transform.cpp
│   │   ├── Rectangle.cpp
│   │   └── Circle.cpp
│   ├── scenes/
│   │   ├── Scene.cpp
│   │   └── State.cpp
│   └── backend/
│       └── sfml/
│           ├── SFMLWindow.h
│           ├── SFMLWindow.cpp
│           ├── SFMLRenderTarget.h
│           ├── SFMLRenderTarget.cpp
│           └── SFMLDrawable.h
└── CMakeLists.txt (or vcxproj)
```

### Phase 5: Build System Configuration

#### 5.1 CMake Example

```cmake
# Public interface library
add_library(Harmony_Interface INTERFACE)
target_include_directories(Harmony_Interface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Private backend library
add_library(Harmony_Backend_SFML STATIC
    src/backend/sfml/SFMLWindow.cpp
    # etc.
)
target_link_libraries(Harmony_Backend_SFML PRIVATE sfml-graphics)

# Main library
add_library(Harmony STATIC
    src/core/Engine.cpp
    # etc.
)
target_link_libraries(Harmony
    PUBLIC Harmony_Interface
    PRIVATE Harmony_Backend_SFML
    PRIVATE EnTT::EnTT
)
```

## Implementation Strategy

1. **Incremental**: Don't break existing code - add new interfaces alongside old implementation
2. **Test-Driven**: Add tests for each abstraction before refactoring
3. **Deprecation Warnings**: Mark old APIs as deprecated before removing
4. **Documentation**: Update documentation with each phase

## Benefits of Full Implementation

1. **Backend Swappability**: Can replace SFML with SDL, Vulkan, etc.
2. **Testing**: Can mock rendering for unit tests
3. **Compilation Speed**: Dramatically faster with proper header hygiene
4. **API Stability**: Public API independent of backend changes
5. **Licensing Flexibility**: Easier to change backend libraries if needed

## Timeline Estimate

- Phase 1 (Core Abstractions): 2-3 weeks
- Phase 2 (Component Refactoring): 2-3 weeks
- Phase 3 (ECS Abstraction): 1-2 weeks
- Phase 4 (Directory Restructure): 1 week
- Phase 5 (Build System): 1 week

Total: ~2-3 months for complete refactoring

## Risk Mitigation

1. **Keep Old Code**: Maintain backward compatibility during transition
2. **Feature Branches**: Use separate branches for each phase
3. **Continuous Testing**: Test on Windows after each change
4. **Documentation**: Document every API change

## Conclusion

The work done so far provides a solid foundation. The remaining work is more mechanical - creating interfaces, implementing backends, and reorganizing files. The biggest challenge will be maintaining backward compatibility during the transition.
