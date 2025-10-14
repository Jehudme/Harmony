#include "pch.h"

#include "StateManagement.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "State.h"


namespace Harmony::Internals {
    StateManagement::StateManagement(Engine& engine) :
        engine(engine) {

        if (auto statesStartupQueue = engine.configuration.get<Utilities::UUIDList>({ "startupStatesIds" })) {
            for (auto stateId : statesStartupQueue.value()) {
                push(stateId);
            }
        } 
    }

    void StateManagement::push(std::uint64_t stateId) {
        const std::string stateKey = std::to_string(stateId);
        const std::optional<Configuration> configuration = engine.configuration.subsection({ "states", stateKey });

        if (configuration) {
            states_.emplace(std::make_shared<State>(configuration.value(), engine));
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

}