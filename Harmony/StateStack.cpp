#include "pch.h"
#include "State.h"
#include "StateStack.h"
#include "Configuration.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace Harmony {

    StateStack::StateStack(std::shared_ptr<Configuration> configuration)
        : Object(configuration) {
        if (const auto states = configuration->get({ "States" })) {
            for (const auto state : states.value()) {
                add(create<State>(create<Configuration>(state)));
            }
        } else {
            throw std::runtime_error("No States Found");
        }

        if (const auto initialState = configuration->get<std::string>({ "InitialState" })) {
            push(initialState.value());
        }
    }

    void StateStack::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (!m_buffer.empty()) {
            m_buffer.top()->draw(renderTarget, states);
        }
    }

    void StateStack::update(const sf::Time& time, TaskQueue& taskQueue) {
        for (const auto& state : m_states) {
            state->update(time, taskQueue);
        }
    }

    void StateStack::add(std::shared_ptr<State> state) {
        m_states.push_back(state);
    }

    void StateStack::remove(const uint64_t& uniqueId) {
        auto it = std::remove_if(m_states.begin(), m_states.end(),
            [uniqueId](const std::shared_ptr<State>& state) {
                return state->getUniqueId() == uniqueId;
            });
        m_states.erase(it);
    }

    void StateStack::remove(const std::string& name) {
        auto it = std::remove_if(m_states.begin(), m_states.end(),
            [name](const std::shared_ptr<State>& state) {
                return state->getName() == name;
            });
        m_states.erase(it);
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

    void StateStack::push(const std::string& stateName) {
        auto state = get(stateName);
        state->onEnter();
        m_buffer.push(state);
    }

    void StateStack::push(const uint64_t& uniqueId) {
        auto state = get(uniqueId);
        state->onEnter();
        m_buffer.push(state);
    }

    void StateStack::pop() {
        m_buffer.pop();
    }

    std::shared_ptr<State> StateStack::getCurrent() {
        return m_buffer.top();
    }

    std::shared_ptr<State> StateStack::get(const uint64_t& uniqueId) {
        for (const auto& state : m_states) {
            if (state->getUniqueId() == uniqueId) {
                return state;
            }
        }
        throw std::runtime_error("State not found");
    }

    std::shared_ptr<State> StateStack::get(const std::string& name) {
        for (const auto& state : m_states) {
            if (state->getName() == name) {
                return state;
            }
        }
        throw std::runtime_error("State not found");
    }
}

