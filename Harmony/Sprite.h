#pragma once
#include "Entity.h"
#include "Resource.h"

namespace Harmony
{
    class Sprite : public Entity
    {
    public:
        Sprite();
        Sprite(const uint64_t& configuration_id);
        Sprite(const std::shared_ptr<Configuration> configuration);

    protected:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateCurrent(const sf::Time& time) override;

    private:
        void initialize(const Configuration& configuration);

    private:
        sf::Sprite drawableInstance_;
        std::shared_ptr<Resource<sf::Texture>> texture_;
        std::shared_ptr<Resource<sf::Shader>> shader_;
    };
}


