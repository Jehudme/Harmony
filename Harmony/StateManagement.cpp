#include "pch.h"

#include "StateManagement.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "State.h"
#include "Logger.h"

namespace Harmony::Errors {
    StateManagerError::StateManagerError(const std::string& msg)
            : std::runtime_error("StateManager error: " + msg) {}
}

namespace Harmony::Management
{
    StateManager::StateManager(Engine& engine)
        : engine(engine)
    {
        HARMONY_INFO("StateManager created");

        const auto startupQueueIds = engine.configuration.get<Utilities::UUIDList>({ "startupStatesIds" });
        if (startupQueueIds.has_value()) {
            HARMONY_INFO("Loading {} startup states", startupQueueIds->size());
            for (Utilities::UUID stateId : startupQueueIds.value()) {
                try {
                    push(stateId);
                }
                catch (const Errors::StateManagerError& e) {
                    HARMONY_ERROR("Failed to push startup state [{}]: {}", stateId, e.what());
                }
            }
        }
        else {
            HARMONY_WARN("No startupStatesIds found in configuration");
        }
    }

    StateManager::~StateManager() {
        HARMONY_INFO("StateManager destroyed, {} states remaining in stack", states_.size());
    }

    void StateManager::push(std::uint64_t stateId)
    {
        const std::string stateKey = std::to_string(stateId);
        const auto configuration = engine.configuration.subsection({ "states", stateKey });

        if (!configuration.has_value()) {
            HARMONY_ERROR("Failed to push state [{}]: no configuration found", stateKey);
            throw Errors::StateManagerError("Missing configuration for state " + stateKey);
        }

        std::lock_guard<std::mutex> lock(mutex_);
        auto state = std::make_shared<Scenes::State>(configuration.value(), engine);
        states_.push(state);

        HARMONY_INFO("State [{}] pushed onto stack (stack size = {})", stateKey, states_.size());
    }

    void StateManager::pop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty()) {
            HARMONY_INFO("State [{}] popped from stack (stack size = {})",
                reinterpret_cast<std::uintptr_t>(states_.front().get()), // or track IDs inside State
                states_.size() - 1);
            states_.pop();
        }
        else {
            HARMONY_WARN("Attempted to pop from empty state stack");
        }
    }

    void StateManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty()) {
            HARMONY_DEBUG("Drawing top state");
            states_.front()->draw(target, states);
        }
        else {
            HARMONY_WARN("No states to draw");
        }
    }

    void StateManager::update(sf::Time deltaTime)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty()) {
            HARMONY_DEBUG("Updating top state with deltaTime = {} ms", deltaTime.asMilliseconds());
            states_.front()->update(deltaTime);
        }
        else {
            HARMONY_WARN("No states to update");
        }
    }
}