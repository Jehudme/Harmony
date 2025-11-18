#include "pch.h"
#include "Stopwatch.h"

namespace Harmony::Utilities
{
    Stopwatch::Stopwatch()
        : accumulatedTime_(Time::Zero)
        , lastLapTime_(Time::Zero)
        , running_(false)
    {
        HARMONY_TRACE("Stopwatch created");
    }

    void Stopwatch::start()
    {
        if (!running_)
        {
            clock_.restart();
            running_ = true;
            HARMONY_DEBUG("Stopwatch started");
        }
        else
        {
            HARMONY_ASSERT_WARN(false, "Attempting to start an already running stopwatch");
        }
    }

    void Stopwatch::pause()
    {
        if (running_)
        {
            accumulatedTime_ += clock_.getElapsedTime();
            running_ = false;
            HARMONY_DEBUG("Stopwatch paused at {:.3f} seconds", accumulatedTime_.asSeconds());
        }
    }

    void Stopwatch::reset()
    {
        accumulatedTime_ = Time::Zero;
        lastLapTime_ = Time::Zero;
        laps_.clear();
        running_ = false;
        HARMONY_DEBUG("Stopwatch reset");
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
        if (!running_)
        {
            HARMONY_ASSERT_WARN(running_, "Attempting to record lap on a stopped stopwatch");
        }
        
        Time currentTime = getElapsedTime();
        Time lapTime = currentTime - lastLapTime_;
        lastLapTime_ = currentTime;
        laps_.push_back(lapTime);
        
        HARMONY_DEBUG("Lap {} recorded: {:.3f} seconds", laps_.size(), lapTime.asSeconds());
        
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
