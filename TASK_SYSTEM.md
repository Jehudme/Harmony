# Harmony Task System Documentation

This document provides an overview of all available task classes in the Harmony engine for multi-threaded operations.

## Overview
The Harmony engine provides a comprehensive task system that allows operations to be executed on different threads with different priorities and execution modes. Tasks can run in three modes:
- **SingleThreaded**: Executes immediately on the main thread
- **FastMultiThreaded**: Executes on the worker pool threads (thread-safe operations)
- **SlowMultiThreaded**: Executes on a new detached thread (I/O or long-running operations)

## Entity Management Tasks

### CreateEntityTask
Creates a new entity in a specified scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**: 
  - `sceneId`: ID of the scene
  - `entityConfig`: Configuration for the entity

### DestroyEntityTask
Destroys an entity in a specified scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityId`: ID of the entity to destroy

### BatchCreateEntitiesTask
Creates multiple entities in a scene at once.
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityConfigs`: Vector of entity configurations

### BatchDestroyEntitiesTask
Destroys multiple entities in a scene at once.
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityIds`: Vector of entity IDs to destroy

## Scene Management Tasks

### CreateSceneTask
Creates a new scene from configuration.
- **Priority**: 100
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene to create

### ResetSceneTask
Resets a scene to its initial configuration by re-initializing all entities.
- **Priority**: 75
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene to reset

### EnableSceneDrawingTask
Enables drawing for a specific scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene

### DisableSceneDrawingTask
Disables drawing for a specific scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene

### EnableSceneUpdatingTask
Enables updating for a specific scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene

### DisableSceneUpdatingTask
Disables updating for a specific scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene

## Component Management Tasks

### AddComponentTask
Adds a component to an existing entity.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityId`: ID of the entity
  - `componentName`: Name of the component to add
  - `componentConfig`: Configuration for the component

### RemoveComponentTask
Removes a component from an entity.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityId`: ID of the entity
  - `componentName`: Name of the component to remove

### CloneEntityTask
Clones an entity with all its components (simplified implementation).
- **Priority**: 75
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `sourceEntityId`: ID of the entity to clone

### BatchAddComponentsTask
Adds the same component to multiple entities at once.
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityIds`: Vector of entity IDs
  - `componentName`: Name of the component to add
  - `componentConfig`: Configuration for the component

### BatchRemoveComponentsTask
Removes the same component from multiple entities at once.
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `entityIds`: Vector of entity IDs
  - `componentName`: Name of the component to remove

### CountEntitiesTask
Counts the number of active entities in a scene.
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `callback`: Function called with the entity count

### ClearSceneEntitiesTask
Removes all entities from a scene.
- **Priority**: 100
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene

