#include "pch.h"
#include "Timer.h"
#include "Assert.h"

namespace Harmony::Utilities
{
    Timer::Timer(Time duration, Callback callback, bool autoRepeat)
        : duration_(duration)
        , accumulatedTime_(Time::Zero)
        , callback_(callback)
        , running_(false)
        , paused_(false)
        , expired_(false)
        , autoRepeat_(autoRepeat)
    {
        HARMONY_ASSERT(duration >= Time::Zero, "Timer duration must be non-negative");
    }

    Timer::Timer()
        : duration_(Time::Zero)
        , accumulatedTime_(Time::Zero)
        , callback_(nullptr)
        , running_(false)
        , paused_(false)
        , expired_(false)
        , autoRepeat_(false)
    {
    }

    void Timer::start()
    {
        clock_.restart();
        accumulatedTime_ = Time::Zero;
        running_ = true;
        paused_ = false;
        expired_ = false;
    }

    void Timer::pause()
    {
        if (running_ && !paused_)
        {
            accumulatedTime_ += clock_.getElapsedTime();
            paused_ = true;
        }
    }

    void Timer::resume()
    {
        if (running_ && paused_)
        {
            clock_.restart();
            paused_ = false;
        }
    }

    void Timer::reset()
    {
        accumulatedTime_ = Time::Zero;
        running_ = false;
        paused_ = false;
        expired_ = false;
    }

    void Timer::handleResources()
    {
        if (!running_ || paused_ || expired_)
            return;

        Time elapsed = getElapsedTime();
        
        if (elapsed >= duration_)
        {
            expired_ = true;
            
            // Invoke callback if set
            if (callback_)
            {
                callback_();
            }

            // Auto-repeat if enabled
            if (autoRepeat_)
            {
                start();
            }
            else
            {
                running_ = false;
            }
        }
    }

    bool Timer::hasExpired() const
    {
        return expired_;
    }

    bool Timer::isRunning() const
    {
        return running_;
    }

    bool Timer::isPaused() const
    {
        return paused_;
    }

    Time Timer::getRemainingTime() const
    {
        if (!running_ || expired_)
            return Time::Zero;

        Time elapsed = getElapsedTime();
        if (elapsed >= duration_)
            return Time::Zero;

        return duration_ - elapsed;
    }

    Time Timer::getElapsedTime() const
    {
        if (!running_)
            return accumulatedTime_;

        if (paused_)
            return accumulatedTime_;

        return accumulatedTime_ + clock_.getElapsedTime();
    }

    Time Timer::getDuration() const
    {
        return duration_;
    }

    void Timer::setDuration(Time duration)
    {
        HARMONY_ASSERT(duration >= Time::Zero, "Timer duration must be non-negative");
        duration_ = duration;
    }

    void Timer::setCallback(Callback callback)
    {
        callback_ = callback;
    }

    void Timer::setAutoRepeat(bool autoRepeat)
    {
        autoRepeat_ = autoRepeat;
    }

    float Timer::getProgress() const
    {
        if (duration_ == Time::Zero)
        {
            HARMONY_ASSERT_WARN(duration_ != Time::Zero, "Timer duration is zero, progress is undefined");
            return 0.0f;
        }

        Time elapsed = getElapsedTime();
        if (elapsed >= duration_)
            return 1.0f;

        return elapsed / duration_;
    }

} // namespace Harmony::Utilities
