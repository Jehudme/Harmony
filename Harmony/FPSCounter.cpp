#include "pch.h"
#include "FPSCounter.h"
#include <algorithm>
#include <numeric>

namespace Harmony::Utilities
{
    FPSCounter::FPSCounter(size_t sampleSize)
        : sampleSize_(sampleSize)
        , totalFrames_(0)
        , currentFPS_(0.0f)
        , minFPS_(0.0f)
        , maxFPS_(0.0f)
        , averageFPS_(0.0f)
    {
    }

    void FPSCounter::update()
    {
        Time frameTime = clock_.restart();
        frameTimes_.push_back(frameTime);
        totalFrames_++;

        // Keep only the most recent samples
        if (frameTimes_.size() > sampleSize_)
        {
            frameTimes_.pop_front();
        }

        updateStatistics();
    }

    float FPSCounter::getFPS() const
    {
        return currentFPS_;
    }

    float FPSCounter::getFrameTime() const
    {
        if (frameTimes_.empty())
            return 0.0f;

        return frameTimes_.back().asSeconds();
    }

    float FPSCounter::getMinFPS() const
    {
        return minFPS_;
    }

    float FPSCounter::getMaxFPS() const
    {
        return maxFPS_;
    }

    float FPSCounter::getAverageFPS() const
    {
        return averageFPS_;
    }

    uint64_t FPSCounter::getTotalFrames() const
    {
        return totalFrames_;
    }

    void FPSCounter::reset()
    {
        frameTimes_.clear();
        totalFrames_ = 0;
        currentFPS_ = 0.0f;
        minFPS_ = 0.0f;
        maxFPS_ = 0.0f;
        averageFPS_ = 0.0f;
        clock_.restart();
    }

    void FPSCounter::setSampleSize(size_t sampleSize)
    {
        sampleSize_ = sampleSize;
        
        // Trim if necessary
        while (frameTimes_.size() > sampleSize_)
        {
            frameTimes_.pop_front();
        }

        updateStatistics();
    }

    void FPSCounter::updateStatistics()
    {
        if (frameTimes_.empty())
        {
            currentFPS_ = 0.0f;
            minFPS_ = 0.0f;
            maxFPS_ = 0.0f;
            averageFPS_ = 0.0f;
            return;
        }

        // Calculate current FPS from the most recent frame
        Time lastFrameTime = frameTimes_.back();
        if (lastFrameTime.asSeconds() > 0.0f)
        {
            currentFPS_ = 1.0f / lastFrameTime.asSeconds();
        }
        else
        {
            currentFPS_ = 0.0f;
        }

        // Calculate average, min, and max from all samples
        float totalSeconds = 0.0f;
        float minSeconds = frameTimes_.front().asSeconds();
        float maxSeconds = minSeconds;

        for (const auto& frameTime : frameTimes_)
        {
            float seconds = frameTime.asSeconds();
            totalSeconds += seconds;
            
            if (seconds > 0.0f)
            {
                minSeconds = std::min(minSeconds, seconds);
                maxSeconds = std::max(maxSeconds, seconds);
            }
        }

        // Average FPS
        float averageSeconds = totalSeconds / static_cast<float>(frameTimes_.size());
        if (averageSeconds > 0.0f)
        {
            averageFPS_ = 1.0f / averageSeconds;
        }
        else
        {
            averageFPS_ = 0.0f;
        }

        // Min and Max FPS (inverse of max and min frame times)
        if (maxSeconds > 0.0f)
        {
            minFPS_ = 1.0f / maxSeconds;
        }
        else
        {
            minFPS_ = 0.0f;
        }

        if (minSeconds > 0.0f)
        {
            maxFPS_ = 1.0f / minSeconds;
        }
        else
        {
            maxFPS_ = 0.0f;
        }
    }

} // namespace Harmony::Utilities
