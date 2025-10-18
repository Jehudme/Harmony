#include "pch.h"

#include "StateManagement.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "State.h"


namespace Harmony::Management
{
    StateManager::StateManager(Engine& engine) :
        engine(engine) 
    {
        const auto startupQueueIds = engine.configuration.get<Utilities::UUIDList>({ "startupStatesIds" });
        
        if (startupQueueIds.has_value()) 
            for (Utilities::UUID stateId : startupQueueIds.value())
                push(stateId);
    }

    StateManager::~StateManager() = default;

    void StateManager::push(std::uint64_t stateId) 
    {
        const std::string stateKey = std::to_string(stateId);
        const std::optional<Utilities::Configuration> configuration = engine.configuration.subsection({ "states", stateKey });

        if (configuration.has_value()) 
        {
            std::lock_guard<std::mutex> lock(mutex_);
			std::shared_ptr<Scenes::State> state = std::make_shared<Scenes::State>(configuration.value(), engine);
			states_.push(state);
        }
    }

    void StateManager::pop() 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty()) 
            states_.pop();
    }

    void StateManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty())
            states_.front()->draw(target, states);
    }

    void StateManager::update(sf::Time deltaTime) 
    {
        if (!states_.empty())
            states_.front()->update(deltaTime);
    }

}