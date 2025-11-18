#include "pch.h"
#include "Clock.h"

namespace Harmony
{
    Clock::Clock()
        : startTime_(std::chrono::steady_clock::now())
    {
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
        return Time::fromMicroseconds(elapsed.count());
    }

} // namespace Harmony::Utilities
