#include "pch.h"
#include "Stopwatch.h"

namespace Harmony::Utilities
{
    Stopwatch::Stopwatch()
        : accumulatedTime_(Time::Zero)
        , lastLapTime_(Time::Zero)
        , running_(false)
    {
    }

    void Stopwatch::start()
    {
        if (!running_)
        {
            clock_.restart();
            running_ = true;
        }
    }

    void Stopwatch::pause()
    {
        if (running_)
        {
            accumulatedTime_ += clock_.getElapsedTime();
            running_ = false;
        }
    }

    void Stopwatch::reset()
    {
        accumulatedTime_ = Time::Zero;
        lastLapTime_ = Time::Zero;
        laps_.clear();
        running_ = false;
    }

    bool Stopwatch::isRunning() const
    {
        return running_;
    }

    Time Stopwatch::getElapsedTime() const
    {
        if (running_)
        {
            return accumulatedTime_ + clock_.getElapsedTime();
        }
        return accumulatedTime_;
    }

    Time Stopwatch::lap()
    {
        Time currentTime = getElapsedTime();
        Time lapTime = currentTime - lastLapTime_;
        lastLapTime_ = currentTime;
        laps_.push_back(lapTime);
        return lapTime;
    }

    const std::vector<Time>& Stopwatch::getLaps() const
    {
        return laps_;
    }

    size_t Stopwatch::getLapCount() const
    {
        return laps_.size();
    }

    void Stopwatch::clearLaps()
    {
        laps_.clear();
        lastLapTime_ = Time::Zero;
    }

} // namespace Harmony::Utilities
