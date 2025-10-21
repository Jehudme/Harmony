#pragma once

namespace Harmony
{
    class Engine 
    {
    public:
        Engine(Utilities::Configuration& configuration);
		~Engine();

        void start();
        void stop();

        bool isRunning() const noexcept;

        void setTargetFPS(unsigned int fps);
        unsigned int getTargetFPS() const noexcept;
        sf::Time getDeltaTime() const noexcept;

    public:
        Utilities::Configuration& configuration;

        std::unique_ptr<Management::TaskManager> taskManagement;
        std::unique_ptr<Management::SceneManager> sceneManagement;
        std::unique_ptr<Management::StateManager> stateManagement;

    private:
        void handleTasks();
        void handleEvents();
        void handleUpdates();
        void handleRendering();

    private:
        sf::RenderWindow window_;

        std::atomic<bool> running_{ false };
        std::atomic<bool> paused_{ false };

        unsigned int targetFPS_{ 0 };
        sf::Time deltaTime_;
		sf::Clock clock_;
    };

}