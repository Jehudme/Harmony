// Demo/test for the new time utilities
// This file demonstrates usage of Time, Clock, Stopwatch, Timer, and FPSCounter

#include "pch.h"
#include "Time.h"
#include "Clock.h"
#include "Stopwatch.h"
#include "Timer.h"
#include "FPSCounter.h"
#include "Logger.h"

namespace Harmony::Tests {

    /// @brief Demonstrates Time class usage
    void demoTimeClass() {
        HARMONY_INFO("=== Time Class Demo ===");

        // Creating time values
        auto oneSecond = Utilities::Time::fromSeconds(1.0f);
        auto halfSecond = Utilities::Time::fromMilliseconds(500);
        auto quarterSecond = Utilities::Time::fromMicroseconds(250000);

        HARMONY_INFO("1 second = {} ms = {} μs", 
            oneSecond.asMilliseconds(), oneSecond.asMicroseconds());
        HARMONY_INFO("0.5 seconds = {} ms", halfSecond.asMilliseconds());
        HARMONY_INFO("0.25 seconds = {} ms", quarterSecond.asMilliseconds());

        // Arithmetic operations
        auto total = oneSecond + halfSecond;
        HARMONY_INFO("1.0s + 0.5s = {} seconds", total.asSeconds());

        auto difference = oneSecond - quarterSecond;
        HARMONY_INFO("1.0s - 0.25s = {} seconds", difference.asSeconds());

        auto doubled = oneSecond * 2;
        HARMONY_INFO("1.0s * 2 = {} seconds", doubled.asSeconds());

        // Comparisons
        if (oneSecond > halfSecond) {
            HARMONY_INFO("1 second is greater than 0.5 seconds");
        }
    }

    /// @brief Demonstrates Clock class usage
    void demoClockClass() {
        HARMONY_INFO("=== Clock Class Demo ===");

        Utilities::Clock clock;
        
        // Simulate some work
        HARMONY_INFO("Starting clock...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        auto elapsed = clock.getElapsedTime();
        HARMONY_INFO("Elapsed time: {} ms", elapsed.asMilliseconds());
        
        // Restart and measure again
        auto restartTime = clock.restart();
        HARMONY_INFO("Time before restart: {} ms", restartTime.asMilliseconds());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        elapsed = clock.getElapsedTime();
        HARMONY_INFO("Time after restart: {} ms", elapsed.asMilliseconds());
    }

    /// @brief Demonstrates Stopwatch class usage
    void demoStopwatchClass() {
        HARMONY_INFO("=== Stopwatch Class Demo ===");

        Utilities::Stopwatch stopwatch;
        
        // Start the stopwatch
        HARMONY_INFO("Starting stopwatch...");
        stopwatch.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Record first lap
        auto lap1 = stopwatch.lap();
        HARMONY_INFO("Lap 1: {} ms", lap1.asMilliseconds());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        // Record second lap
        auto lap2 = stopwatch.lap();
        HARMONY_INFO("Lap 2: {} ms", lap2.asMilliseconds());
        
        // Pause the stopwatch
        stopwatch.pause();
        auto pausedTime = stopwatch.getElapsedTime();
        HARMONY_INFO("Paused at: {} ms", pausedTime.asMilliseconds());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Resume and check time hasn't changed
        stopwatch.start();
        HARMONY_INFO("Time after resume (should be same): {} ms", 
            stopwatch.getElapsedTime().asMilliseconds());
        
        HARMONY_INFO("Total laps recorded: {}", stopwatch.getLapCount());
        
        // Reset
        stopwatch.reset();
        HARMONY_INFO("After reset - running: {}, elapsed: {} ms", 
            stopwatch.isRunning(), stopwatch.getElapsedTime().asMilliseconds());
    }

    /// @brief Demonstrates Timer class usage
    void demoTimerClass() {
        HARMONY_INFO("=== Timer Class Demo ===");

        bool timerFired = false;
        auto duration = Utilities::Time::fromMilliseconds(200);
        
        // Create timer with callback
        Utilities::Timer timer(duration, [&timerFired]() {
            HARMONY_INFO("Timer callback fired!");
            timerFired = true;
        });

        HARMONY_INFO("Starting timer for {} ms", duration.asMilliseconds());
        timer.start();

        // Simulate game loop
        while (!timer.hasExpired()) {
            timer.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            if (timer.isRunning()) {
                HARMONY_INFO("Progress: {:.1f}%, Remaining: {} ms",
                    timer.getProgress() * 100.0f,
                    timer.getRemainingTime().asMilliseconds());
            }
        }

        HARMONY_INFO("Timer completed. Callback fired: {}", timerFired);

        // Demo auto-repeat timer
        HARMONY_INFO("\n--- Auto-repeat Timer ---");
        int repeatCount = 0;
        Utilities::Timer repeatTimer(Utilities::Time::fromMilliseconds(100), 
            [&repeatCount]() {
                repeatCount++;
                HARMONY_INFO("Repeat #{}", repeatCount);
            }, 
            true);  // Auto-repeat enabled

        repeatTimer.start();
        for (int i = 0; i < 35; ++i) {
            repeatTimer.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (repeatCount >= 3) break;
        }
        HARMONY_INFO("Auto-repeat count: {}", repeatCount);
    }

    /// @brief Demonstrates FPSCounter class usage
    void demoFPSCounterClass() {
        HARMONY_INFO("=== FPS Counter Class Demo ===");

        Utilities::FPSCounter fpsCounter(10);  // Average over 10 frames
        
        HARMONY_INFO("Simulating game frames...");
        
        // Simulate 30 frames with varying frame times
        for (int i = 0; i < 30; ++i) {
            // Vary frame time to simulate real conditions
            int sleepTime = 16 + (i % 3) * 2;  // 16-20ms per frame
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            
            fpsCounter.update();
            
            if (i % 10 == 9) {  // Report every 10 frames
                HARMONY_INFO("Frame {}: FPS={:.1f}, FrameTime={:.2f}ms, Min={:.1f}, Max={:.1f}, Avg={:.1f}",
                    i + 1,
                    fpsCounter.getFPS(),
                    fpsCounter.getFrameTime() * 1000.0f,
                    fpsCounter.getMinFPS(),
                    fpsCounter.getMaxFPS(),
                    fpsCounter.getAverageFPS());
            }
        }
        
        HARMONY_INFO("Total frames counted: {}", fpsCounter.getTotalFrames());
    }

    /// @brief Run all time utility demos
    void runTimeUtilityDemos() {
        HARMONY_INFO("========================================");
        HARMONY_INFO("  Harmony Time Utilities Demo");
        HARMONY_INFO("========================================\n");

        demoTimeClass();
        HARMONY_INFO("");
        
        demoClockClass();
        HARMONY_INFO("");
        
        demoStopwatchClass();
        HARMONY_INFO("");
        
        demoTimerClass();
        HARMONY_INFO("");
        
        demoFPSCounterClass();
        
        HARMONY_INFO("\n========================================");
        HARMONY_INFO("  All demos completed!");
        HARMONY_INFO("========================================");
    }

} // namespace Harmony::Tests
