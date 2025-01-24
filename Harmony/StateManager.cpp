#include "pch.h"
#include "StateManager.h"
#include "Logger.h"

namespace harmony::core
{
    // Constructor
    StateManager::StateManager()
        : m_currentState(nullptr), m_states(std::map<std::string, std::shared_ptr<State>>())
    {
        LOG_TRACE(Logger::core, "[StateManager] StateManager created");
    }

    // Draw the current state
    void StateManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
    {
        if (m_currentState)
        {
            LOG_TRACE(Logger::core, "[StateManager] Drawing current state with ID: {}", m_currentState->uniqueId);
            m_currentState->draw(renderTarget, states);
        }
    }

    // Update the current state
    void StateManager::update(const sf::Time& time, EventPool& eventPool)
    {
        if (m_currentState)
        {
            LOG_TRACE(Logger::core, "[StateManager] Updating current state with ID: {}", m_currentState->uniqueId);
            m_currentState->update(time, eventPool);
        }
    }

    // Add a state to the manager
    void StateManager::addState(const std::shared_ptr<State> state)
    {
        if (state)
        {
            LOG_INFO(Logger::core, "[StateManager] Adding state with ID: {}", state->uniqueId);
            m_states[state->name] = state;
        }
    }

    // Remove a state from the manager
    void StateManager::removeState(const std::string& name)
    {
        if (m_states.find(name) != m_states.end())
        {
            LOG_INFO(Logger::core, "[StateManager] Removing state with Name: {}", name);
            if (m_currentState == m_states[name])
            {
                LOG_TRACE(Logger::core, "[StateManager] Current state matches removing state, setting current state to nullptr");
                m_currentState = nullptr;
            }
            m_states.erase(name);
        }
    }

    // Push a state to become the current state
    void StateManager::swichState(const std::string& name)
    {
        if (m_states.find(name) != m_states.end())
        {
            LOG_INFO(Logger::core, "[StateManager] Pushing state with Name: {}", name);
            if (m_currentState)
            {
                LOG_INFO(Logger::core, "[StateManager] Exiting current state with ID: {}", m_currentState->uniqueId);
                m_currentState->onExit();
            }

            m_currentState = m_states[name];
            LOG_TRACE(Logger::core, "[StateManager] Entering new state with ID: {}", m_currentState->uniqueId);
            m_currentState->onEnter();
        }
    }

    // Pop the current state
    void StateManager::popState()
    {
        if (m_currentState)
        {
            LOG_TRACE(Logger::core, "[StateManager] Popping current state with ID: {}", m_currentState->uniqueId);
            m_currentState->onExit();
            m_currentState = nullptr;
        }
    }

    // Switch to a different state
    void StateManager::switchState(const std::string& name)
    {
        LOG_TRACE(Logger::core, "[StateManager] Switching to state with name: {}", name);
        popState();
        swichState(name);
    }

    // Get the current state
    std::shared_ptr<State> StateManager::getCurrentState()
    {
        if (m_currentState) {
            LOG_TRACE(Logger::core, "[StateManager] Getting current state with ID: {}", m_currentState->uniqueId);
        }
        else {
            LOG_TRACE(Logger::core, "[StateManager] No current state");
        }
        return m_currentState;
    }
}
