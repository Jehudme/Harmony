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
        State(std::shared_ptr<Configuration> configuration);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void onEnter();
        void onExit();

        std::shared_ptr<Scene> getSceneByName(const std::string& name);
        std::shared_ptr<Scene> getSceneById(const uint64_t& uniqueId);

        void addScene(std::shared_ptr<Scene> scene);
        void removeSceneByName(const std::string& name);
        void removeSceneById(const uint64_t& uniqueId);

        void queueSceneByName(const std::string& name);
        void queueSceneById(const uint64_t& uniqueId);

        void removeQueuedSceneByName(const std::string& name);
        void removeQueuedSceneById(const uint64_t& uniqueId);

        void clearSceneBuffer();

    private:
        void processInitializeScenes();

    private:
        std::vector<std::variant<uint64_t, std::string>> m_initialBuffer;
        std::vector<std::shared_ptr<Scene>> m_sceneBuffer;
        std::vector<std::shared_ptr<Scene>> m_scenes;
        std::shared_ptr<Script> m_script;
    };
}


