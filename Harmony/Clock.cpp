#include "pch.h"
#include "Clock.h"
#include "Logger.h"

namespace Harmony
{
    Clock::Clock()
        : startTime_(std::chrono::steady_clock::now())
    {
        HARMONY_TRACE("Clock created");
    }

    Time Clock::getElapsedTime() const
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime_);
        return Time::fromMicroseconds(elapsed.count());
    }

    Time Clock::restart()
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime_);
        startTime_ = now;
        
        HARMONY_TRACE("Clock restarted, elapsed time: {:.3f} seconds", 
                     Time::fromMicroseconds(elapsed.count()).asSeconds());
        
        return Time::fromMicroseconds(elapsed.count());
    }

} // namespace Harmony
