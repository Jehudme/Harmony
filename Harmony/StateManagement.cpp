#include "pch.h"

#include "StateManagement.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "State.h"
#include "Logger.h"

namespace Harmony::Management
{
    StateManager::StateManager(Engine& engine)
        : engine(engine)
    {
        const auto startupQueueIds = engine.configuration.get<Utilities::UUIDList>({ "startupStatesIds" });
        if (startupQueueIds.has_value())
            for (Utilities::UUID stateId : startupQueueIds.value()) push(stateId);
		else throw Exceptions::StartupStatesNotDefined();
    }

    StateManager::~StateManager() { HARMONY_INFO("StateManager destroyed, {} states remaining in stack", states_.size()); }

    void StateManager::push(std::uint64_t stateId)
    {
        const std::string stateKey = std::to_string(stateId);
        const auto configuration = engine.configuration.subsection({ "states", stateKey });

        if (!configuration.has_value()) throw Exceptions::StateConfigurationNotFound(stateKey);

        std::lock_guard<std::shared_mutex> lock(mutex_);
        states_.emplace(std::make_shared<Scenes::State>(configuration.value(), engine));

        HARMONY_INFO("State [{}] pushed onto stack (stack size = {})", stateKey, states_.size());
    }

    void StateManager::pop()
    {
        std::lock_guard<std::shared_mutex> lock(mutex_);
        if (!states_.empty()) states_.pop();
        else HARMONY_WARN("Attempted to pop from empty state stack");

        HARMONY_INFO("State [{}] popped from stack (stack size = {})",
            reinterpret_cast<std::uintptr_t>(states_.front().get()),
            states_.size());
    }

    void StateManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (!states_.empty()) {
            std::shared_lock<std::shared_mutex> lock(mutex_);
			const std::shared_ptr<Scenes::State>& currentState = states_.front();
			lock.unlock();

            currentState->draw(target, states);
        }
        else throw Exceptions::StateStackEmptyError();
    }

    void StateManager::update(sf::Time deltaTime)
    {
        if (!states_.empty()) 
        {
			std::shared_lock<std::shared_mutex> lock(mutex_);
			const std::shared_ptr<Scenes::State>& currentState = states_.front();
            lock.unlock();

            currentState->update(deltaTime);
        }
		else throw Exceptions::StateStackEmptyError();
    }
}