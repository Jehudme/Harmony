# ResourceHandler System

## Overview

The ResourceHandler system provides a complete, robust, and thread-safe resource management solution for the Harmony engine. It handles all 2D, 3D, and audio Raylib resources with automatic memory management, lazy loading, and LRU-based eviction.

## Features

- **ID-based Resource Access**: Simple unsigned int ResourceID for resource identification
- **Automatic Loading/Unloading**: Resources load on-demand and unload when idle
- **Memory Cap Enforcement**: Configurable memory limit with automatic LRU eviction
- **Thread-Safe Operations**: All operations use shared_mutex for safe concurrent access
- **Configuration-Driven**: Flexible setup through Configuration system
- **Batch Operations**: Efficient batch load/unload for scene transitions
- **Always-Loaded Resources**: Mark critical resources to prevent unloading
- **Comprehensive Logging**: Detailed logging for debugging and monitoring
- **Type Safety**: Polymorphic base class with type-specific implementations

## Architecture

### Core Classes

#### Resource_t (Base Class)
```cpp
class Resource_t {
public:
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual bool isLoaded() const = 0;
    virtual const char* getTypeName() const = 0;
    
    // Time tracking
    void updateLastUsedTime();
    bool shouldUnload() const;
    
    // Metadata
    ResourceID getId() const;
    std::size_t getFileSize() const;
    float getUnloadDelay() const;
    bool isAlwaysLoaded() const;
};
```

#### ResourceHandler
```cpp
class ResourceHandler {
public:
    // Resource lifecycle
    void load(ResourceID id);
    void unload(ResourceID id);
    std::shared_ptr<Resource_t> get(ResourceID id);
    
    // Batch operations
    void batchLoad(const std::vector<ResourceID>& ids);
    void batchUnload(const std::vector<ResourceID>& ids);
    void loadAll();
    void unloadAll();
    
    // Management
    void registerResource(std::shared_ptr<Resource_t> resource);
    void updateAutoUnload();
    void updateMusicStreaming();
    
    // Memory management
    std::size_t getCurrentMemoryUsage() const;
    void setMemoryCap(std::size_t cap);
};
```

## Supported Resource Types

### 2D & Image Resources
- **TextureResource**: 2D textures loaded via LoadTexture()
- **ImageResource**: Raw image data via LoadImage()
- **FontResource**: Font files with custom size via LoadFontEx()

### Audio Resources
- **SoundResource**: Sound effects via LoadSound()
- **MusicResource**: Music streams via LoadMusicStream() with automatic streaming updates

### 3D Resources
- **ModelResource**: 3D models via LoadModel()
- **MaterialResource**: Materials via LoadMaterialDefault()
- **ShaderResource**: Vertex and fragment shaders via LoadShader()
- **MeshResource**: Mesh data extracted from models
- **TextureCubemapResource**: Cubemap textures from 6 face images

## Usage

### Basic Setup

```cpp
#include <Harmony/ResourceHandler.h>
#include <Harmony/TextureResources.h>
#include <Harmony/AudioResources.h>
#include <Harmony/ModelResources.h>

// Create configuration
Configuration config;
config.set<std::size_t>({ "resources", "memoryCap" }, 512 * 1024 * 1024);  // 512MB
config.set<float>({ "resources", "defaultUnloadDelay" }, 60.0f);  // 60 seconds
config.set<bool>({ "resources", "enableLogging" }, true);

// Create ResourceHandler
auto resourceHandler = std::make_unique<ResourceHandler>(config);
```

### Registering Resources

```cpp
// Register a texture resource
auto texture = std::make_shared<TextureResource>(
    1,                                  // ResourceID
    "assets/textures/player.png",      // File path
    30.0f,                              // Unload delay (seconds)
    false                               // Always loaded?
);
resourceHandler->registerResource(texture);

// Register a sound resource
auto sound = std::make_shared<SoundResource>(
    10,
    "assets/sounds/jump.wav"
);
resourceHandler->registerResource(sound);

// Register an always-loaded font
auto font = std::make_shared<FontResource>(
    100,
    "assets/fonts/ui.ttf",
    32,    // Font size
    60.0f,
    true   // Always loaded
);
resourceHandler->registerResource(font);
```

### Using Resources

```cpp
// Get a resource (automatically loads if not loaded)
auto textureResource = std::dynamic_pointer_cast<TextureResource>(
    resourceHandler->get(1)
);

// Use the resource
if (textureResource) {
    const Texture2D& texture = textureResource->getTexture();
    DrawTexture(texture, x, y, WHITE);
}
```

### Batch Operations

```cpp
// Load all resources for a scene
std::vector<ResourceID> sceneResources = { 1, 2, 3, 10, 11, 50 };
resourceHandler->batchLoad(sceneResources);

// Unload when leaving scene
resourceHandler->batchUnload(sceneResources);
```

### Automatic Management

```cpp
// In your main loop
void update(float deltaTime) {
    // Auto-unload idle resources
    resourceHandler->updateAutoUnload();
    
    // Update music streaming
    resourceHandler->updateMusicStreaming();
}
```

### Memory Monitoring

```cpp
// Check memory usage
std::size_t currentUsage = resourceHandler->getCurrentMemoryUsage();
std::size_t cap = resourceHandler->getMemoryCap();
float percentage = (float)currentUsage / cap * 100.0f;

Logger::info("Memory: {} / {} bytes ({}%)", currentUsage, cap, percentage);

// Adjust memory cap dynamically
resourceHandler->setMemoryCap(256 * 1024 * 1024);  // 256MB
```

