#pragma once

#include <chrono>
#include "Time.h"
#include <shared_mutex>

namespace Harmony
{
    /// @brief High-precision clock for measuring elapsed time
    /// @details Similar to sf::Clock but as a native Harmony utility.
    /// Uses std::chrono for cross-platform time measurement.
    class Clock
    {
    public:
        /// @brief Constructor - starts the clock automatically
        Clock();
		~Clock();

        /// @brief Get the elapsed time since the clock started or last restart
        /// @return Time elapsed
        Time getElapsedTime() const;

        /// @brief Restart the clock and return the elapsed time since last restart
        /// @return Time that was elapsed before restart
        Time restart();

    private:
		mutable std::shared_mutex m_mutex;                      ///< Mutex for thread-safe access
        std::chrono::steady_clock::time_point m_startTime; ///< Clock start time point
    };

} // namespace Harmony::Utilities
