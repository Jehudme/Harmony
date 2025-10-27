# New Components and Tasks Documentation

This document describes the 8 new components and 27 related tasks added to the Harmony engine.

## Overview

The new components focus on improving engine functionality, performance, and safety by providing:
- Enhanced rendering capabilities (Sprite, Camera, ParticleEmitter)
- Audio management (Audio)
- Physics simulation (PhysicsBody, Collision)
- Game logic utilities (Timer, StateMachine)

All components follow the existing Harmony patterns:
- PImpl idiom for hiding implementation details
- Thread-safe operations
- Configuration-driven initialization
- Comprehensive error handling and logging
- RAII memory management

---

## Components

### 1. Sprite Component

**Purpose**: Efficient texture rendering with sprite sheet animation support.

**Key Features**:
- Texture management with resource ID
- Sprite sheet support with texture rectangles
- Frame-based animation system with play/pause/stop controls
- Color tinting and alpha blending
- Horizontal and vertical flipping
- Smooth animation updates with delta time

**Configuration Example**:
```json
{
  "texture": 5000,
  "texture_rect": {
    "left": 0,
    "top": 0,
    "width": 32,
    "height": 32
  },
  "color": {
    "r": 255,
    "g": 255,
    "b": 255,
    "a": 255
  },
  "flip_horizontal": false,
  "flip_vertical": false
}
```

**API Methods**:
- `setTexture(UUID)`, `getTextureId()`
- `setTextureRect(left, top, width, height)`
- `setColor(r, g, b, a)`
- `setAnimation(frames, frameTime)`
- `playAnimation(loop)`, `stopAnimation()`, `pauseAnimation()`, `resumeAnimation()`
- `setFlipHorizontal(bool)`, `setFlipVertical(bool)`
- `updateAnimation(deltaTime)` - Must be called each frame for animations

**Use Cases**:
- Character sprite rendering with walk/run animations
- Animated UI elements
- Sprite sheet-based particle effects
- 2D game objects with multiple animation states

---

### 2. Camera Component

**Purpose**: View control and viewport management for scene rendering.

**Key Features**:
- Center position control
- Size and zoom management
- Rotation support
- Viewport control (render to portion of window)
- Active/inactive state
- Reset to default view

**Configuration Example**:
```json
{
  "center": { "x": 400, "y": 300 },
  "size": { "width": 800, "height": 600 },
  "rotation": 0,
  "viewport": {
    "left": 0.0,
    "top": 0.0,
    "width": 1.0,
    "height": 1.0
  },
  "zoom": 1.0,
  "active": false
}
```

**API Methods**:
- `setCenter(x, y)`, `getCenter(x, y)`
- `move(offsetX, offsetY)`
- `setSize(width, height)`, `getSize(width, height)`
- `setRotation(angle)`, `rotate(angle)`
- `zoom(factor)`, `setZoom(level)`, `getZoom()`
- `setViewport(left, top, width, height)`
- `reset()` - Reset to default configuration
- `setActive(bool)`, `isActive()`

**Use Cases**:
- Follow camera for player characters
- Split-screen multiplayer (multiple cameras with viewports)
- Cinematic camera movements
- Mini-maps and picture-in-picture effects
- Zoom in/out for strategy games

---

### 3. Audio Component

**Purpose**: Sound and music playback attached to entities for spatial audio.

**Key Features**:
- Separate sound and music playback
- Volume and pitch control for both
- Positional 3D audio support
- Auto-play on creation
- Loop control for music
- Min distance and attenuation for 3D sound

**Configuration Example**:
```json
{
  "sound": 7000,
  "music": 8000,
  "auto_play_sound": false,
  "auto_play_music": true,
  "loop_music": true,
  "sound_volume": 100.0,
  "music_volume": 80.0,
  "sound_pitch": 1.0,
  "music_pitch": 1.0,
  "position": { "x": 0, "y": 0, "z": 0 },
  "min_distance": 1.0,
  "attenuation": 1.0
}
```

**API Methods**:
- `playSound(soundId)`, `stopSound()`, `pauseSound()`, `resumeSound()`
- `playMusic(musicId, loop)`, `stopMusic()`, `pauseMusic()`, `resumeMusic()`
- `setSoundVolume(volume)`, `setMusicVolume(volume)`
- `setSoundPitch(pitch)`, `setMusicPitch(pitch)`
- `setPosition(x, y, z)` - For 3D spatial audio
- `setMinDistance(distance)`, `setAttenuation(attenuation)`

