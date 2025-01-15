#include "pch.h"
#include "StateManager.h"

namespace Harmony::Core {

    StateManager::StateManager() = default;

    StateManager::~StateManager() {
        // Destructor doesn't need to do anything special for this class
    }

    void StateManager::pushState(std::shared_ptr<State> state) {
        // If there is an existing state, exit it first
        if (!m_stateStack.empty()) {
            m_stateStack.top()->onExit();
        }

        // Push the new state onto the stack and enter it
        m_stateStack.push(state);
        state->onEnter();
    }

    void StateManager::popState() {
        if (!m_stateStack.empty()) {
            // Exit the current top state and pop it
            m_stateStack.top()->onExit();
            m_stateStack.pop();

            // Enter the previous state (if it exists)
            if (!m_stateStack.empty()) {
                m_stateStack.top()->onEnter();
            }
        }
    }

    std::shared_ptr<State> StateManager::getCurrentState() const {
        return m_stateStack.empty() ? nullptr : m_stateStack.top();
    }

    void StateManager::update(const sf::Time& time) {
        if (!m_stateStack.empty()) {
            m_stateStack.top()->update(time);
        }
    }

    void StateManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!m_stateStack.empty()) {
            m_stateStack.top()->draw(target, states);
        }
    }

    bool StateManager::hasStates() const {
        return !m_stateStack.empty();
    }

}
