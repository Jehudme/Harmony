#include "pch.h"
#include "Clock.h"
#include "Logger.h"

namespace Harmony
{
    Clock::Clock()
        : m_startTime(std::chrono::steady_clock::now())
    {
        HARMONY_TRACE("Clock created");
    }

	Clock::~Clock() = default;

    Time Clock::getElapsedTime() const
    {
		std::shared_lock<std::shared_mutex> lock(m_mutex);
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - m_startTime);
        return Time::fromMicroseconds(elapsed.count());
    }

    Time Clock::restart()
    {
		std::lock_guard<std::shared_mutex> lock(m_mutex);
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - m_startTime);
        m_startTime = now;
        
        HARMONY_TRACE("Clock restarted, elapsed time: {:.3f} seconds", 
                     Time::fromMicroseconds(elapsed.count()).asSeconds());
        
        return Time::fromMicroseconds(elapsed.count());
    }

} // namespace Harmony