**Use Cases**:
- Player footsteps with positional audio
- Background music management
- Environmental sound effects
- Voice acting and dialogue
- UI sound feedback

---

### 4. ParticleEmitter Component

**Purpose**: Particle system for visual effects like explosions, fire, smoke, etc.

**Key Features**:
- Configurable emission rate and max particles
- Particle lifetime, speed, size, and color ranges
- Emitter shape (radius) and angle control
- Gravity and force simulation
- Real-time particle updates
- Auto-start option
- Performance-optimized rendering

**Configuration Example**:
```json
{
  "position": { "x": 400, "y": 300 },
  "emission_rate": 50.0,
  "max_particles": 500,
  "min_lifetime": 0.5,
  "max_lifetime": 2.0,
  "min_speed": 50.0,
  "max_speed": 150.0,
  "min_size": 2.0,
  "max_size": 8.0,
  "color": { "r": 255, "g": 100, "b": 0, "a": 255 },
  "radius": 10.0,
  "min_angle": 0.0,
  "max_angle": 360.0,
  "gravity": { "x": 0, "y": 98.0 },
  "auto_start": false
}
```

**API Methods**:
- `start()`, `stop()`, `pause()`, `resume()`
- `setEmissionRate(rate)`, `setMaxParticles(count)`
- `setParticleLifetime(min, max)`, `setParticleSpeed(min, max)`, `setParticleSize(min, max)`
- `setParticleColor(r, g, b, a)`, `setParticleColorRange(r1, g1, b1, a1, r2, g2, b2, a2)`
- `setEmitterPosition(x, y)`, `setEmitterRadius(radius)`, `setEmitterAngle(min, max)`
- `setGravity(x, y)`
- `update(deltaTime)` - Must be called each frame
- `clear()` - Remove all active particles
- `getActiveParticleCount()`

**Use Cases**:
- Explosion effects
- Fire, smoke, and steam
- Magic spell effects
- Weather effects (rain, snow)
- Debris and destruction particles
- Trail effects for projectiles

---

### 5. PhysicsBody Component

**Purpose**: Basic physics simulation for entity movement and forces.

**Key Features**:
- Velocity and acceleration control
- Force accumulation and impulse application
- Mass, drag, and angular properties
- Gravity scale
- Kinematic mode (non-physics controlled)
- Automatic force integration

**Configuration Example**:
```json
{
  "velocity": { "x": 0, "y": 0 },
  "acceleration": { "x": 0, "y": 0 },
  "mass": 1.0,
  "drag": 0.1,
  "angular_velocity": 0.0,
  "angular_drag": 0.1,
  "gravity_scale": 1.0,
  "kinematic": false
}
```

**API Methods**:
- `setVelocity(x, y)`, `getVelocity(x, y)`, `addVelocity(x, y)`
- `setAcceleration(x, y)`, `getAcceleration(x, y)`
- `applyForce(x, y)` - Continuous force
- `applyImpulse(x, y)` - Instant velocity change
- `clearForces()`
- `setMass(mass)`, `getMass()`
- `setDrag(drag)`, `getDrag()`
- `setAngularVelocity(velocity)`, `setAngularDrag(drag)`
- `setGravityScale(scale)`, `getGravityScale()`
- `update(deltaTime)` - Must be called each frame to integrate physics
- `setKinematic(bool)`, `isKinematic()`

**Use Cases**:
- Platformer character movement with gravity
- Physics-based projectiles
- Rigid body simulation
- Vehicle physics
- Falling objects and debris

---

### 6. Collision Component

**Purpose**: Collision detection and response for game physics.

**Key Features**:
- Rectangle and circle collision shapes
- Collision offset from entity center
- Layer-based collision filtering
- Trigger mode (no physical response)
- Enable/disable collision
- Collision callbacks (enter, exit, stay)
- Efficient AABB and circle collision tests

**Configuration Example**:
```json
{
  "shape": "rectangle",
  "width": 32.0,
  "height": 32.0,
  "radius": 16.0,
  "offset": { "x": 0, "y": 0 },
  "enabled": true,
  "is_trigger": false,
  "layer": 0,
  "mask": 4294967295
}
```

**API Methods**:
- `setShape(Shape)`, `getShape()` - Rectangle or Circle
- `setBounds(width, height)`, `setRadius(radius)`
- `setOffset(x, y)` - Offset from entity position
- `setEnabled(bool)`, `isEnabled()`
- `setTrigger(bool)`, `isTrigger()`
- `setLayer(layer)`, `setMask(mask)` - For selective collision
- `checkCollision(other, x1, y1, x2, y2)` - Test collision between two components
- `setOnCollisionEnter(callback)`, `setOnCollisionExit(callback)`, `setOnCollisionStay(callback)`

