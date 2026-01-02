#include "Engine.h"
#include "Harmony/Assert.h"
#include "Harmony/Logger.h"

namespace Harmony
{
Engine::Engine() : m_state(State::Uninitialized) {}

Engine::~Engine() = default;

void Engine::Run()
{
    switch (m_state)
    {
    case State::Uninitialized:
        return HARMONY_ERROR("Engine::Run - Engine not initialized");
    case State::Running:
        return HARMONY_ERROR("Engine::Run - Engine already running");
    case State::Shutdown:
        return HARMONY_ERROR("Engine::Run - Engine is shutdown");
    case State::Initialized:
        m_state = State::Running;
        break;
    default:
        return HARMONY_ERROR("Engine::Run - Unknown engine state");
        break;
    }

    HARMONY_INFO("Engine::Run - Starting main loop");
    while (m_state == State::Running) {
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
    switch (m_state)
    {
    case State::Uninitialized:
        return HARMONY_ERROR("Engine::Shutdown - Engine not initialized");
    case State::Shutdown:
        return HARMONY_ERROR("Engine::Shutdown - Engine already shutdown");
    case State::Initialized:
    case State::Running:
        HARMONY_INFO("Engine::Shutdown - Shutting down engine");
        break;
    default:
        return HARMONY_ERROR("Engine::Shutdown - Unknown engine state");
        break;
    }

    m_state = State::Shutdown;
}

Engine::State Engine::GetState() const
{
    return m_state;
}

ISystemsRegistry& Engine::GetSystemsRegistry()
{
    return m_systemsRegistry;
}

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
} // namespace Harmony
