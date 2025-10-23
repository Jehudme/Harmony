# Flappy Bird Visual Design Document

## Game Layout (1280x720)

```
┌──────────────────────────────────────────────────────────────────┐
│  Sky Background (Sky Blue - 135, 206, 235)                       │
│                                                                   │
│  ☁️ Cloud       🌞 Sun                        ☁️ Cloud          │
│                                                                   │
│                   🎯 Score                                        │
│                                                                   │
│                                                                   │
│                                          ┃                        │
│                  🐦 Bird                 ┃ Top Pipe              │
│                                          ┃                        │
│                                          ╋─────╋                  │
│                                                                   │
│                                          ╋─────╋                  │
│                                          ┃                        │
│                                          ┃ Bottom Pipe            │
│  🌿 Grass                                ┃                        │
│──────────────────────────────────────────────────────────────────│
│  Ground (Brown - 139, 69, 19)                                    │
└──────────────────────────────────────────────────────────────────┘
```

## Entity Hierarchy

### Layer -20: Background (Farthest)
- **Sky Background 1** (640, 360) - 1280x720 rectangle
  - Light blue background
  - Scrolls slowly (50 px/s)
- **Sky Background 2** (1920, 360) - 1280x720 rectangle
  - Duplicate for seamless scrolling

### Layer 0: Gameplay (Middle)
- **Ground 1** (640, 670) - 1280x100 brown rectangle
  - Scrolls at 200 px/s
  - Wraps around at -640
- **Ground 2** (1920, 670) - Duplicate ground

#### Obstacles (3 pipe pairs)
- **Pipe Set 1** (x=800)
  - Top Pipe (800, 125) - 80x250 green
  - Pipe Cap Top (800, 250) - 100x30 medium sea green
  - Bottom Pipe (800, 525) - 80x250 green
  - Pipe Cap Bottom (800, 400) - 100x30 medium sea green
  
- **Pipe Set 2** (x=1200)
  - Top Pipe (1200, 150) - 80x300 green
  - Pipe Cap Top (1200, 300) - 100x30 medium sea green
  - Bottom Pipe (1200, 550) - 80x200 green
  - Pipe Cap Bottom (1200, 450) - 100x30 medium sea green

- **Pipe Set 3** (x=1600)
  - Top Pipe (1600, 100) - 80x200 green
  - Pipe Cap Top (1600, 200) - 100x30 medium sea green
  - Bottom Pipe (1600, 500) - 80x200 green
  - Pipe Cap Bottom (1600, 400) - 100x30 medium sea green

#### Decorations
- **Cloud 1** (300, 150) - 60x60 white circle, 180 alpha
- **Cloud 2** (700, 200) - 80x80 white circle, 180 alpha
- **Grass Tuft 1** (200, 630) - 20x30 green rectangle
- **Grass Tuft 2** (500, 630) - 25x35 lime green rectangle
- **Grass Tuft 3** (900, 630) - 20x30 green rectangle
- **Grass Tuft 4** (1100, 630) - 25x35 lime green rectangle

### Layer 10: Player (Foreground)
- **Bird Body** (150, 300) - 40x40 golden circle
  - Orange outline (3px)
- **Bird Wing** (165, 295) - 15x15 orange circle
  - No outline
- **Bird Eye** (160, 290) - 8x8 black circle
  - Positioned on front of bird

### Layer 50: UI (Nearest)
- **Score Indicator** (50, 30) - 30x30 golden circle
  - Visual score representation
- **Sun Decoration** (1100, 100) - 80x80 yellow circle
  - Orange outline (5px)
- **Game Manager** (0, 0) - Invisible entity
  - Handles game state

## Color Palette

### Primary Colors
| Element | RGB | Hex | Usage |
|---------|-----|-----|-------|
| Sky Blue | (135, 206, 235) | #87CEEB | Background sky |
| Golden Yellow | (255, 215, 0) | #FFD700 | Bird body, UI |
| Orange | (255, 140, 0) | #FF8C00 | Bird outlines, wing |
| Forest Green | (34, 139, 34) | #228B22 | Pipes |
| Saddle Brown | (139, 69, 19) | #8B4513 | Ground |

### Accent Colors
| Element | RGB | Hex | Usage |
|---------|-----|-----|-------|
| White | (255, 255, 255, 180) | #FFFFFF | Clouds (transparent) |
| Dark Green | (0, 100, 0) | #006400 | Pipe outlines |
| Lime Green | (50, 205, 50) | #32CD32 | Grass tufts |
| Medium Sea Green | (60, 179, 113) | #3CB371 | Pipe caps |
| Black | (0, 0, 0) | #000000 | Bird eye |
| Bright Yellow | (255, 223, 0) | #FFDF00 | Sun |

## Physics Parameters