**Use Cases**:
- Player-enemy collision detection
- Collectible item pickup
- Platform and wall collision
- Trigger zones and level boundaries
- Projectile hit detection
- Environmental hazards

---

### 7. Timer Component

**Purpose**: Entity-specific timing, delays, and periodic events.

**Key Features**:
- Start, stop, pause, resume controls
- Duration-based timing
- Loop support
- Tick interval for repeated callbacks
- Completion and tick callbacks
- Elapsed and remaining time queries

**Configuration Example**:
```json
{
  "duration": 5.0,
  "loop": false,
  "tick_interval": 1.0,
  "auto_start": false
}
```

**API Methods**:
- `start()`, `stop()`, `pause()`, `resume()`, `reset()`
- `isRunning()`, `isPaused()`
- `setDuration(seconds)`, `getDuration()`
- `getElapsedTime()`, `getRemainingTime()`, `hasElapsed()`
- `setLoop(bool)`, `isLooping()`
- `update(deltaTime)` - Must be called each frame
- `setOnComplete(callback)` - Called when timer finishes
- `setOnTick(callback)` - Called at tick intervals
- `setTickInterval(seconds)`, `getTickInterval()`

**Use Cases**:
- Countdown timers
- Cooldown systems for abilities
- Delayed actions (spawn enemies after 5 seconds)
- Timed power-ups
- Periodic events (health regeneration every 2 seconds)
- Game round timers

---

### 8. StateMachine Component

**Purpose**: Entity state management with callbacks for state transitions.

**Key Features**:
- Dynamic state addition/removal
- State change with enter/exit callbacks
- Update callback per state
- Previous state tracking
- Custom data storage per state
- Safe state transitions

**Configuration Example**:
```json
{
  "states": {
    "idle": {},
    "walking": {},
    "jumping": {},
    "attacking": {}
  },
  "initial_state": "idle"
}
```

**API Methods**:
- `addState(name)`, `removeState(name)`, `hasState(name)`
- `changeState(name)` - Triggers exit callback on old state, enter on new
- `getCurrentState()`, `getPreviousState()`
- `setOnStateEnter(state, callback)`, `setOnStateExit(state, callback)`, `setOnStateUpdate(state, callback)`
- `update(deltaTime)` - Calls current state's update callback
- `setStateData(state, key, value)`, `getStateData(state, key)` - Store custom data

**Use Cases**:
- Character state machines (idle, walk, run, jump, attack)
- Enemy AI behavior states
- Game flow states (menu, playing, paused, game over)
- Animation state control
- UI state management
- Boss fight phases

---

## Tasks

All tasks follow Harmony conventions:
- Priority 50 (normal)
- FastMultiThreaded execution mode
- Comprehensive error handling with try-catch
- Logging at appropriate levels
- Safe null/validity checks

### Sprite Tasks (4)

#### PlaySpriteAnimationTask
- **Purpose**: Start playing a sprite's animation
- **Parameters**: sceneId, entityId, loop (default: true)
- **Example**: `new PlaySpriteAnimationTask(sceneId, playerEntity, true)`

#### StopSpriteAnimationTask
- **Purpose**: Stop a sprite's animation
- **Parameters**: sceneId, entityId
- **Example**: `new StopSpriteAnimationTask(sceneId, playerEntity)`

#### SetSpriteFrameTask
- **Purpose**: Set sprite to specific animation frame
- **Parameters**: sceneId, entityId, frameIndex
- **Example**: `new SetSpriteFrameTask(sceneId, playerEntity, 3)`

#### SetSpriteTextureTask
- **Purpose**: Change sprite's texture
- **Parameters**: sceneId, entityId, textureId
- **Example**: `new SetSpriteTextureTask(sceneId, playerEntity, newTextureId)`

### Camera Tasks (4)

#### SetActiveCameraTask
- **Purpose**: Set a camera as the active rendering camera
- **Parameters**: sceneId, cameraEntityId
- **Example**: `new SetActiveCameraTask(sceneId, mainCameraEntity)`

#### MoveCameraTask
- **Purpose**: Move camera to specific position
- **Parameters**: sceneId, cameraEntityId, x, y
- **Example**: `new MoveCameraTask(sceneId, cameraEntity, 400.0f, 300.0f)`

