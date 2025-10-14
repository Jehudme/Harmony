#pragma once

namespace Harmony::Internals {
    /**
     * @brief Core engine class responsible for managing the main loop,
     *        tasks, events, updates, and rendering.
     */
    class Engine {
    public:
        using TimePoint = std::chrono::steady_clock::time_point;
        using Duration = std::chrono::duration<float>;

        /**
         * @brief Construct the engine with a given configuration.
         * @param configuration Reference to the engine configuration object.
         */
        explicit Engine(Configuration& configuration);
		~Engine();

        /**
         * @brief Start the engine main loop.
         */
        void start();

        /**
         * @brief Stop the engine main loop.
         */
        void stop();

        /**
         * @brief Pause the engine without shutting it down.
         */
        void pause();

        /**
         * @brief Resume the engine if paused.
         */
        void resume();

        /**
         * @brief Check if the engine is currently running.
         */
        bool isRunning() const noexcept;

        /**
         * @brief Check if the engine is currently paused.
         */
        bool isPaused() const noexcept;

        /**
         * @brief Set the target frames per second (FPS).
         * @param fps Target FPS (0 = uncapped).
         */
        void setTargetFPS(unsigned int fps);

        /**
         * @brief Get the current target FPS.
         */
        unsigned int getTargetFPS() const noexcept;

        /**
         * @brief Get the time elapsed between the last two frames (delta time).
         */
        sf::Time getDeltaTime() const noexcept;

    public:
        Configuration& configuration;

        std::unique_ptr<TaskManagement> taskManagement;
        std::unique_ptr<SceneManagement> sceneManagement;
        std::unique_ptr<StateManagement> stateManagement;

    private:
        // Internal loop stages
        void handleTasks();
        void handleEvents();
        void handleUpdates();
        void handleRendering();

    private:
        // Internal state
        sf::RenderWindow window_;

        std::atomic<bool> running_{ false };
        std::atomic<bool> paused_{ false };

        unsigned int targetFPS_{ 0 }; // 0 = uncapped
        sf::Time deltaTime_;
		sf::Clock clock_;
    };

} // namespace Harmony