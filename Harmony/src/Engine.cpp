#include "Engine.h"
#include "PluginsRegistry.h"

#include <condition_variable>

namespace Harmony {
	struct Engine::Internal {
		Internal(const Properties& properties);

		State state;
		Clock clock;
		Context context;
		PluginsRegistry pluginsRegistry;

		std::mutex pausingMutex;
		std::condition_variable pausingCondition;
	};

	Engine::Engine(const Properties& properties) :
		m_internal(std::make_unique<Internal>(properties))
	{
	}

	Engine::~Engine()
	{
		m_internal->state = State::Uninitialized;
	}

	void Engine::Run()
	{
		if (m_internal->state <= State::Initialized) {
			// log error: engine not initialized
			return;
		}

		while (true) {
			handleEvents();
			handleUpdate();
			handleRender();
		}
	}

	void Engine::Pause()
	{
		std::lock_guard lock(m_internal->pausingMutex);
		m_internal->state = State::Paused;
	}

	void Engine::Resume()
	{
		std::lock_guard lock(m_internal->pausingMutex);
		m_internal->state = State::Running;
		
		m_internal->pausingCondition.notify_all();
	}

	Engine::State Engine::GetState() const
	{
		return m_internal->state;
	}

	Context& Engine::GetContext()
	{
		return m_internal->context;
	}

	inline void Engine::handleUpdate()
	{
		//log
	}

	inline void Engine::handleRender()
	{
		//log
	}

	inline void Engine::handleEvents()
	{
		//log
	}

	inline void Engine::waitIfPaused()
	{
		std::unique_lock lock(m_internal->pausingMutex);
		m_internal->pausingCondition.wait(lock, [this]() {
			return m_internal->state != State::Paused;
			});
	}

	Engine::Internal::Internal(const Properties& properties) :
		state(State::Initialized),
		pluginsRegistry(context, properties["Plugins"])
	{
	}
}