#### ZoomCameraTask
- **Purpose**: Set camera zoom level
- **Parameters**: sceneId, cameraEntityId, zoomLevel
- **Example**: `new ZoomCameraTask(sceneId, cameraEntity, 1.5f)` // 1.5x zoom

#### ResetCameraTask
- **Purpose**: Reset camera to default view
- **Parameters**: sceneId, cameraEntityId
- **Example**: `new ResetCameraTask(sceneId, cameraEntity)`

### Audio Tasks (4)

#### PlayEntityAudioTask
- **Purpose**: Play sound on entity
- **Parameters**: sceneId, entityId, soundId
- **Example**: `new PlayEntityAudioTask(sceneId, playerEntity, jumpSoundId)`

#### StopEntityAudioTask
- **Purpose**: Stop sound and/or music on entity
- **Parameters**: sceneId, entityId, stopSound (default: true), stopMusic (default: true)
- **Example**: `new StopEntityAudioTask(sceneId, entity, true, false)` // Stop sound only

#### SetAudioVolumeTask
- **Purpose**: Set volume for sound or music
- **Parameters**: sceneId, entityId, volume (0-100), isMusic (default: false)
- **Example**: `new SetAudioVolumeTask(sceneId, entity, 75.0f, true)` // Set music volume to 75%

#### PlayEntityMusicTask
- **Purpose**: Play music on entity
- **Parameters**: sceneId, entityId, musicId, loop (default: true)
- **Example**: `new PlayEntityMusicTask(sceneId, entity, bgMusicId, true)`

### Particle Tasks (3)

#### StartParticleEmitterTask
- **Purpose**: Start particle emission
- **Parameters**: sceneId, entityId
- **Example**: `new StartParticleEmitterTask(sceneId, explosionEntity)`

#### StopParticleEmitterTask
- **Purpose**: Stop particle emission
- **Parameters**: sceneId, entityId
- **Example**: `new StopParticleEmitterTask(sceneId, explosionEntity)`

#### ClearParticlesTask
- **Purpose**: Remove all active particles immediately
- **Parameters**: sceneId, entityId
- **Example**: `new ClearParticlesTask(sceneId, explosionEntity)`

### Physics Tasks (4)

#### ApplyForceTask
- **Purpose**: Apply continuous force to physics body
- **Parameters**: sceneId, entityId, forceX, forceY
- **Example**: `new ApplyForceTask(sceneId, ballEntity, 100.0f, 0.0f)` // Push right

#### ApplyImpulseTask
- **Purpose**: Apply instant impulse to physics body
- **Parameters**: sceneId, entityId, impulseX, impulseY
- **Example**: `new ApplyImpulseTask(sceneId, playerEntity, 0.0f, -500.0f)` // Jump

#### SetVelocityTask
- **Purpose**: Directly set physics body velocity
- **Parameters**: sceneId, entityId, velocityX, velocityY
- **Example**: `new SetVelocityTask(sceneId, projectileEntity, 200.0f, 0.0f)`

#### ClearForcesTask
- **Purpose**: Clear all accumulated forces
- **Parameters**: sceneId, entityId
- **Example**: `new ClearForcesTask(sceneId, entity)`

### Collision Tasks (3)

#### EnableCollisionTask
- **Purpose**: Enable collision detection on entity
- **Parameters**: sceneId, entityId
- **Example**: `new EnableCollisionTask(sceneId, entity)`

#### DisableCollisionTask
- **Purpose**: Disable collision detection on entity
- **Parameters**: sceneId, entityId
- **Example**: `new DisableCollisionTask(sceneId, entity)`

#### SetCollisionLayerTask
- **Purpose**: Set collision layer for filtering
- **Parameters**: sceneId, entityId, layer
- **Example**: `new SetCollisionLayerTask(sceneId, entity, 2)` // Layer 2

### Timer Tasks (3)

#### StartTimerTask
- **Purpose**: Start or restart timer
- **Parameters**: sceneId, entityId
- **Example**: `new StartTimerTask(sceneId, timerEntity)`

#### StopTimerTask
- **Purpose**: Stop and reset timer
- **Parameters**: sceneId, entityId
- **Example**: `new StopTimerTask(sceneId, timerEntity)`

#### ResetTimerTask
- **Purpose**: Reset timer to zero without stopping
- **Parameters**: sceneId, entityId
- **Example**: `new ResetTimerTask(sceneId, timerEntity)`

### StateMachine Tasks (3)

