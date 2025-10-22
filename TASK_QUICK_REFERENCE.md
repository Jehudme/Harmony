# Task Quick Reference Guide

## Task Categories and Use Cases

### When to Use Entity Tasks
Use entity tasks when you need to create or destroy game objects in a thread-safe manner:
- Spawning enemies during gameplay
- Creating projectiles in response to player actions
- Cleaning up destroyed objects
- Loading multiple entities from level data

### When to Use Scene Tasks
Use scene tasks for loading and managing different game scenes:
- Loading new levels in the background
- Transitioning between menu and gameplay scenes
- Preloading upcoming scenes while current scene is active
- Pausing/resuming scenes by disabling/enabling updates
- Hiding/showing scenes by disabling/enabling drawing
- Resetting scenes to their initial state (e.g., retry level)

### When to Use Component Tasks
Use component tasks for modifying entity components at runtime:
- Adding abilities or power-ups to entities
- Attaching scripts to entities dynamically
- Batch processing entity modifications

### When to Use State Tasks
Use state tasks for managing game states:
- Menu navigation (main menu → settings → gameplay)
- Pausing and resuming gameplay
- Switching between different game modes
- Clearing state stack on game exit

### When to Use Resource Tasks
Use resource tasks for asset loading and management:
- Loading textures, sounds, and fonts asynchronously
- Preloading assets for upcoming levels
- Unloading unused assets to free memory
- Background loading to avoid frame drops

### When to Use Engine Tasks
Use engine tasks for engine-level operations:
- Adjusting performance settings during runtime
- Gracefully shutting down the engine
- Scheduling timed events
- Implementing cooldowns or delayed reactions

### When to Use Utility Tasks
Use utility tasks for custom behaviors:
- Implementing game-specific logic with callbacks
- Creating custom event systems
- Conditional behavior based on game state
- Repeating actions (e.g., periodic saves, heartbeat systems)

## Common Patterns

### Pattern: Asynchronous Level Loading
```cpp
// Load scene in background
auto loadSceneTask = std::make_unique<CreateSceneTask>(nextLevelId);
engine.taskManagement->submit(std::move(loadSceneTask));

// After loading, switch to it
auto switchTask = std::make_unique<SwitchStateTask>(nextLevelStateId);
engine.taskManagement->submit(std::move(switchTask));
```

### Pattern: Batch Entity Creation
```cpp
// Create multiple entities efficiently
std::vector<Utilities::Configuration> enemyConfigs;
for (int i = 0; i < 10; i++) {
    enemyConfigs.push_back(enemyTemplateConfig);
}
auto batchTask = std::make_unique<BatchCreateEntitiesTask>(sceneId, enemyConfigs);
engine.taskManagement->submit(std::move(batchTask));
```

### Pattern: Delayed Cleanup
```cpp
// Destroy entity after 3 seconds (e.g., projectile lifespan)
auto destroyTask = std::make_unique<DelayedActionTask>(
    [sceneId, entityId]() {
        // Destruction logic
    },
    std::chrono::milliseconds(3000)
);
engine.taskManagement->submit(std::move(destroyTask));
```

### Pattern: Conditional Logic
```cpp
// Execute logic based on game state
auto checkTask = std::make_unique<ConditionalTask>(
    [](Engine& engine) { return engine.getTargetFPS() < 30; },
    [](Engine& engine) { engine.setTargetFPS(60); },  // Restore FPS
    nullptr,  // No action if FPS is fine
    50  // Medium priority
);
engine.taskManagement->submit(std::move(checkTask));
```

### Pattern: Periodic Auto-Save
```cpp
// Auto-save every 5 minutes
auto autoSaveTask = std::make_unique<RepeatedTask>(
    [](Engine& engine) {
        // Save game logic
        HARMONY_INFO("Auto-saved game");
    },
    60,  // 60 iterations
    std::chrono::milliseconds(5000)  // 5 second delay (for demo)
);
engine.taskManagement->submit(std::move(autoSaveTask));
```

### Pattern: Resource Preloading
```cpp
// Preload assets for next level
std::vector<std::pair<std::string, UUID>> nextLevelAssets = {
    {"texture", textureId1},
    {"texture", textureId2},
    {"sound", soundId1},
    {"music", musicId1}
};
auto preloadTask = std::make_unique<BatchLoadResourcesTask>(nextLevelAssets);
engine.taskManagement->submit(std::move(preloadTask));
```

### Pattern: Pause/Resume Game
```cpp
// Pause game (stop updates but keep rendering)
auto pauseTask = std::make_unique<DisableSceneUpdatingTask>(gameSceneId);
engine.taskManagement->submit(std::move(pauseTask));

// Resume game
auto resumeTask = std::make_unique<EnableSceneUpdatingTask>(gameSceneId);
engine.taskManagement->submit(std::move(resumeTask));
```

### Pattern: Reset Level (Retry)
```cpp
// Reset scene to initial configuration
auto resetTask = std::make_unique<ResetSceneTask>(levelSceneId);
engine.taskManagement->submit(std::move(resetTask));
```

### Pattern: Hide/Show UI or Background Layers
```cpp
// Hide background scene to save performance
auto hideTask = std::make_unique<DisableSceneDrawingTask>(backgroundSceneId);
engine.taskManagement->submit(std::move(hideTask));

// Show background scene again
auto showTask = std::make_unique<EnableSceneDrawingTask>(backgroundSceneId);
engine.taskManagement->submit(std::move(showTask));
```

## Thread Safety Tips

1. **SingleThreaded tasks** are safe for operations that:
   - Modify rendering state
   - Access UI elements
   - Need to be synchronized with the main game loop

2. **FastMultiThreaded tasks** are safe for:
   - Quick computations
   - Thread-safe data structure operations
   - Using the engine's thread-safe managers

3. **SlowMultiThreaded tasks** are best for:
   - File I/O operations
   - Network requests
   - Long-running computations
   - Resource loading

## Performance Guidelines

### Priority Levels
- **0**: Immediate - use sparingly, executes before queued tasks
- **1-49**: Critical - time-sensitive operations
- **50-99**: High - important game logic
- **100-149**: Normal - standard operations
- **150-199**: Low - background tasks
- **200+**: Very Low - cleanup and maintenance

### Batch vs Individual Tasks
- Use batch tasks when processing 10+ similar items
- Individual tasks have lower overhead for 1-5 items
- Batch tasks provide better logging and error reporting

### Delay Usage
- Delays are more efficient than polling
- Use DelayedActionTask for timed events
- Consider RepeatedTask for periodic actions

## Error Handling

All tasks include built-in error handling with logging. Check logs with:
```
HARMONY_ERROR - Critical errors that prevent task execution
HARMONY_WARN - Warnings about unexpected but handled conditions
HARMONY_INFO - Successful task completions
HARMONY_DEBUG - Detailed task execution information
```

## Task Lifecycle

1. **Creation**: Task is instantiated with parameters
2. **Submission**: Task is submitted to TaskManager
3. **Queueing**: Task is placed in priority queue or executed immediately
4. **Execution**: Task's run() method is called
5. **Completion**: Task is destroyed after execution

Tasks are **single-use** - create a new task instance for each execution.
