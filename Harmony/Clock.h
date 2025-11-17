#pragma once

#include <chrono>
#include "Time.h"

namespace Harmony::Utilities
{
    /// @brief High-precision clock for measuring elapsed time
    /// @details Similar to sf::Clock but as a native Harmony utility.
    /// Uses std::chrono for cross-platform time measurement.
    class Clock
    {
    public:
        /// @brief Constructor - starts the clock automatically
        Clock();

        /// @brief Get the elapsed time since the clock started or last restart
        /// @return Time elapsed
        Time getElapsedTime() const;

        /// @brief Restart the clock and return the elapsed time since last restart
        /// @return Time that was elapsed before restart
        Time restart();

    private:
        std::chrono::steady_clock::time_point startTime_; ///< Clock start time point
    };

} // namespace Harmony::Utilities
