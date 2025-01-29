#pragma once
#include "Object.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace Harmony
{
    class Configuration;
    class TaskQueue;

    class Scene : public Object, public sf::Drawable
    {
    public:
        Scene(std::shared_ptr<Configuration> configuration);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void onEnter();
        void onExit();
    };
}

