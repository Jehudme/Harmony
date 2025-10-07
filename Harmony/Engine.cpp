#include "pch.h"
#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>

namespace Harmony::Internals {

    Engine::Engine(Configuration& configuration)
        : configuration_(configuration),
        taskManager_(std::make_unique<TaskManagement>())
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

        window_.create(sf::VideoMode(width, height), title);
        if (targetFPS_ > 0) {
            window_.setFramerateLimit(targetFPS_);
        }
    }

    void Engine::start() {
        clock_.restart();
        running_ = true;
        paused_ = false;

        while (running_ && window_.isOpen()) {
            deltaTime_ = clock_.restart();

            // Main loop stages
            handleTasks();
            handleEvents();

            if (!paused_) {
                if (preUpdateCallback_) preUpdateCallback_(deltaTime_.asSeconds());
                handleUpdates();
                if (postUpdateCallback_) postUpdateCallback_(deltaTime_.asSeconds());
            }

            handleRendering();
        }
    }

    void Engine::stop() {
        running_ = false;
        if (window_.isOpen()) {
            window_.close();
        }
    }

    void Engine::pause() {
        paused_ = true;
    }

    void Engine::resume() {
        paused_ = false;
		clock_.restart();
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
            window_.setFramerateLimit(targetFPS_);
        }
        else {
            window_.setFramerateLimit(0); // uncapped
        }
    }

    unsigned int Engine::getTargetFPS() const noexcept {
        return targetFPS_;
    }

    sf::Time Engine::getDeltaTime() const noexcept {
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
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                stop();
            }
        }
    }

    void Engine::handleUpdates() {
    }

    void Engine::handleRendering() {
        window_.clear(sf::Color::Black);

        window_.display();
    }

} // namespace Harmony