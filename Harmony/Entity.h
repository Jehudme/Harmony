#pragma once
#include "SceneNode.h"
#include "Resource.h"
#include <SFML/Graphics.hpp>

namespace Harmony
{
    class Entity : public SceneNode, public sf::Drawable
    {
    public:
        Entity();
        Entity(const uint64_t& configuration_id);
        Entity(const std::shared_ptr<Configuration> configuration);

        void enableDrawing(const bool option);
        void enableUpdate(const bool option);

    private:
        void initialize(const Configuration& configuration);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual void update(const sf::Time& time);
        virtual void updateCurrent(const sf::Time& time);

    private:
        bool isDrawEnabled_;
        bool isUpdateEnabled_;
    };
}

