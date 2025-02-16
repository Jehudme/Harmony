#include <Harmony/Script.h>
#include <Harmony/Utilities.h>
#include <Harmony/Rectangle.h>
#include <Harmony/Circle.h>
#include <SFML/Window/Keyboard.hpp>

namespace Pong {

    constexpr float BAR_SPEED = 300;

    HARMONY_REGISTER_SCRIPT(RightBarScript) : public Harmony::Script{
        void onUpdate(std::shared_ptr<Object> object, const sf::Time& time, Harmony::TaskQueue& taskQueue) override 
        {
            auto bar = std::static_pointer_cast<Harmony::Rectangle>(object);
            float direction = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                direction--;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                direction++;

            bar->positionVelocity.y = BAR_SPEED * direction;
        }
    };

    HARMONY_REGISTER_SCRIPT(LeftBarScript) : public Harmony::Script{
        void onUpdate(std::shared_ptr<Object> object, const sf::Time & time, Harmony::TaskQueue & taskQueue) override
        {
            auto bar = std::static_pointer_cast<Harmony::Rectangle>(object);
            auto ball = Harmony::find<Harmony::Circle>("Ball");
            float direction = 0;

            bar->setPosition(bar->getPosition().x, ball->getPosition().y);

            bar->positionVelocity.y = BAR_SPEED * direction;
        }
    };
}