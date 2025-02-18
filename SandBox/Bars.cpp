#include <Harmony/Script.h>
#include <Harmony/Utilities.h>
#include <Harmony/Rectangle.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <map>

namespace Pong {

    constexpr float BAR_SPEED = 300.f;

    struct PaddleScript : public Harmony::Script {
        void onEnter(std::shared_ptr<Object> object) override {
            auto paddle = std::static_pointer_cast<Harmony::Rectangle>(object);
            auto id = paddle->getUniqueId();

            defaultColor[id] = paddle->sprite.getFillColor();
            clocks[id] = sf::Clock();
        }

        void onUpdate(std::shared_ptr<Object> object, const sf::Time& time, Harmony::TaskQueue&) override {
            auto paddle = std::static_pointer_cast<Harmony::Rectangle>(object);
            auto ball = Harmony::find<Harmony::Rectangle>("Ball");
            auto arena = Harmony::find<Harmony::Rectangle>("Arena");


            const auto id = paddle->getUniqueId();
            const auto arenaBounds = arena->sprite.getLocalBounds();
            float direction = (sf::Keyboard::isKeyPressed(upKey) ? -1.f : 0.f) +
                (sf::Keyboard::isKeyPressed(dwKey) ? 1.f : 0.f);

            paddle->positionVelocity.y = BAR_SPEED * direction;

            if (Harmony::SceneNode::intersect(paddle, ball)) {
                ball->positionVelocity.x *= -1;
                clocks[paddle->getUniqueId()].restart();
            }

            auto elapsed = clocks[paddle->getUniqueId()].getElapsedTime().asSeconds();
            paddle->sprite.setFillColor(Harmony::Utilities::getInterpolatedColor(elapsed, 0.5f, {
                sf::Color::White, defaultColor[id]
                }));

            paddle->setPosition({
                paddle->getPosition().x,
                std::clamp(paddle->getPosition().y, paddle->getOrigin().y, arenaBounds.height - paddle->getOrigin().y)
                });
        }

        sf::Keyboard::Key upKey, dwKey;

        static inline std::map<uint64_t, sf::Color> onCollisionColor;
        static inline std::map<uint64_t, sf::Color> defaultColor;
        static inline std::map<uint64_t, sf::Clock> clocks;
    };

    HARMONY_REGISTER_SCRIPT(LeftBarScript) : public PaddleScript{
        void onEnter(std::shared_ptr<Object> object) override {
            PaddleScript::onEnter(object);
            upKey = sf::Keyboard::Up;
            dwKey = sf::Keyboard::Down;
        }
    };

    HARMONY_REGISTER_SCRIPT(RightBarScript) : public PaddleScript{
        void onEnter(std::shared_ptr<Object> object) override {
            PaddleScript::onEnter(object);
            upKey = sf::Keyboard::W;
            dwKey = sf::Keyboard::S;
        }
    };

}
