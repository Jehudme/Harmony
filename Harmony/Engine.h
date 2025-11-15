#pragma once

#include <memory>
#include <atomic>

#include <SFML/System/Time.hpp>

// Forward declarations - no external library headers
namespace Harmony {
    namespace Utilities {
        class Configuration;
    }
    namespace Management {
        class TaskManager;
        class SceneManager;
        class StateManager;
		class ComponentManager;
		class ResourceManager;
		class RenderManager;
    }
	class InputManager;
}

namespace Harmony
{
    /// @brief Main engine class with hidden implementation details
    class Engine 
    {
    public:
        Engine(Utilities::Configuration& configuration);
		~Engine();

        // Prevent copying
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        void start();
        void stop();

        bool isRunning() const noexcept;
        
       const sf::Time& getDeltaTime() const noexcept;

    private:
        void handleTasks();
        void handleEvents();
        void handleUpdates();
        void handleRendering();

    private:
        struct EngineImpl;
        std::unique_ptr<EngineImpl> impl_;

        std::atomic<bool> running_{ false };
        std::atomic<bool> paused_{ false };
        unsigned int targetFPS_{ 0 };

    public:
        Utilities::Configuration& configuration;

        std::unique_ptr<InputManager> inputManager;
        std::unique_ptr<Management::RenderManager> renderManager;
        std::unique_ptr<Management::ResourceManager> resourceManager;
        std::unique_ptr<Management::TaskManager> taskManagement;
        std::unique_ptr<Management::SceneManager> sceneManagement;
        std::unique_ptr<Management::ComponentManager> componentManagement;
        std::unique_ptr<Management::StateManager> stateManagement;
    };

}