### MoveEntityTask
Moves an entity from one scene to another (simplified implementation).
- **Priority**: 75
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sourceSceneId`: ID of the source scene
  - `targetSceneId`: ID of the target scene
  - `entityId`: ID of the entity to move

### BatchMoveEntitiesTask
Moves multiple entities from one scene to another (simplified implementation).
- **Priority**: 100
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `sourceSceneId`: ID of the source scene
  - `targetSceneId`: ID of the target scene
  - `entityIds`: Vector of entity IDs to move

## State Management Tasks

### PushState
Pushes a new state onto the state stack.
- **Priority**: 100
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `stateId`: ID of the state to push

### PopState
Pops the current state from the state stack.
- **Priority**: 100
- **Mode**: FastMultiThreaded

### ClearStatesTask
Clears all states from the state stack.
- **Priority**: 150
- **Mode**: FastMultiThreaded

### SwitchStateTask
Switches to a different state (pops current and pushes new).
- **Priority**: 100
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `newStateId`: ID of the state to switch to

## Resource Management Tasks

### LoadResourceTask
Loads a resource asynchronously.
- **Priority**: 75
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `resourceType`: Type of the resource
  - `resourceId`: ID of the resource

### UnloadResourceTask
Unloads a resource.
- **Priority**: 75
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `resourceId`: ID of the resource to unload

### UnloadAllResourcesTask
Unloads all loaded resources.
- **Priority**: 150
- **Mode**: SlowMultiThreaded

### BatchLoadResourcesTask
Loads multiple resources at once.
- **Priority**: 100
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `resources`: Vector of (resourceType, resourceId) pairs

## Engine Tasks

### SetFPS
Sets the target frames per second for the engine.
- **Priority**: 0 (immediate execution)
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `targetFPS`: Target FPS value

### StopEngineTask
Stops the engine.
- **Priority**: 200
- **Mode**: FastMultiThreaded

### DelayedActionTask
Executes an action after a specified delay.
- **Priority**: 0
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `action`: Function to execute
  - `delay`: Delay in milliseconds

## Utility Tasks

### CallbackTask
Executes a callback function with engine access.
- **Mode**: Configurable (default: SingleThreaded)
- **Priority**: Configurable (default: 0)
- **Parameters**:
  - `callback`: Function that takes Engine& as parameter
  - `priority`: Optional priority
  - `mode`: Optional execution mode

### LambdaTask
Executes a simple lambda function.
- **Mode**: Configurable (default: SingleThreaded)
- **Priority**: Configurable (default: 0)
- **Parameters**:
  - `lambda`: Function to execute
  - `priority`: Optional priority
  - `mode`: Optional execution mode

### LogMessageTask
Logs a message at a specific level.
- **Priority**: 0
- **Mode**: SingleThreaded
- **Parameters**:
  - `message`: Message to log
  - `level`: Log level (Trace, Debug, Info, Warning, Error)

### ConditionalTask
Executes different actions based on a condition.
- **Priority**: Configurable (default: 0)
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `condition`: Function that returns bool
  - `onTrue`: Action to execute if condition is true
  - `onFalse`: Optional action to execute if condition is false
  - `priority`: Optional priority

### RepeatedTask
Executes an action multiple times with optional delays.
- **Priority**: 0
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `action`: Function to execute repeatedly
  - `repeatCount`: Number of times to repeat
  - `delayBetween`: Optional delay between executions

### SequentialTasksTask
Executes multiple actions in sequence.
- **Priority**: Configurable (default: 50)
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `actions`: Vector of functions to execute in order
  - `priority`: Optional priority

### TimedActionTask
Measures and logs execution time of an action.
- **Priority**: Configurable (default: 50)
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `action`: Function to execute and measure
  - `actionName`: Name for logging purposes
  - `priority`: Optional priority

### RetryTask
Retries an action on failure with configurable retries.
- **Priority**: Configurable (default: 50)
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `action`: Function that returns bool (success/failure)
  - `maxRetries`: Maximum number of retry attempts
  - `delayBetween`: Delay between retry attempts
  - `priority`: Optional priority

### ThrottledTask
Executes at most once per time period (throttling).
- **Priority**: Configurable (default: 50)
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `action`: Function to execute
  - `minDelay`: Minimum delay between executions
  - `priority`: Optional priority

### DebouncedTask
Executes only after a period of inactivity (debouncing).
- **Priority**: Configurable (default: 50)
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `action`: Function to execute
  - `delay`: Delay to wait before executing
  - `priority`: Optional priority

### MemorySnapshotTask
Logs current memory usage information.
- **Priority**: 0
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `callback`: Optional callback to receive snapshot string

### DumpSceneInfoTask
Logs detailed scene information for debugging.
- **Priority**: 0
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene to dump

### ValidateSceneTask
Checks scene integrity and reports results.
- **Priority**: 0
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene to validate
  - `callback`: Optional callback with (isValid, message)

### ChainedTasksTask
Executes multiple tasks with error handling, aborting on first failure.
- **Priority**: Configurable (default: 50)
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `tasks`: Vector of functions returning bool
  - `onError`: Optional error handler with failed task index
  - `priority`: Optional priority

### ScheduledTask
Schedules an action to execute at a specific time.
- **Priority**: Configurable (default: 50)
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `action`: Function to execute
  - `executeAt`: Time point when to execute
  - `priority`: Optional priority

### IntervalTask
Executes an action at regular intervals (timer functionality).
- **Priority**: 0
- **Mode**: SlowMultiThreaded
- **Parameters**:
  - `action`: Function returning bool (continue/stop)
  - `interval`: Delay between executions
  - `maxExecutions`: Maximum executions (-1 for infinite)

## Scene Management Tasks (Additional)

### SetSceneDrawOrderTask
Attempts to modify scene draw order (note: draw order is currently immutable).
- **Priority**: 50
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene
  - `drawOrder`: New draw order value

### CheckSceneExistsTask
Checks if a scene exists and reports result via callback.
- **Priority**: 0
- **Mode**: FastMultiThreaded
- **Parameters**:
  - `sceneId`: ID of the scene to check
  - `callback`: Function called with bool result

## Usage Examples

### Creating an Entity
```cpp
auto config = engine.configuration.subsection({"entity_template"}).value();
auto task = std::make_unique<Harmony::Tasks::CreateEntityTask>(sceneId, config);
engine.taskManagement->submit(std::move(task));
```

### Loading Multiple Resources
```cpp
std::vector<std::pair<std::string, Harmony::Utilities::UUID>> resources = {
    {"texture", 1001},
    {"sound", 2001},
    {"font", 3001}
};
auto task = std::make_unique<Harmony::Tasks::BatchLoadResourcesTask>(resources);
engine.taskManagement->submit(std::move(task));
```

### Delayed Action
```cpp
auto action = []() { 
    std::cout << "This runs after 5 seconds" << std::endl; 
};
auto task = std::make_unique<Harmony::Tasks::DelayedActionTask>(
    action, 
    std::chrono::milliseconds(5000)
);
engine.taskManagement->submit(std::move(task));
```

### Conditional Execution
```cpp
auto condition = [](Engine& engine) { 
    return engine.isRunning(); 
};
auto onTrue = [](Engine& engine) { 
    std::cout << "Engine is running" << std::endl; 
};
auto task = std::make_unique<Harmony::Tasks::ConditionalTask>(
    condition, 
    onTrue
);
engine.taskManagement->submit(std::move(task));
```

### Batch Component Addition
```cpp
std::vector<EntityID> entities = {entity1, entity2, entity3};
auto task = std::make_unique<Harmony::Tasks::BatchAddComponentsTask>(
    sceneId,
    entities,
    "Transform",
    componentConfig
);
engine.taskManagement->submit(std::move(task));
```

### Retry with Error Handling
```cpp
auto riskyOperation = [](Engine& engine) -> bool {
    // Returns true on success, false on failure
    return performNetworkRequest();
};
auto task = std::make_unique<Harmony::Tasks::RetryTask>(
    riskyOperation,
    3,  // Max 3 retries
    std::chrono::milliseconds(1000)  // 1 second between retries
);
engine.taskManagement->submit(std::move(task));
```

### Timed Performance Measurement
```cpp
auto heavyOperation = [](Engine& engine) {
    // Perform expensive computation
};
auto task = std::make_unique<Harmony::Tasks::TimedActionTask>(
    heavyOperation,
    "Physics Calculation"  // Name for logging
);
engine.taskManagement->submit(std::move(task));
```

### Scene Information Debugging
```cpp
auto task = std::make_unique<Harmony::Tasks::DumpSceneInfoTask>(sceneId);
engine.taskManagement->submit(std::move(task));
// Logs: entity count, drawing/updating status, draw order, etc.
```

### Throttled Input Handling
```cpp
auto handleInput = [](Engine& engine) {
    // Process input - will only execute once per 100ms
};
auto task = std::make_unique<Harmony::Tasks::ThrottledTask>(
    handleInput,
    std::chrono::milliseconds(100)  // Minimum 100ms between executions
);
engine.taskManagement->submit(std::move(task));
```

## Best Practices

1. **Choose the Right Mode**:
   - Use `SingleThreaded` for operations that must run on the main thread (e.g., rendering)
   - Use `FastMultiThreaded` for quick, thread-safe operations
   - Use `SlowMultiThreaded` for I/O operations or long-running tasks

2. **Set Appropriate Priorities**:
   - Priority 0: Immediate execution
   - Priority 1-99: High priority tasks
   - Priority 100-199: Normal priority tasks
   - Priority 200+: Low priority tasks

3. **Handle Errors**:
   - All task implementations include error handling
   - Check logs for task execution failures

4. **Batch Operations**:
   - Use batch tasks (BatchCreateEntitiesTask, BatchLoadResourcesTask) for better performance when processing multiple items

5. **Resource Management**:
   - Always unload resources when no longer needed to prevent memory leaks
   - Use UnloadAllResourcesTask when transitioning between major game states
