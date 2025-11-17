#pragma once

#include <vector>
#include "Time.h"
#include "Clock.h"

namespace Harmony::Utilities
{
    /// @brief Advanced stopwatch with pause, resume, and lap functionality
    /// @details Provides more control than Clock, allowing pausing/resuming
    /// and recording lap times for performance measurement.
    class Stopwatch
    {
    public:
        /// @brief Constructor - creates a stopped stopwatch
        Stopwatch();

        /// @brief Start or resume the stopwatch
        void start();

        /// @brief Pause the stopwatch (can be resumed later)
        void pause();

        /// @brief Stop and reset the stopwatch
        void reset();

        /// @brief Check if the stopwatch is currently running
        /// @return true if running, false otherwise
        bool isRunning() const;

        /// @brief Get the total elapsed time (including paused periods)
        /// @return Total elapsed time
        Time getElapsedTime() const;

        /// @brief Record a lap time without stopping
        /// @return Time for the current lap
        Time lap();

        /// @brief Get all recorded lap times
        /// @return Vector of all lap times
        const std::vector<Time>& getLaps() const;

        /// @brief Get the number of laps recorded
        /// @return Number of laps
        size_t getLapCount() const;

        /// @brief Clear all recorded laps
        void clearLaps();

    private:
        Clock clock_;                  ///< Internal clock for time measurement
        Time accumulatedTime_;         ///< Accumulated time from previous runs
        Time lastLapTime_;             ///< Time of the last lap
        std::vector<Time> laps_;       ///< Recorded lap times
        bool running_;                 ///< Whether the stopwatch is currently running
    };

} // namespace Harmony::Utilities
