#include "pch.h"
#include "StateStack.h"
#include "State.h"
#include "Configuration.h"
#include "Script.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <algorithm>

namespace Harmony {

    StateStack::StateStack(std::shared_ptr<Configuration> configuration)
        : Object(configuration) {
        if (const auto states = configuration->get({ Config::STATES })) {
            for (const auto& state : states.value()) {
                add(create<State>(create<Configuration>(state)));
            }
        }
        else {
            throw std::runtime_error(Error::NO_STATES_FOUND);
        }

        if (const auto initialState = configuration->get<std::string>({ Config::INITIAL_STATE })) {
            push(initialState.value());
        }

        if (const auto scriptName = configuration->get<std::string>({ Config::SCRIPT })) {
            script_ = find<Script>(scriptName.value());
        }
    }

    void StateStack::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (script_) {
            script_->onDraw(shared_from_this(), renderTarget, states);
        }
        if (!buffer_.empty()) {
            buffer_.top()->draw(renderTarget, states);
        }
    }

    void StateStack::update(const sf::Time& time, TaskQueue& taskQueue) {
        if (script_) {
            script_->onUpdate(shared_from_this(), time, taskQueue);
        }

        if (!buffer_.empty()) {
            buffer_.top()->update(time, taskQueue);
        }
    }

    void StateStack::add(std::shared_ptr<State> state) {
        states_.push_back(state);
    }

    void StateStack::remove(uint64_t uniqueId) {
        states_.erase(std::remove_if(states_.begin(), states_.end(),
            [uniqueId](const std::shared_ptr<State>& state) {
                return state->getUniqueId() == uniqueId;
            }), states_.end());
    }

    void StateStack::remove(const std::string& name) {
        states_.erase(std::remove_if(states_.begin(), states_.end(),
            [name](const std::shared_ptr<State>& state) {
                return state->getName() == name;
            }), states_.end());
    }

    void StateStack::removeAll() {
        states_.clear();
    }

    void StateStack::clear() {
        states_.clear();
        while (!buffer_.empty()) {
            buffer_.pop();
        }
    }

    void StateStack::push(const std::string& stateName) {
        auto state = get(stateName);
        state->onEnter();
        buffer_.push(state);
    }

    void StateStack::push(uint64_t uniqueId) {
        auto state = get(uniqueId);
        state->onEnter();
        buffer_.push(state);
    }

    void StateStack::pop() {
        if (!buffer_.empty()) {
            buffer_.pop();
        }
    }

    std::shared_ptr<State> StateStack::getCurrent() const {
        if (!buffer_.empty()) {
            return buffer_.top();
        }
        throw std::runtime_error(Error::NO_CURRENT_STATE);
    }

    std::shared_ptr<State> StateStack::get(uint64_t uniqueId) const {
        for (const auto& state : states_) {
            if (state->getUniqueId() == uniqueId) {
                return state;
            }
        }
        throw std::runtime_error(Error::STATE_NOT_FOUND);
    }

    std::shared_ptr<State> StateStack::get(const std::string& name) const {
        for (const auto& state : states_) {
            if (state->getName() == name) {
                return state;
            }
        }
        throw std::runtime_error(Error::STATE_NOT_FOUND);
    }
}