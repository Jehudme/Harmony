#pragma once

#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <memory>
#include <string>
#include <variant>

namespace Harmony {

    class TaskQueue;
    class Scene;
    struct Script;

    class State : public Object, public sf::Drawable {
    public:
        explicit State(std::shared_ptr<Configuration> configuration);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void onEnter();
        void onExit();

        // Overloaded functions for getting scenes
        std::shared_ptr<Scene> getScene(const std::string& name) const;
        std::shared_ptr<Scene> getScene(uint64_t uniqueId) const;

        void addScene(std::shared_ptr<Scene> scene);

        // Overloaded functions for removing scenes
        void removeScene(const std::string& name);
        void removeScene(uint64_t uniqueId);

        // Overloaded functions for queuing scenes
        void queueScene(const std::string& name);
        void queueScene(uint64_t uniqueId);

        // Overloaded functions for removing queued scenes
        void removeQueuedScene(const std::string& name);
        void removeQueuedScene(uint64_t uniqueId);

        void clearSceneBuffer();

    private:
        void processInitializeScenes();

        std::vector<std::variant<uint64_t, std::string>> initialBuffer_;
        std::vector<std::shared_ptr<Scene>> sceneBuffer_;
        std::vector<std::shared_ptr<Scene>> scenes_;
        std::shared_ptr<Script> script_;
    };
}