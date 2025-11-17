#pragma once

#include <deque>
#include "Time.h"
#include "Clock.h"

namespace Harmony::Utilities
{
    /// @brief Frame rate counter with statistics
    /// @details Tracks FPS, frame time, and provides statistical data
    /// useful for performance monitoring and debugging.
    class FPSCounter
    {
    public:
        /// @brief Constructor
        /// @param sampleSize Number of frames to average over (default 60)
        explicit FPSCounter(size_t sampleSize = 60);

        /// @brief Update the FPS counter (call once per frame)
        void update();

        /// @brief Get the current frames per second
        /// @return Current FPS
        float getFPS() const;

        /// @brief Get the average frame time
        /// @return Average frame time in seconds
        float getFrameTime() const;

        /// @brief Get the minimum FPS from recent samples
        /// @return Minimum FPS
        float getMinFPS() const;

        /// @brief Get the maximum FPS from recent samples
        /// @return Maximum FPS
        float getMaxFPS() const;

        /// @brief Get the average FPS from recent samples
        /// @return Average FPS
        float getAverageFPS() const;

        /// @brief Get the total number of frames counted
        /// @return Total frame count
        uint64_t getTotalFrames() const;

        /// @brief Reset all counters
        void reset();

        /// @brief Set the sample size for averaging
        /// @param sampleSize Number of frames to average over
        void setSampleSize(size_t sampleSize);

    private:
        void updateStatistics();

    private:
        Clock clock_;                    ///< Clock for measuring frame time
        std::deque<Time> frameTimes_;    ///< Recent frame times for averaging
        size_t sampleSize_;              ///< Number of frames to average over
        uint64_t totalFrames_;           ///< Total number of frames
        
        // Cached statistics
        float currentFPS_;               ///< Current calculated FPS
        float minFPS_;                   ///< Minimum FPS in recent samples
        float maxFPS_;                   ///< Maximum FPS in recent samples
        float averageFPS_;               ///< Average FPS in recent samples
    };

} // namespace Harmony::Utilities
