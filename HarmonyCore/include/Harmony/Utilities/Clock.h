#pragma once

#include "Harmony/Utilities/Time.h"

#include <chrono>
#include <shared_mutex>

namespace Harmony
{
    // ========================================================
    // Clock for Measuring Elapsed Time
    // ========================================================
    
    /// @brief clock for measuring elapsed time
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
        Time GetElapsedTime() const;

        /// @brief Restart the clock and return the elapsed time since last restart
        /// @return Time that was elapsed before restart
        Time Restart();

    private:
        mutable std::shared_mutex m_mutex;
        std::chrono::steady_clock::time_point m_startTime;
    };

} // namespace Harmony