#pragma once

#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <string>
#include <stdexcept>

namespace Harmony {

    class Configuration;
    class TaskQueue;
    class Group;
    struct Script;

    class Scene : public Object, public sf::Drawable {
    public:
        explicit Scene(std::shared_ptr<Configuration> configuration);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void onEnter();
        void onExit();

        void initialize(std::shared_ptr<Configuration> configuration);

    private:
        void configureView(std::shared_ptr<Configuration> configuration);

        std::shared_ptr<Configuration> configuration_;
        std::shared_ptr<Group> sceneGraph_;
        sf::View view_;
        std::shared_ptr<Script> script_;
    };
}