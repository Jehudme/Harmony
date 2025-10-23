#pragma once

#include <memory>
#include <atomic>

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

        void setTargetFPS(unsigned int fps);
        unsigned int getTargetFPS() const noexcept;
        
        /// @brief Get delta time in seconds
        float getDeltaTime() const noexcept;

    public:
        Utilities::Configuration& configuration;

        std::unique_ptr<Management::ResourceManager> resourceManager;
        std::unique_ptr<Management::TaskManager> taskManagement;
        std::unique_ptr<Management::SceneManager> sceneManagement;
        std::unique_ptr<Management::StateManager> stateManagement;
		std::unique_ptr<Management::ComponentManager> componentManagement;
		std::unique_ptr<InputManager> inputManager;


    private:
        void handleTasks();
        void handleEvents();
        void handleUpdates();
        void handleRendering();

    private:
        // PImpl idiom - hide SFML window, clock, and time implementation
        struct EngineImpl;
        std::unique_ptr<EngineImpl> impl_;

        std::atomic<bool> running_{ false };
        std::atomic<bool> paused_{ false };
        unsigned int targetFPS_{ 0 };
    };

}