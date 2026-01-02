#include "Engine.h"
#include "Harmony/Logger.h"
#include "Harmony/Assert.h"

#include <condition_variable>
#include <shared_mutex>

namespace Harmony 
{
    // ========================================================
    // Engine Constructor and Destructor
    // ========================================================

    Engine::Engine() :
        m_state(State::Uninitialized) {
    }

    Engine::~Engine()
    {
		if (m_state != State::Uninitialized)
            Shutdown();
        
    }

    // ========================================================
    // Engine Public Methods
    // ========================================================

    void Engine::Run()
    {
        if (std::shared_lock lock(m_stateMutex); m_state != State::Initialized)
            return HARMONY_ERROR("Engine::Run - Engine not properly initialized");

        HARMONY_INFO("Engine::Run - Starting main loop");

        while (m_state != State::Stopped) 
        {
            WaitIfPaused();
            HandleEvents();
            HandleUpdate();
            HandleRender();
        }
    }

    void Engine::Initialize(const Properties& properties)
    {
        if (m_state != State::Uninitialized)
		    return HARMONY_ERROR("Engine::Initialize - Engine already initialized");
		
        m_state = State::Initialized;
    }

    void Engine::Shutdown()
    {
    }

    void Engine::Pause()
    {
        std::unique_lock lock(m_stateMutex);
        m_state = State::Paused;
        HARMONY_INFO("Engine paused");
    }

    void Engine::Resume()
    {
        {
            std::unique_lock lock(m_stateMutex);
            m_state = State::Running;
        }
        
        m_pausingCondition.notify_all();
        HARMONY_INFO("Engine resumed");
    }

    Engine::State Engine::GetState() const
    {
        std::shared_lock lock(m_stateMutex);
        return m_state;
    }

    Context& Engine::GetContext()
    {
        return m_context;
    }

    // ========================================================
    // Engine Private Methods
    // ========================================================

    void Engine::HandleUpdate()
    {
        HARMONY_TRACE("Engine::HandleUpdate - Processing update");
    }

    void Engine::HandleRender()
    {
        HARMONY_TRACE("Engine::HandleRender - Processing render");
    }

    void Engine::HandleEvents()
    {
        HARMONY_TRACE("Engine::HandleEvents - Processing events");
    }

    void Engine::WaitIfPaused()
    {

        std::unique_lock lock(m_pausingMutex);
        m_pausingCondition.wait(lock, [this]() 
        {
            std::shared_lock stateLock(m_stateMutex);
            return m_state != State::Paused;
        });
    }

} // namespace Harmony
