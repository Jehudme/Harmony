#include "Harmony/Engine.h"
#include "Harmony/Assert.h"
#include "Harmony/IRenderer.h"
#include "Harmony/TaskDispatcher.h"
#include "Harmony/EngineContext.h"

#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

namespace Harmony 
{


	struct Engine::Internal 
	{
		std::unique_ptr<IRenderer> renderer;
		std::unique_ptr<TaskDispatcher> taskDispatcher;

		std::condition_variable pauseCondition;
		std::mutex pauseMutex;

		std::atomic<bool> running{ false };
		std::atomic<bool> paused{ false };

		Properties properties;
	};

	Engine::Engine(Properties properties) :
		m_internal(std::make_unique<Internal>()) 
	{
		m_internal->properties = properties;
		m_internal->taskDispatcher = std::make_unique<TaskDispatcher>(*this);
	}

	Engine::~Engine() = default;

	void Engine::start() 
	{
		HARMONY_ASSERT(!m_internal->running, "Engine is already running");
		
		m_internal->running.store(true);
		m_internal->taskDispatcher->startPool();

		while (m_internal->running) {
			if (m_internal->paused) {
				waitWhilePaused();
			}

			event();
			update();
			render();
		}

		m_internal->taskDispatcher->stopPool();
	}

	EngineContext Harmony::Engine::context()
	{
		return EngineContext(*this, *m_internal->taskDispatcher);
	}

	void Engine::stop()
	{
		m_internal->running.store(false);
		resume(); 
	}

	void Engine::pause()
	{
		m_internal->paused.store(true);
	}

	void Engine::resume()
	{
		m_internal->paused.store(false);
		m_internal->pauseCondition.notify_all();
	}

	void Engine::event()
	{
	}

	void Engine::update()
	{
	}

	void Engine::render()
	{
	}

	bool Engine::running() const
	{
		return m_internal->paused;
	}

	bool Engine::paused() const 
	{
		return m_internal->paused;
	}

	void Engine::waitWhilePaused()
	{
		std::unique_lock lock(m_internal->pauseMutex);
		m_internal->pauseCondition.wait(lock, [this]() {
			return !m_internal->paused.load();
		});
	}
}