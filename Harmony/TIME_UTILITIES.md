# Harmony Time Utilities

A complete set of professional time and clock utilities for the Harmony game engine, inspired by SFML's time module.

## Overview

This module provides high-precision time measurement and management utilities essential for game development, including frame timing, cooldowns, performance monitoring, and more.

## Classes

### Time

Represents a time value with high precision, stored internally in microseconds.

```cpp
using namespace Harmony::Utilities;

// Create time values
Time oneSecond = Time::fromSeconds(1.0f);
Time halfSecond = Time::fromMilliseconds(500);
Time quarterSecond = Time::fromMicroseconds(250000);

// Convert between units
float seconds = oneSecond.asSeconds();        // 1.0
int32_t ms = oneSecond.asMilliseconds();     // 1000
int64_t us = oneSecond.asMicroseconds();     // 1000000

// Arithmetic operations
Time total = oneSecond + halfSecond;         // 1.5 seconds
Time difference = oneSecond - halfSecond;     // 0.5 seconds
Time doubled = oneSecond * 2;                 // 2.0 seconds
Time halved = oneSecond / 2;                  // 0.5 seconds

// Comparisons
if (oneSecond > halfSecond) {
    // true
}
```

### Clock

High-precision clock for measuring elapsed time.

```cpp
Clock clock;

// Do some work...
Time elapsed = clock.getElapsedTime();

// Restart and get time since last restart
Time timeSinceStart = clock.restart();
```

### Stopwatch

Advanced stopwatch with pause/resume and lap timing functionality.

```cpp
Stopwatch stopwatch;

stopwatch.start();
// Do something...

stopwatch.pause();
// Paused - time doesn't advance

stopwatch.start();  // Resume
Time lapTime = stopwatch.lap();  // Record a lap

Time total = stopwatch.getElapsedTime();
size_t laps = stopwatch.getLapCount();

stopwatch.reset();  // Reset to zero
```

### Timer

Countdown timer with callback support and auto-repeat.

```cpp
// Create a 5-second timer with callback
Timer timer(Time::fromSeconds(5.0f), []() {
    HARMONY_INFO("Timer expired!");
});

timer.start();

// In your game loop:
timer.update();  // Call every frame

// Check timer status
if (timer.hasExpired()) {
    // Timer completed
}

float progress = timer.getProgress();  // 0.0 to 1.0
Time remaining = timer.getRemainingTime();

// Auto-repeat timer
Timer autoTimer(Time::fromSeconds(1.0f), 
    []() { HARMONY_INFO("Tick!"); }, 
    true);  // Auto-repeat
```

### FPSCounter

Frame rate counter with statistics.

```cpp
FPSCounter fpsCounter(60);  // Average over 60 frames

// In your game loop:
fpsCounter.update();

float fps = fpsCounter.getFPS();
float avgFPS = fpsCounter.getAverageFPS();
float minFPS = fpsCounter.getMinFPS();
float maxFPS = fpsCounter.getMaxFPS();
float frameTime = fpsCounter.getFrameTime();
uint64_t totalFrames = fpsCounter.getTotalFrames();
```

## Use Cases

### Game Cooldown System

```cpp
class Weapon {
    Timer cooldown;
    
public:
    Weapon() : cooldown(Time::fromSeconds(0.5f)) {}
    
    void fire() {
        if (!cooldown.isRunning()) {
            // Fire weapon
            cooldown.start();
        }
    }
    
    void update() {
        cooldown.update();
    }
};
```

### Performance Profiling

```cpp
Stopwatch profiler;
profiler.start();

// Code to profile
processPhysics();

auto physicsTime = profiler.lap();
renderScene();

auto renderTime = profiler.lap();

HARMONY_INFO("Physics: {} ms, Render: {} ms", 
    physicsTime.asMilliseconds(), 
    renderTime.asMilliseconds());
```

### FPS Limiter

```cpp
Clock frameClock;
const Time targetFrameTime = Time::fromSeconds(1.0f / 60.0f);  // 60 FPS

while (running) {
    // Update game...
    
    Time frameTime = frameClock.restart();
    if (frameTime < targetFrameTime) {
        Time sleepTime = targetFrameTime - frameTime;
        std::this_thread::sleep_for(
            std::chrono::microseconds(sleepTime.asMicroseconds())
        );
    }
}
```

### Power-up Duration

```cpp
Timer powerUpTimer(Time::fromSeconds(10.0f), [this]() {
    deactivatePowerUp();
});

void activatePowerUp() {
    powerUpActive = true;
    powerUpTimer.start();
}

void update() {
    powerUpTimer.update();
    
    // Show remaining time to player
    if (powerUpActive) {
        float timeLeft = powerUpTimer.getRemainingTime().asSeconds();
        displayUI("Power-up: {:.1f}s", timeLeft);
    }
}
```

## Features

- **High Precision**: Microsecond-level accuracy using std::chrono
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **SFML-Compatible**: Similar API to SFML's time utilities
- **Modern C++**: Uses C++17/23 features
- **Well-Documented**: Comprehensive inline documentation
- **Tested**: Includes demo file with usage examples

## Integration

All utilities are in the `Harmony::Utilities` namespace and automatically included in the Harmony engine build.

To use in your code:

```cpp
#include "Time.h"
#include "Clock.h"
#include "Stopwatch.h"
#include "Timer.h"
#include "FPSCounter.h"

using namespace Harmony::Utilities;
```

## Demo

See `TimeUtilitiesDemo.cpp` for comprehensive examples of all utilities in action.

Run the demos:
```cpp
Harmony::Tests::runTimeUtilityDemos();
```

## License

Part of the Harmony game engine. See LICENSE.txt for details.
