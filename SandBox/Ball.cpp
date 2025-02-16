#include <Harmony/Script.h>
#include <Harmony/Utilities.h>
#include <Harmony/Rectangle.h>
#include <Harmony/Circle.h>

namespace Pong {
    constexpr float BALL_VELOCITY = 500.0f;

        HARMONY_REGISTER_SCRIPT(BallScript) : public Harmony::Script {
        sf::FloatRect boardBound;

        void onEnter(Object* object) override {
            const float velocityAngle = static_cast<float>(Harmony::Utilities::generateRandomNumber<int>(-75, 75));

            auto board = Harmony::find<Harmony::Rectangle>("Board");
            auto ball = static_cast<Harmony::Circle*>(object);

            boardBound = board->getGlobalBound();

            ball->positionVelocity = {
                BALL_VELOCITY * std::sin(velocityAngle),
                BALL_VELOCITY * std::cos(velocityAngle)
            };
        }

        void onExit(Object* object) override {
            // Cleanup logic (if needed)
        }

        void onUpdate(std::shared_ptr<Object> object, const sf::Time& time, Harmony::TaskQueue& taskQueue) override {
            auto ball = std::static_pointer_cast<Harmony::Circle>(object);

            const float xPosition = ball->getGlobalPosition().x;
            const float yPosition = ball->getGlobalPosition().y;

            // Check for horizontal collisions with board boundaries
            if (xPosition < boardBound.left || xPosition > boardBound.left + boardBound.width) {
                ball->positionVelocity.x *= -1;
            }

            // Check for vertical collisions with board boundaries
            if (yPosition < boardBound.top || yPosition > boardBound.top + boardBound.height) {
                ball->positionVelocity.y *= -1;
            }

            // Clamp ball position within the board boundaries
            ball->setPosition(
                std::clamp(xPosition, boardBound.left, boardBound.left + boardBound.width),
                std::clamp(yPosition, boardBound.top, boardBound.top + boardBound.height)
            );

            // Check for collisions with the left and right bars
            if (Harmony::SceneNode::intersect(ball, Harmony::find<Harmony::SceneNode>("LeftBar"))) {
                ball->positionVelocity.x *= -1;
            }

            if (Harmony::SceneNode::intersect(ball, Harmony::find<Harmony::SceneNode>("RightBar"))) {
                ball->positionVelocity.x *= -1;
            }
        }

        void onDraw(std::shared_ptr<const Object> object, sf::RenderTarget& renderTarget, sf::RenderStates states) const override {
            // Drawing logic (if needed)
        }
    };
}