#pragma once
#include "Entity.h"
#include "Resource.h"

namespace Harmony
{
    class Rectangle : public Entity
    {
    public:
        Rectangle();
        Rectangle(const uint64_t& configuration_id);
        Rectangle(const std::shared_ptr<Configuration> configuration);

    protected:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateCurrent(const sf::Time& time) override;

    private:
        void initialize(const Configuration& configuration);

    private:
        sf::RectangleShape drawableInstance_;
        std::shared_ptr<Resource<sf::Texture>> texture_;
        std::shared_ptr<Resource<sf::Shader>> shader_;
    };
}