#### ChangeEntityStateTask
- **Purpose**: Change entity's current state
- **Parameters**: sceneId, entityId, stateName
- **Example**: `new ChangeEntityStateTask(sceneId, enemy, "attacking")`

#### AddEntityStateTask
- **Purpose**: Add new state to entity's state machine
- **Parameters**: sceneId, entityId, stateName
- **Example**: `new AddEntityStateTask(sceneId, player, "dashing")`

#### RemoveEntityStateTask
- **Purpose**: Remove state from entity's state machine
- **Parameters**: sceneId, entityId, stateName
- **Example**: `new RemoveEntityStateTask(sceneId, enemy, "deprecated_state")`

---

## Usage Examples

### Example 1: Animated Character with Physics

```cpp
// Entity configuration in JSON
{
  "name": "Player",
  "components": {
    "Transform": {
      "position": {"x": 100, "y": 100},
      "rotation": 0,
      "scale": {"x": 1, "y": 1}
    },
    "Sprite": {
      "texture": 5000,
      "texture_rect": {"left": 0, "top": 0, "width": 32, "height": 32}
    },
    "PhysicsBody": {
      "mass": 1.0,
      "drag": 0.1,
      "gravity_scale": 1.0
    },
    "Collision": {
      "shape": "rectangle",
      "width": 30,
      "height": 30,
      "layer": 1
    },
    "StateMachine": {
      "states": {
        "idle": {},
        "walking": {},
        "jumping": {}
      },
      "initial_state": "idle"
    }
  }
}

// In game code - Jump action
auto impulseTask = std::make_unique<ApplyImpulseTask>(sceneId, playerEntity, 0.0f, -300.0f);
engine.taskManagement->submit(std::move(impulseTask));

auto stateTask = std::make_unique<ChangeEntityStateTask>(sceneId, playerEntity, "jumping");
engine.taskManagement->submit(std::move(stateTask));
```

### Example 2: Particle Effect System

```cpp
// Create explosion particle emitter
{
  "name": "Explosion",
  "components": {
    "Transform": {
      "position": {"x": 300, "y": 200}
    },
    "ParticleEmitter": {
      "emission_rate": 100.0,
      "max_particles": 200,
      "min_lifetime": 0.3,
      "max_lifetime": 1.0,
      "min_speed": 50.0,
      "max_speed": 200.0,
      "color": {"r": 255, "g": 100, "b": 0, "a": 255},
      "gravity": {"x": 0, "y": 50.0}
    },
    "Timer": {
      "duration": 2.0,
      "auto_start": true
    }
  }
}

// Start explosion
auto startTask = std::make_unique<StartParticleEmitterTask>(sceneId, explosionEntity);
engine.taskManagement->submit(std::move(startTask));

// Stop after timer completes (set timer callback)
timer.setOnComplete([&]() {
    auto stopTask = std::make_unique<StopParticleEmitterTask>(sceneId, explosionEntity);
    engine.taskManagement->submit(std::move(stopTask));
    
    auto destroyTask = std::make_unique<DestroyEntityTask>(sceneId, explosionEntity);
    engine.taskManagement->submit(std::move(destroyTask));
});
```

### Example 3: Camera Follow System

```cpp
// In script update
void FollowCameraScript::onPreUpdate() override {
    auto& camera = getScene().componentReference<Camera>(cameraEntityId);
    auto& playerTransform = getScene().componentReference<Transform>(playerEntityId);
    
    float playerX, playerY;
    playerTransform.getPosition(playerX, playerY);
    
    // Smooth camera follow
    float cameraX, cameraY;
    camera.getCenter(cameraX, cameraY);
    
    float smoothing = 0.1f;
    float newX = cameraX + (playerX - cameraX) * smoothing;
    float newY = cameraY + (playerY - cameraY) * smoothing;
    
    auto moveTask = std::make_unique<MoveCameraTask>(getScene().sceneId, cameraEntityId, newX, newY);
    getScene().engine.taskManagement->submit(std::move(moveTask));
}
```

### Example 4: Timed Power-Up

