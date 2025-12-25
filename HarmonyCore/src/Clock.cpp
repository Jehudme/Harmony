#include "Harmony/Utilities/Clock.h"
#include "Harmony/Logger.h"
#include "Harmony/Assert.h"

namespace Harmony
{
    // ========================================================
    // Clock Implementation
    // ========================================================

    Clock::Clock()
        : m_startTime(std::chrono::steady_clock::now())
    {
        HARMONY_TRACE("Clock created");
    }

    Clock::~Clock() = default;

    Time Clock::GetElapsedTime() const
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_startTime);
        
        return Time::FromMicroseconds(elapsedDuration.count());
    }

    Time Clock::Restart()
    {
        std::lock_guard<std::shared_mutex> lock(m_mutex);
        
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_startTime);
        m_startTime = currentTime;

        Time elapsedTime = Time::FromMicroseconds(elapsedDuration.count());
        HARMONY_TRACE("Clock restarted, elapsed time: {:.3f} seconds", elapsedTime.AsSeconds());

        return elapsedTime;
    }

} // namespace Harmony