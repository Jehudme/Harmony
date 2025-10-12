#include "pch.h"

#include "pch.h"
#include "StateManagement.h"

namespace Harmony::Internals {

    StateManagement::StateManagement(const Configuration& configuration)
        : configuration_(configuration)
    {
        auto startupStates = configuration_.get<std::vector<std::uint64_t>>({ "startup-states" }).value_or({});
        for (auto stateId : startupStates) {
            push(stateId);
        }
    }

    void StateManagement::push(std::uint64_t stateId) {
        auto stateKey = std::to_string(stateId);
        auto configuration = configuration_.subsection({ stateKey });

        if (configuration) {
            states_.emplace(std::make_shared<State>(*configuration));
        }
    }

    void StateManagement::swap(std::uint64_t stateId) {
        auto stateKey = std::to_string(stateId);
        auto configuration = configuration_.subsection({ stateKey });

        if (configuration && !states_.empty()) {
            states_.front() = std::make_shared<State>(*configuration);
        }
    }

    void StateManagement::pop() {
        if (!states_.empty()) {
            states_.pop();
        }
    }

    std::optional<std::uint64_t> StateManagement::getId(const std::string& targetName) const {
        const auto keys = configuration_.extractKeys({});

        for (const auto& key : keys) {
            auto stateName = configuration_.get<std::string>({ key, "name" });
            if (stateName && *stateName == targetName) {
                return std::stoull(key);
            }
        }
        return std::nullopt;
    }

    void StateManagement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!states_.empty()) {
            states_.front()->draw(target, states);
        }
    }

    void StateManagement::update(sf::Time deltaTime, TaskManagement& taskManagement) {
        if (!states_.empty()) {
            states_.front()->update(deltaTime, taskManagement);
        }
    }

} // namespace Harmony::Internals