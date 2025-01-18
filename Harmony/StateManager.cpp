#include "pch.h"
#include "StateManager.h"

namespace Harmony::Core
{
    // Constructor
    StateManager::StateManager()
        : m_currentState(nullptr) {
    }

    // Draw the current state
    void StateManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (m_currentState) {
            m_currentState->draw(renderTarget, states);
        }
    }

    // Update the current state
    void StateManager::update(const sf::Time& time, EventPool& eventPool) {
        if (m_currentState) {
            m_currentState->update(time, eventPool);
        }
    }

    // Add a state to the manager
    void StateManager::addState(const std::string& name, const std::shared_ptr<State> state) {
        if (state) {
            m_states[name] = state;
        }
    }

    // Remove a state from the manager
    void StateManager::removeState(const std::string& name) {
        if (m_states.find(name) != m_states.end()) {
            if (m_currentState == m_states[name]) {
                m_currentState = nullptr;
            }
            m_states.erase(name);
        }
    }

    // Push a state to become the current state
    void StateManager::pushState(const std::string& name) {
        if (m_states.find(name) != m_states.end()) {
            if (m_currentState) {
                m_currentState->onExit();
            }

            m_currentState = m_states[name];
            m_currentState->onEnter();
        }
    }

    // Pop the current state
    void StateManager::popState() {
        if (m_currentState) {
            m_currentState->onExit();
            m_currentState = nullptr;
        }
    }

    // Switch to a different state
    void StateManager::switchState(const std::string& name) {
        popState();
        pushState(name);
    }

    // Get the current state
    std::shared_ptr<State> StateManager::getCurrentState() {
        return m_currentState;
    }
}

