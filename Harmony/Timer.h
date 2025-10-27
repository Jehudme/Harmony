#pragma once

#include <memory>
#include <functional>

namespace Harmony::Components
{
	/// @brief Timer component - entity-specific timing and delays
	class Timer
	{
	public:
		Timer(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Timer();

		// Timer control
		void start();
		void stop();
		void pause();
		void resume();
		void reset();
		bool isRunning() const;
		bool isPaused() const;

		// Duration
		void setDuration(float seconds);
		float getDuration() const;
		float getElapsedTime() const;
		float getRemainingTime() const;
		bool hasElapsed() const;

		// Loop
		void setLoop(bool loop);
		bool isLooping() const;

		// Update
		void update(float deltaTime);

		// Callbacks
		using TimerCallback = std::function<void()>;
		void setOnComplete(TimerCallback callback);
		void setOnTick(TimerCallback callback); // Called every tick interval

		// Tick interval (for repeated callbacks)
		void setTickInterval(float seconds);
		float getTickInterval() const;

	private:
		struct TimerImpl;
		std::unique_ptr<TimerImpl> impl_;
	};
}
