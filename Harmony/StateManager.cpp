#include "pch.h"
#include "StateManager.h"
#include <memory>
#include <string>
#include "Event.h"
#include "State.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

namespace harmony::core {

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
    void StateManager::update(const sf::Time& time, EventQueue& eventQueue) {
        if (m_currentState) {
            m_currentState->update(time, eventQueue);
        }
    }

    // Add a state to the manager
    void StateManager::addState(const std::shared_ptr<State> state) {
        if (state) {
            m_states[state->name] = state;
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

    void StateManager::swichState(const std::string& name)
    {
        popState();
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

    // Get the current state
    std::shared_ptr<State> StateManager::getCurrentState() const {
        return m_currentState;
    }
}
