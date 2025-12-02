#pragma once

#include <functional>
#include "Time.h"
#include "Clock.h"

namespace Harmony::Utilities
{
    /// @brief Countdown timer with callback support
    /// @details Can trigger callbacks when time expires, useful for game events,
    /// cooldowns, and timed actions.
    class Timer
    {
    public:
        /// @brief Callback function type (called when timer expires)
        using Callback = std::function<void()>;

        /// @brief Constructor - creates a timer with specified duration
        /// @param duration Time until the timer expires
        /// @param callback Optional callback to invoke when timer expires
        /// @param autoRepeat Whether to automatically restart after expiring
        Timer(Time duration, Callback callback = nullptr, bool autoRepeat = false);

        /// @brief Default constructor - creates an inactive timer
        Timer();

        /// @brief Start or restart the timer
        void start();

        /// @brief Pause the timer
        void pause();

        /// @brief Resume the timer from paused state
        void resume();

        /// @brief Stop and reset the timer
        void reset();

        /// @brief Update the timer (should be called each frame)
        /// @details Checks if timer has expired and invokes callback if set
        void handleResources();

        /// @brief Check if the timer has expired
        /// @return true if expired, false otherwise
        bool hasExpired() const;

        /// @brief Check if the timer is currently running
        /// @return true if running, false otherwise
        bool isRunning() const;

        /// @brief Check if the timer is paused
        /// @return true if paused, false otherwise
        bool isPaused() const;

        /// @brief Get remaining time until expiration
        /// @return Time remaining
        Time getRemainingTime() const;

        /// @brief Get elapsed time since timer started
        /// @return Time elapsed
        Time getElapsedTime() const;

        /// @brief Get the timer's total duration
        /// @return Total duration
        Time getDuration() const;

        /// @brief Set a new duration for the timer
        /// @param duration New duration
        void setDuration(Time duration);

        /// @brief Set the callback function
        /// @param callback Function to call when timer expires
        void setCallback(Callback callback);

        /// @brief Set whether the timer auto-repeats
        /// @param autoRepeat true to auto-repeat, false otherwise
        void setAutoRepeat(bool autoRepeat);

        /// @brief Get the progress as a ratio (0.0 to 1.0)
        /// @return Progress from 0.0 (just started) to 1.0 (expired)
        float getProgress() const;

    private:
        Clock clock_;              ///< Internal clock for time measurement
        Time duration_;            ///< Total duration of the timer
        Time accumulatedTime_;     ///< Accumulated time when paused
        Callback callback_;        ///< Function to call when timer expires
        bool running_;             ///< Whether timer is currently running
        bool paused_;              ///< Whether timer is paused
        bool expired_;             ///< Whether timer has expired
        bool autoRepeat_;          ///< Whether to automatically restart
    };

} // namespace Harmony::Utilities
