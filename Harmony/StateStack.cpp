#include "pch.h"
#include "State.h"
#include "StateStack.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace Harmony {

    StateStack::StateStack(std::shared_ptr<Configuration> configuration)
        : Object(configuration) {
    }

    void StateStack::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        for (const auto& state : m_states) {
            renderTarget.draw(*state, states);
        }
    }

    void StateStack::update(const sf::Time& time, TaskQueue& taskQueue) {
        for (const auto& state : m_states) {
            state->update(time, taskQueue);
        }
    }

    void StateStack::add(std::shared_ptr<State> state) {
        m_buffer.push(state);
    }

    void StateStack::remove(const uint64_t& uniqueId) {
        auto it = std::remove_if(m_states.begin(), m_states.end(),
            [uniqueId](const std::shared_ptr<State>& state) {
                return state->getUniqueId() == uniqueId;
            });
        m_states.erase(it, m_states.end());
    }

    void StateStack::remove(const std::string& name) {
        auto it = std::remove_if(m_states.begin(), m_states.end(),
            [name](const std::shared_ptr<State>& state) {
                return state->getName() == name;
            });
        m_states.erase(it, m_states.end());
    }

    void StateStack::removeAll() {
        m_states.clear();
    }

    void StateStack::clear() {
        m_states.clear();
        while (!m_buffer.empty()) {
            m_buffer.pop();
        }
    }

    std::shared_ptr<State> StateStack::findState(const uint64_t& uniqueId) {
        for (const auto& state : m_states) {
            if (state->getUniqueId() == uniqueId) {
                return state;
            }
        }
        throw std::runtime_error("State not found");
    }

    std::shared_ptr<State> StateStack::findState(const std::string& name) {
        for (const auto& state : m_states) {
            if (state->getName() == name) {
                return state;
            }
        }
        throw std::runtime_error("State not found");
    }

}