### Bird
- **Starting Position**: (150, 300)
- **Size**: 40x40 pixels
- **Gravity**: 980 px/s²
- **Jump Velocity**: -350 px/s
- **Terminal Velocity**: 600 px/s
- **Hitbox**: 20px radius from center

### Pipes
- **Width**: 80px (body) / 100px (cap)
- **Gap Size**: ~150-200px
- **Scroll Speed**: 200 px/s
- **Recycle Distance**: x < -100
- **Respawn Position**: x = 1400

### Background
- **Sky Scroll Speed**: 50 px/s (parallax)
- **Ground Scroll Speed**: 200 px/s
- **Wrap Distance**: -640px

## Collision Zones

### Bird Collision Box
```
     Top (y - 20)
       ┌─────┐
Left   │ 🐦  │   Right
(x-20) │     │   (x+20)
       └─────┘
    Bottom (y + 20)
```

### Pipe Collision Zones
```
Top Pipe:
┌─────────┐ Cap (100x30)
├─────────┤
│         │ Body (80xH)
│    ┃    │
│    ┃    │
└────┃────┘
     ↓ Bottom edge is collision boundary

Bottom Pipe:
     ↑ Top edge is collision boundary
┌────┃────┐
│    ┃    │
│    ┃    │ Body (80xH)
├─────────┤
└─────────┘ Cap (100x30)
```

### Boundary Collision
- **Ceiling**: y < 20
- **Ground**: y > 600

## Animation States

### Bird States
1. **Idle/Rising**: Bird has upward velocity (negative y velocity)
2. **Falling**: Bird affected by gravity (positive y velocity)
3. **Game Over**: Bird frozen at collision position

### Environmental
1. **Background**: Continuous slow scroll (parallax)
2. **Ground**: Continuous fast scroll with wrap
3. **Pipes**: Fast scroll with recycling
4. **Clouds**: Static decorations

## Scoring System

### Score Calculation
- **Points per pipe**: +1
- **Scoring trigger**: Bird x-position passes pipe x-position (150 < pipe_x)
- **Score counted**: Only once per pipe (bottom pipe only to avoid double counting)
- **Display**: Visual indicator (golden circle count)

### High Score (Future Enhancement)
- Could be stored in configuration.json
- Displayed as separate UI element
- Persisted between game sessions

## Game States

### Playing State
- Bird responds to SPACE key
- All entities update and scroll
- Collision detection active
- Score increments

### Game Over State
- Bird stops at collision point
- All scrolling pauses
- Collision detection disabled
- "R" key enables restart

### Reset State (R key pressed)
- Scene.reset() called
- All entities return to initial positions
- Score resets to 0
- Game returns to Playing state

## Technical Details

### Entity Count
- Total: 31 entities
- Background: 2 entities
- Gameplay: 23 entities (ground, pipes, caps, decorations)
- Player: 3 entities (body, wing, eye)
- UI: 3 entities

### Script Instances
- FlappyBirdScript: 1 (bird)
- PipeScript: 12 (6 pipe bodies + 6 pipe caps)
- GroundScript: 2 (ground tiles)
- BackgroundScript: 2 (sky tiles)
- GameManagerScript: 1 (game state)
- Total: 18 active scripts

### Component Breakdown
- Transform: 31 (one per entity)
- Rectangle: 24 (ground, pipes, grass)
- Circle: 7 (bird, clouds, decorations)
- Script: 18 (behavior components)
- Total: 80 component instances

## Visual Enhancements

### Current Features
✅ Multi-layer parallax scrolling  
✅ Colorful sprite-like shapes  
✅ Decorative elements (clouds, sun, grass)  
✅ Bird detail (body, wing, eye)  
✅ Pipe caps for 3D effect  
✅ Smooth animations  
✅ Consistent color palette  

### Potential Improvements
- Add particle effects on collision
- Animated wing flapping based on velocity
- Day/night cycle with color transitions
- Weather effects (rain, wind)
- More varied cloud shapes and positions
- Score display with text (requires font)
- Animated grass swaying
- Bird rotation based on velocity
- Trail effect behind bird
- Sparkle effects on score increment

## Implementation Notes

### Performance Optimization
- All entities use efficient ECS architecture
- Minimal draw calls with proper batching
- Delta-time based physics for frame independence
- Recycling system prevents entity creation overhead
- Static game state reduces communication overhead

### Code Organization
- All game scripts in single file (FlappyBirdScripts.cpp)
- Clear separation of concerns (bird, pipes, background, game state)
- Configuration-driven design (no hardcoded entities)
- Modular script system (easy to add new behaviors)

### Testing Considerations
- Collision detection accuracy
- Physics feel (gravity, jump strength)
- Visual alignment (hitboxes vs sprites)
- Performance under load (entity count)
- Edge cases (very fast scrolling, multiple collisions)
