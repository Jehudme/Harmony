#include "pch.h"

#include "StateManagement.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "State.h"


namespace Harmony::Internals 
{
    StateManagement::StateManagement(Engine& engine) :
        engine(engine) 
    {
        const auto startupQueueIds = engine.configuration.get<Utilities::UUIDList>({ "startupStatesIds" });
        
        if (startupQueueIds.has_value()) 
            for (Utilities::UUID stateId : startupQueueIds.value())
                push(stateId);
    }

    StateManagement::~StateManagement() = default;

    void StateManagement::push(std::uint64_t stateId) 
    {
        const std::string stateKey = std::to_string(stateId);
        const std::optional<Configuration> configuration = engine.configuration.subsection({ "states", stateKey });

        if (configuration.has_value()) 
        {
            std::lock_guard<std::mutex> lock(mutex_);
			std::shared_ptr<State> state = std::make_shared<State>(configuration.value(), engine);
			states_.push(state);
        }
    }

    void StateManagement::pop() 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty()) 
            states_.pop();
    }

    void StateManagement::draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!states_.empty())
            states_.front()->draw(target, states);
    }

    void StateManagement::update(sf::Time deltaTime, TaskManagement& taskManagement) 
    {
        if (!states_.empty())
            states_.front()->update(deltaTime, taskManagement);
    }

}