## Configuration Options

### Memory Cap
```cpp
config.set<std::size_t>({ "resources", "memoryCap" }, 1024 * 1024 * 1024);  // 1GB
```

### Default Unload Delay
```cpp
config.set<float>({ "resources", "defaultUnloadDelay" }, 30.0f);  // 30 seconds
```

### Preload List
```cpp
config.set<ResourceID>({ "resources", "preload", "texture1" }, 1);
config.set<ResourceID>({ "resources", "preload", "sound1" }, 10);
```

### Always-Loaded List
```cpp
config.set<ResourceID>({ "resources", "alwaysLoaded", "uiFont" }, 100);
config.set<ResourceID>({ "resources", "alwaysLoaded", "defaultShader" }, 200);
```

### Enable Logging
```cpp
config.set<bool>({ "resources", "enableLogging" }, true);
```

## Best Practices

### 1. Use Weak Pointers in Game Objects
```cpp
class Player {
    std::weak_ptr<Resource_t> textureResource_;
    
    void render() {
        if (auto res = textureResource_.lock()) {
            auto tex = std::dynamic_pointer_cast<TextureResource>(res);
            DrawTexture(tex->getTexture(), x, y, WHITE);
        }
    }
};
```

### 2. Mark Core Resources as Always-Loaded
```cpp
// UI fonts, default shaders, fallback textures
auto defaultFont = std::make_shared<FontResource>(
    ID_DEFAULT_FONT,
    "assets/fonts/default.ttf",
    16, 60.0f, true  // Always loaded
);
```

### 3. Use Batch Operations
```cpp
// Load scene resources together
void loadLevel(int levelId) {
    auto resources = getLevelResources(levelId);
    resourceHandler->batchLoad(resources);
}
```

### 4. Update Every Frame
```cpp
void gameLoop() {
    while (!WindowShouldClose()) {
        resourceHandler->updateAutoUnload();
        resourceHandler->updateMusicStreaming();
        
        // ... game logic and rendering
    }
}
```

### 5. Monitor Memory Usage
```cpp
void checkMemory() {
    std::size_t usage = resourceHandler->getCurrentMemoryUsage();
    std::size_t cap = resourceHandler->getMemoryCap();
    
    if (usage > cap * 0.9f) {
        Logger::warn("Memory usage high: {} / {}", usage, cap);
    }
}
```

## Exception Handling

All resource operations can throw exceptions:

```cpp
try {
    resourceHandler->load(textureId);
}
catch (const Exceptions::ResourceNotFoundException& e) {
    Logger::error("Resource not found: {}", e.what());
}
catch (const Exceptions::TextureLoadException& e) {
    Logger::error("Failed to load texture: {}", e.what());
}
catch (const Exceptions::ResourceMemoryCapExceededException& e) {
    Logger::error("Memory cap exceeded: {}", e.what());
}
```

## Thread Safety

All public methods of ResourceHandler are thread-safe:

```cpp
// Safe to call from multiple threads
std::thread t1([&]() { resourceHandler->load(1); });
std::thread t2([&]() { resourceHandler->load(2); });
std::thread t3([&]() { resourceHandler->get(3); });

t1.join();
t2.join();
t3.join();
```

## Memory Management Strategy

### Automatic Unloading
1. Resources track last used time
2. `updateAutoUnload()` checks for idle resources
3. Resources exceeding `unloadDelay` are unloaded
4. Always-loaded resources are never unloaded

### LRU Eviction
1. When memory cap is exceeded:
2. Find least recently used non-always-loaded resource
3. Unload it to free memory
4. Repeat until under cap or no more candidates

### Memory Calculation
- Textures: width × height × 4 bytes (RGBA)
- Images: width × height × 4 bytes
- Fonts: texture atlas size
- Sounds: frameCount × sampleSize × channels
- Music: ~1MB buffer estimate (streaming)
- Models: sum of mesh vertex and index data
- Materials: ~1KB estimate
- Shaders: ~10KB estimate

## Sandbox Demo

The Sandbox project includes a comprehensive demo:

```bash
# Build and run Sandbox
# The demo will:
# - Create ResourceHandler with configuration
# - Register various resource types
# - Demonstrate batch operations
# - Show memory tracking
# - Test auto-unload
# - Validate memory cap enforcement
```

## File Structure

```
Harmony/
├── Resource.h              # Base Resource_t class
├── Resource.cpp
├── ResourceHandler.h       # Main ResourceHandler class
├── ResourceHandler.cpp
├── TextureResources.h      # 2D & Image resources
├── TextureResources.cpp
├── AudioResources.h        # Audio resources
├── AudioResources.cpp
├── ModelResources.h        # 3D resources
├── ModelResources.cpp
└── Exceptions.h/cpp        # Resource exceptions

Sandbox/
└── main.cpp                # Comprehensive demo
```

## Performance Characteristics

- **O(1)** resource access via unordered_map
- **O(n)** LRU eviction (where n = loaded resource count)
- **O(n)** auto-unload check
- Thread-safe with minimal lock contention
- Lazy loading minimizes startup time
- Memory-efficient with automatic cleanup

## Future Enhancements

Potential future improvements:
- Asset streaming from disk
- Resource compression
- Async loading with progress callbacks
- Resource dependency tracking
- Hot-reloading support
- Resource atlasing
- Reference counting optimization
