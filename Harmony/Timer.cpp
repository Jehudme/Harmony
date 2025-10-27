#include "pch.h"
#include "Timer.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"

HARMONY_REGISTER_COMPONENT(Harmony::Components::Timer, Timer)

namespace Harmony::Components
{
	struct TimerImpl {
		float duration = 1.0f;
		float elapsedTime = 0.0f;
		float tickInterval = 0.0f;
		float timeSinceLastTick = 0.0f;
		bool running = false;
		bool paused = false;
		bool loop = false;
		Timer::TimerCallback onComplete;
		Timer::TimerCallback onTick;
	};

	Timer::Timer(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<TimerImpl>())
	{
		try {
			impl_->duration = configuration.get<float>({ "duration" }).value_or(1.0f);
			impl_->loop = configuration.get<bool>({ "loop" }).value_or(false);
			impl_->tickInterval = configuration.get<float>({ "tick_interval" }).value_or(0.0f);

			bool autoStart = configuration.get<bool>({ "auto_start" }).value_or(false);
			if (autoStart) {
				impl_->running = true;
			}

			HARMONY_DEBUG("Timer component initialized with duration {}", impl_->duration);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Timer component: {}", e.what());
			throw;
		}
	}

	Timer::~Timer() = default;

	void Timer::start() {
		impl_->running = true;
		impl_->paused = false;
		impl_->elapsedTime = 0.0f;
		impl_->timeSinceLastTick = 0.0f;
	}

	void Timer::stop() {
		impl_->running = false;
		impl_->paused = false;
		impl_->elapsedTime = 0.0f;
		impl_->timeSinceLastTick = 0.0f;
	}

	void Timer::pause() {
		impl_->paused = true;
	}

	void Timer::resume() {
		impl_->paused = false;
	}

	void Timer::reset() {
		impl_->elapsedTime = 0.0f;
		impl_->timeSinceLastTick = 0.0f;
	}

	bool Timer::isRunning() const {
		return impl_->running && !impl_->paused;
	}

	bool Timer::isPaused() const {
		return impl_->paused;
	}

	void Timer::setDuration(float seconds) {
		impl_->duration = seconds;
	}

	float Timer::getDuration() const {
		return impl_->duration;
	}

	float Timer::getElapsedTime() const {
		return impl_->elapsedTime;
	}

	float Timer::getRemainingTime() const {
		return std::max(0.0f, impl_->duration - impl_->elapsedTime);
	}

	bool Timer::hasElapsed() const {
		return impl_->elapsedTime >= impl_->duration;
	}

	void Timer::setLoop(bool loop) {
		impl_->loop = loop;
	}

	bool Timer::isLooping() const {
		return impl_->loop;
	}

	void Timer::update(float deltaTime) {
		if (!impl_->running || impl_->paused) return;

		impl_->elapsedTime += deltaTime;
		impl_->timeSinceLastTick += deltaTime;

		// Handle tick callbacks
		if (impl_->tickInterval > 0.0f && impl_->timeSinceLastTick >= impl_->tickInterval) {
			impl_->timeSinceLastTick = 0.0f;
			if (impl_->onTick) {
				try {
					impl_->onTick();
				}
				catch (const std::exception& e) {
					HARMONY_ERROR("Timer tick callback failed: {}", e.what());
				}
			}
		}

		// Handle completion
		if (impl_->elapsedTime >= impl_->duration) {
			if (impl_->onComplete) {
				try {
					impl_->onComplete();
				}
				catch (const std::exception& e) {
					HARMONY_ERROR("Timer completion callback failed: {}", e.what());
				}
			}

			if (impl_->loop) {
				impl_->elapsedTime = 0.0f;
			}
			else {
				impl_->running = false;
			}
		}
	}

	void Timer::setOnComplete(TimerCallback callback) {
		impl_->onComplete = callback;
	}

	void Timer::setOnTick(TimerCallback callback) {
		impl_->onTick = callback;
	}

	void Timer::setTickInterval(float seconds) {
		impl_->tickInterval = seconds;
	}

	float Timer::getTickInterval() const {
		return impl_->tickInterval;
	}
}
