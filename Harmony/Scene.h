#pragma once
#include "Object.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <optional>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <type_traits>


namespace Harmony
{
    class Configuration;
    class TaskQueue;
    class Group;

    class Scene : public Object, public sf::Drawable
    {
    public:
        Scene(std::shared_ptr<Configuration> configuration);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void onEnter();
        void onExit();

        void initialize(std::shared_ptr<Configuration> configuration);

    private:
        void configure_view(std::shared_ptr<Configuration> configuration);

    private:
        std::shared_ptr<Configuration> m_configuration;

    public:
        std::shared_ptr<Group> sceneGraph;
        sf::View view;
    };
}
