#include "Engine.h"
#include "PluginsRegistry.h"
#include "Harmony/Logger.h"
#include "Harmony/Assert.h"

#include <condition_variable>
#include <shared_mutex>

namespace Harmony 
{
    // ========================================================
    // Engine Internal Structure
    // ========================================================

    struct Engine::Internal 
    {
        Internal(const Properties& properties);

        State state;
        Clock clock;
        Context context;
        PluginsRegistry pluginsRegistry;

        mutable std::shared_mutex stateMutex;
        std::mutex pausingMutex;
        std::condition_variable pausingCondition;
    };

    // ========================================================
    // Engine Constructor and Destructor
    // ========================================================

    Engine::Engine(const Properties& properties) :
        m_internal(std::make_unique<Internal>(properties))
    {
        HARMONY_ASSERT_NOT_NULL(m_internal.get(), "Engine internal structure is null");
        HARMONY_INFO("Engine created successfully");
    }

    Engine::~Engine()
    {
        if (m_internal)
        {
            std::unique_lock lock(m_internal->stateMutex);
            m_internal->state = State::Uninitialized;
            HARMONY_INFO("Engine destroyed");
        }
    }

    // ========================================================
    // Engine Public Methods
    // ========================================================

    void Engine::Run()
    {
        if (!m_internal)
        {
            HARMONY_ERROR("Engine::Run - Internal structure is null");
            return;
        }

        {
            std::shared_lock lock(m_internal->stateMutex);
            if (m_internal->state <= State::Initialized) 
            {
                HARMONY_ERROR("Engine::Run - Engine not properly initialized");
                return;
            }
        }

        HARMONY_INFO("Engine::Run - Starting main loop");

        while (true) 
        {
            WaitIfPaused();
            HandleEvents();
            HandleUpdate();
            HandleRender();
        }
    }

    void Engine::Pause()
    {
        if (!m_internal)
        {
            HARMONY_ERROR("Engine::Pause - Internal structure is null");
            return;
        }

        std::unique_lock lock(m_internal->stateMutex);
        m_internal->state = State::Paused;
        HARMONY_INFO("Engine paused");
    }

    void Engine::Resume()
    {
        if (!m_internal)
        {
            HARMONY_ERROR("Engine::Resume - Internal structure is null");
            return;
        }

        {
            std::unique_lock lock(m_internal->stateMutex);
            m_internal->state = State::Running;
        }
        
        m_internal->pausingCondition.notify_all();
        HARMONY_INFO("Engine resumed");
    }

    Engine::State Engine::GetState() const
    {
        if (!m_internal)
        {
            HARMONY_ERROR("Engine::GetState - Internal structure is null");
            return State::Uninitialized;
        }

        std::shared_lock lock(m_internal->stateMutex);
        return m_internal->state;
    }

    Context& Engine::GetContext()
    {
        HARMONY_ASSERT_NOT_NULL(m_internal.get(), "Engine internal structure is null");
        return m_internal->context;
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
        if (!m_internal)
        {
            return;
        }

        std::unique_lock lock(m_internal->pausingMutex);
        m_internal->pausingCondition.wait(lock, [this]() 
        {
            std::shared_lock stateLock(m_internal->stateMutex);
            return m_internal->state != State::Paused;
        });
    }

    // ========================================================
    // Engine Internal Constructor
    // ========================================================

    Engine::Internal::Internal(const Properties& properties) :
        state(State::Initialized),
        pluginsRegistry(context, properties["Plugins"])
    {
        HARMONY_INFO("Engine internal structure initialized");
    }

} // namespace Harmony
