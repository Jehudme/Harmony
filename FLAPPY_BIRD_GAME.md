# Flappy Bird Game - Harmony Engine Demo

A fully-featured Flappy Bird clone built to showcase the capabilities of the Harmony Engine.

## Overview

This implementation demonstrates the engine's core features including:
- Entity-Component-System architecture
- Multi-layer rendering with draw orders
- Physics simulation with delta-time
- Collision detection
- Input handling
- Script-based game logic
- Scene management
- Configuration-driven entity creation

## How to Play

### Controls
- **SPACE**: Jump/Flap the bird
- **R**: Restart game after Game Over
- **ESC/Close Window**: Exit game

### Objective
Guide the golden bird through gaps in the green pipes without hitting them, the ground, or the ceiling. Each pipe you successfully pass increases your score.

### Game Mechanics
- The bird is constantly affected by gravity (980 px/s²)
- Pressing SPACE gives the bird an upward impulse (-350 px/s)
- Collision with pipes, ground, or ceiling ends the game
- Pipes continuously scroll from right to left and recycle
- Score increases each time you pass through a pipe gap

## Building and Running

### Prerequisites
- Windows OS
- Visual Studio 2019 or later
- vcpkg package manager (configured automatically)

### Build Steps
1. Open `Harmony.sln` in Visual Studio
2. Restore NuGet/vcpkg packages (automatic on first build)
3. Set `Sandbox` as the startup project
4. Build the solution (Ctrl+Shift+B)
5. Run the game (F5)

### Configuration
The game is configured entirely through `Sandbox/configuration.json`. All entities, scenes, and behaviors are defined there.

## Technical Implementation

### Architecture

#### Scripts (5 custom scripts)
1. **FlappyBirdScript**: Controls the bird's physics, input, and collision detection
2. **PipeScript**: Handles pipe movement, scoring, and recycling
3. **GroundScript**: Manages ground scrolling animation
4. **BackgroundScript**: Creates parallax background effect
5. **GameManagerScript**: Manages game state and restart functionality

#### Scenes (4 layered scenes)
1. **Scene 100** (drawOrder: -20): Background layer with sky and scrolling clouds
2. **Scene 200** (drawOrder: 0): Gameplay layer with ground, pipes, and decorative clouds
3. **Scene 300** (drawOrder: 10): Player layer with the bird entity
4. **Scene 400** (drawOrder: 50): UI layer with score indicators and decorations

#### Entities (18 entities total)
- **Background**: 2 sky rectangles for seamless scrolling
- **Ground**: 2 ground rectangles for continuous movement
- **Pipes**: 6 pipe rectangles (3 pairs) with recycling system
- **Clouds**: 2 decorative cloud circles for atmosphere
- **Bird**: 2 circles (body + wing) forming the player character
- **UI**: 3 elements (score indicator, sun decoration, game manager)

### Physics System
```
Gravity: 980 px/s²
Jump Strength: -350 px/s
Max Velocity: 600 px/s
Scroll Speed (Pipes/Ground): 200 px/s
Scroll Speed (Background): 50 px/s (parallax effect)
```

### Collision Detection
- **Algorithm**: Axis-Aligned Bounding Box (AABB)
- **Bird Hitbox**: 40x40 pixels (20px radius from center)
- **Pipe Hitbox**: 80px width, variable height
- **Boundary Checks**: Ground (y > 600), Ceiling (y < 20)

### Visual Design

#### Color Palette
- **Sky**: Light Blue (RGB: 135, 206, 235)
- **Bird**: Golden Yellow (RGB: 255, 215, 0) with Orange (RGB: 255, 140, 0) outlines
- **Pipes**: Forest Green (RGB: 34, 139, 34) with Dark Green (RGB: 0, 100, 0) outlines
- **Ground**: Brown (RGB: 139, 69, 19) with Saddle Brown (RGB: 101, 67, 33) outlines
- **Clouds**: White (RGB: 255, 255, 255) with 180 alpha transparency
- **Sun**: Bright Yellow (RGB: 255, 223, 0) with Orange (RGB: 255, 165, 0) outline

#### Layer System
```
Draw Order -20: Sky background (farthest)
Draw Order   0: Ground and pipes (middle)
Draw Order  10: Player bird (foreground)
Draw Order  50: UI elements (nearest)
```

### Game State Management
- **Static State Variables**: `isGameOver` and `currentScore`
- **Cross-Entity Communication**: Scripts share state through GameManagerScript
- **Reset System**: Scene.reset() restores all entities to initial configuration

## Code Structure

### Header File: FlappyBirdScripts.h
Contains declarations for all 5 game scripts with their private member variables.

### Implementation File: FlappyBirdScripts.cpp
Implements game logic including:
- Bird physics and input handling
- Collision detection with all obstacles
- Pipe movement and recycling
- Scrolling backgrounds and ground
- Game state management

### Configuration File: configuration.json
Defines all game entities, components, scenes, and states in JSON format.

### Project Files
- **Harmony.vcxproj**: Added FlappyBirdScripts.h/.cpp to build
- **Harmony.vcxproj.filters**: Organized files in Components folder

## Features Demonstrated

### Engine Capabilities
✅ **Entity-Component-System**: Efficient entity management with EnTT  
✅ **Script Components**: Custom behavior through C++ scripts  
✅ **Transform Component**: Position, rotation, scale management  
✅ **Shape Components**: Circle and Rectangle rendering  
✅ **Multi-Layer Rendering**: Draw order system for proper layering  
✅ **Configuration System**: JSON-driven entity creation  
✅ **Input Handling**: SFML keyboard integration  
✅ **Delta Time**: Frame-rate independent physics  
✅ **Scene Management**: Multiple scenes with enable/disable  
✅ **State Management**: Game state tracking and reset  

### Game Features
✅ **Smooth Physics**: Gravity and jump mechanics  
✅ **Collision Detection**: AABB with ceiling, ground, and pipes  
✅ **Infinite Gameplay**: Pipe recycling system  
✅ **Score Tracking**: Increment on passing pipes  
✅ **Visual Polish**: Layered rendering with decorations  
✅ **Parallax Scrolling**: Multi-speed background  
✅ **Game Over State**: Detection and restart capability  
✅ **Responsive Controls**: Instant jump response  

## Performance

### Entity Count: 18 entities
### Component Count: ~36 components
### Active Scripts: 15+ script instances
### Target FPS: 60 (configurable)
### Render Layers: 4 distinct draw orders

## Extensibility

### Easy Modifications
1. **Adjust Difficulty**: Modify gravity/jump in FlappyBirdScript
2. **Change Visuals**: Update colors in configuration.json
3. **Add Entities**: Define new entities in configuration
4. **New Mechanics**: Create additional script components
5. **Sound Effects**: Add sound resources and play on events

### Potential Enhancements
- Add Text component for score display (requires font file)
- Implement sound effects for jump and collision
- Add particle effects for explosions
- Create multiple difficulty levels
- Add high score persistence
- Implement power-ups or obstacles
- Add animations to bird sprite

## Learning Resources

This game serves as an excellent reference for:
- Creating custom script components
- Implementing game physics
- Managing game state across entities
- Using the configuration system effectively
- Organizing entities with draw orders
- Handling input in scripts
- Building complete games with Harmony Engine

## Credits

**Engine**: Harmony Engine v1.0.0  
**Game Design**: Inspired by the classic Flappy Bird  
**Implementation**: Jehudme  
**Purpose**: Engine testing and demonstration  

## License

This demo game follows the same MIT License as the Harmony Engine.
