#pragma once
#include "Entity.h"
#include "Resource.h"

namespace Harmony
{
    class Circle : public Entity
    {
    public:
        Circle();
        Circle(const uint64_t& configuration_id);
        Circle(const std::shared_ptr<Configuration> configuration);

    protected:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateCurrent(const sf::Time& time) override;

    private:
        void initialize(const Configuration& configuration);

    private:
        sf::CircleShape drawableInstance_;
        std::shared_ptr<Resource<sf::Texture>> texture_;
        std::shared_ptr<Resource<sf::Shader>> shader_;
    };
}
