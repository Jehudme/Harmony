#include "pch.h"
#include "Engine.h"
#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>

namespace Harmony {

    // Internal state hidden from the header
    struct Engine::Internal {
        sf::RenderWindow window;
    };

    Engine::Engine(Configuration& configuration)
        : configuration_(configuration),
        taskManager_(std::make_unique<TaskManagement>()),
        internal_(std::make_unique<Internal>())
    {
        // Load window settings from configuration
        auto titleOpt = configuration_.get<std::string>({ "window", "title" });
        auto widthOpt = configuration_.get<unsigned int>({ "window", "width" });
        auto heightOpt = configuration_.get<unsigned int>({ "window", "height" });
        auto fpsOpt = configuration_.get<unsigned int>({ "window", "fps" });

        std::string title = titleOpt.value_or("Harmony Engine");
        unsigned int width = widthOpt.value_or(800);
        unsigned int height = heightOpt.value_or(600);
        targetFPS_ = fpsOpt.value_or(0); // 0 = uncapped

        internal_->window.create(sf::VideoMode(width, height), title);
        if (targetFPS_ > 0) {
            internal_->window.setFramerateLimit(targetFPS_);
        }

        lastFrameTime_ = std::chrono::steady_clock::now();
    }

    void Engine::start() {
        running_ = true;
        paused_ = false;

        while (running_ && internal_->window.isOpen()) {
            // Calculate delta time
            TimePoint currentFrameTime = std::chrono::steady_clock::now();
            deltaTime_ = std::chrono::duration_cast<Duration>(currentFrameTime - lastFrameTime_).count();
            lastFrameTime_ = currentFrameTime;

            // Main loop stages
            handleTasks();
            handleEvents();

            if (!paused_) {
                if (preUpdateCallback_) preUpdateCallback_(deltaTime_);
                handleUpdates();
                if (postUpdateCallback_) postUpdateCallback_(deltaTime_);
            }

            handleRendering();

            // Optional FPS limiting (if not using SFML's setFramerateLimit)
            if (targetFPS_ > 0) {
                float frameDuration = 1.0f / static_cast<float>(targetFPS_);
                if (deltaTime_ < frameDuration) {
                    auto sleepTime = std::chrono::duration<float>(frameDuration - deltaTime_);
                    std::this_thread::sleep_for(sleepTime);
                }
            }
        }
    }

    void Engine::stop() {
        running_ = false;
        if (internal_->window.isOpen()) {
            internal_->window.close();
        }
    }

    void Engine::pause() {
        paused_ = true;
    }

    void Engine::resume() {
        paused_ = false;
        lastFrameTime_ = std::chrono::steady_clock::now(); // reset delta time
    }

    bool Engine::isRunning() const noexcept {
        return running_;
    }

    bool Engine::isPaused() const noexcept {
        return paused_;
    }

    void Engine::setTargetFPS(unsigned int fps) {
        targetFPS_ = fps;
        if (targetFPS_ > 0) {
            internal_->window.setFramerateLimit(targetFPS_);
        }
        else {
            internal_->window.setFramerateLimit(0); // uncapped
        }
    }

    unsigned int Engine::getTargetFPS() const noexcept {
        return targetFPS_;
    }

    float Engine::getDeltaTime() const noexcept {
        return deltaTime_;
    }

    void Engine::setPreUpdateCallback(std::function<void(float)> callback) {
        preUpdateCallback_ = std::move(callback);
    }

    void Engine::setPostUpdateCallback(std::function<void(float)> callback) {
        postUpdateCallback_ = std::move(callback);
    }

    TaskManagement& Engine::getTaskManager() {
        return *taskManager_;
    }

    void Engine::handleTasks() {
        taskManager_->run_ready();
    }

    void Engine::handleEvents() {
        sf::Event event;
        while (internal_->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                stop();
            }
            // You can expand this to handle keyboard/mouse/etc.
        }
    }

    void Engine::handleUpdates() {
        // Game logic updates go here
        // Example: std::cout << "Updating game logic...\n";
    }

    void Engine::handleRendering() {
        internal_->window.clear(sf::Color::Black);

        internal_->window.display();
    }

} // namespace Harmony