```cpp
// Power-up entity
{
  "name": "SpeedBoost",
  "components": {
    "Timer": {
      "duration": 5.0,
      "loop": false
    },
    "Audio": {
      "sound": 7001
    }
  }
}

// Apply power-up
void applySpeedBoost(EntityID playerEntity, EntityID powerUpEntity) {
    // Increase player speed
    auto& physics = scene.componentReference<PhysicsBody>(playerEntity);
    physics.setVelocity(physics.getVelocity() * 2.0f);
    
    // Start timer
    auto startTimer = std::make_unique<StartTimerTask>(sceneId, powerUpEntity);
    engine.taskManagement->submit(std::move(startTimer));
    
    // Play sound
    auto playSound = std::make_unique<PlayEntityAudioTask>(sceneId, powerUpEntity, 7001);
    engine.taskManagement->submit(std::move(playSound));
    
    // Set timer completion callback
    auto& timer = scene.componentReference<Timer>(powerUpEntity);
    timer.setOnComplete([=]() {
        // Restore normal speed
        auto& physics = scene.componentReference<PhysicsBody>(playerEntity);
        physics.setVelocity(physics.getVelocity() * 0.5f);
    });
}
```

---

## Performance Considerations

### Sprite Component
- Animation updates are efficient but should be called once per frame
- Use sprite sheets to reduce texture switching
- Texture changes may cause state changes in rendering

### Camera Component
- Active camera checks should be minimized
- Camera updates are fast, suitable for smooth following

### Audio Component
- 3D positional audio has computational cost
- Limit simultaneous sound sources
- Consider audio pooling for frequently used sounds

### ParticleEmitter Component
- Particle count directly impacts performance
- Use `maxParticles` to limit CPU/GPU load
- Clear particles when emitter is destroyed
- Update calls should be optimized (already use vertex arrays)

### PhysicsBody Component
- Physics updates are per-entity, can be expensive with many bodies
- Consider using kinematic mode for entities that don't need physics
- Drag and force accumulation are optimized

### Collision Component
- Collision checks are O(n²) if checking all pairs
- Use layers and masks to reduce check count
- Broad-phase optimization recommended for many entities

### Timer Component
- Very lightweight, minimal performance impact
- Suitable for many concurrent timers

### StateMachine Component
- Callback overhead is minimal
- State changes are efficient with hash map lookup

---

## Safety and Error Handling

All components and tasks implement:

1. **Null Safety**: Check for null pointers and invalid references
2. **Exception Handling**: Try-catch blocks in all task run() methods
3. **Logging**: Errors, warnings, and debug information at appropriate levels
4. **Resource Management**: RAII with smart pointers, no memory leaks
5. **Thread Safety**: Compatible with Harmony's task system threading model
6. **Validation**: Input validation for all parameters
7. **Graceful Degradation**: Continue operation when non-critical errors occur

Error messages follow the pattern:
```
HARMONY_ERROR("{TaskName}: {Context} - {Error Details}")
```

---

## Integration with Existing Systems

### Component Registration
All components are registered using the `HARMONY_REGISTER_COMPONENT` macro:
```cpp
HARMONY_REGISTER_COMPONENT(Harmony::Components::Sprite, Sprite)
```

Drawable components use:
```cpp
HARMONY_REGISTER_COMPONENT_WITH_BASE(sf::Drawable, Harmony::Components::Sprite, Sprite)
```

### Task Submission
Tasks are submitted through the TaskManager:
```cpp
auto task = std::make_unique<TaskType>(parameters...);
engine.taskManagement->submit(std::move(task));
```

### Scene Integration
Components are accessed through scenes:
```cpp
auto& component = scene.componentReference<ComponentType>(entityId);
```

### Update Pattern
Components requiring per-frame updates (Sprite, ParticleEmitter, PhysicsBody, Timer, StateMachine) should have their update methods called in a script's `onPreUpdate()` or `onPostUpdate()`:

```cpp
void GameScript::onPreUpdate() override {
    // Update all animated sprites
    for (auto entityId : spriteEntities) {
        auto& sprite = getScene().componentReference<Sprite>(entityId);
        sprite.updateAnimation(getScene().engine.getDeltaTime());
    }
    
    // Update physics
    for (auto entityId : physicsEntities) {
        auto& physics = getScene().componentReference<PhysicsBody>(entityId);
        physics.update(getScene().engine.getDeltaTime());
    }
    
    // Update timers
    for (auto entityId : timerEntities) {
        auto& timer = getScene().componentReference<Timer>(entityId);
        timer.update(getScene().engine.getDeltaTime());
    }
}
```

---

## Summary

This addition provides:
- **8 new components** covering rendering, audio, physics, and game logic
- **27 new tasks** for controlling these components
- **Thread-safe** operations following Harmony patterns
- **Comprehensive error handling** and logging
- **Configuration-driven** initialization
- **Performance-optimized** implementations
- **Well-documented** API and usage examples

The new components significantly expand Harmony's capabilities while maintaining consistency with existing architecture and coding standards.
