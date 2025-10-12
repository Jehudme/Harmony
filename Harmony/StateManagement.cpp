#include "pch.h"

#include "StateManagement.h"
#include "Engine.h"
#include "Configuration.h"
#include "SceneManagement.h"

namespace Harmony::Internals {

	using uuidList = std::vector<std::uint64_t>;

    StateManagement::StateManagement(Engine& engine) :
        engine_(engine) {

        if (auto statesStartupQueue = engine_.configuration.get<uuidList>({ "startupStatesIds" })) {
            for (auto stateId : statesStartupQueue.value()) {
                push(stateId);
            }
        } 
    }

    void StateManagement::push(std::uint64_t stateId) {
        const std::string stateKey = std::to_string(stateId);
        const std::optional<Configuration> configuration = engine_.configuration.subsection({stateKey});

        if (configuration) {
            states_.emplace(std::make_unique<State>(configuration.value(), engine_));
        }
    }

    void StateManagement::pop() {
        if (!states_.empty()) {
            states_.pop();
        }
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