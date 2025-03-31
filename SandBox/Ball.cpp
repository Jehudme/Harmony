#include <Harmony/Script.h>
#include <Harmony/Utilities.h>
#include <Harmony/Rectangle.h>
#include <Harmony/Circle.h>
#include <Harmony/Text.h>

namespace Pong {
    constexpr float BALL_VELOCITY = 500.0f;

        HARMONY_REGISTER_SCRIPT(BallScript) : public Harmony::Script {
        void onEnter(std::shared_ptr<Object> object) override {
            const float velocityAngle = static_cast<float>(Harmony::Utilities::generateRandomNumber<int>(-75, 75));
            auto ball = std::static_pointer_cast<Harmony::Circle>(object);

            ball->positionVelocity = {
                BALL_VELOCITY * std::sin(velocityAngle),
                BALL_VELOCITY * std::cos(velocityAngle)
            };
        }

        void onUpdate(std::shared_ptr<Object> object, const sf::Time& time, Harmony::TaskQueue& taskQueue) override {
            sf::FloatRect arenaBounds = Harmony::find<Harmony::Rectangle>("Arena")->sprite.getLocalBounds();
            auto ball = std::static_pointer_cast<Harmony::Circle>(object);
            auto scoreLeft = Harmony::find<Harmony::Text>("ScoreLeft");
            auto scoreRight = Harmony::find<Harmony::Text>("ScoreRight");

            const float xPosition = ball->getPosition().x;
            const float yPosition = ball->getPosition().y;
            const float xOrigin = ball->getOrigin().x + 2;
            const float yOrigin = ball->getOrigin().y + 2;

            if (xPosition - ball->getOrigin().x < arenaBounds.left)
            {
                const std::string stringScore = scoreRight->sprite.getString();
                const int newScore = std::stoi(stringScore) + 1;

                scoreRight->sprite.setString(std::to_string(newScore));

				if (newScore >= 1)
				{
					taskQueue.push(Harmony::create<Harmony::QueueSceneTask>("GameState", "EndGameScene"));
				}
            }
            

            if (xPosition + ball->getOrigin().x > arenaBounds.left + arenaBounds.width)
            {
                const std::string stringScore = scoreLeft->sprite.getString();
                const int newScore = std::stoi(stringScore) + 1;

                scoreLeft->sprite.setString(std::to_string(newScore));

                if (newScore >= 1)
                {
                    taskQueue.push(Harmony::create<Harmony::QueueSceneTask>("GameState", "EndGameScene"));
                }
            }

            // Check for horizontal collisions with board boundaries
            if (xPosition - ball->getOrigin().x < arenaBounds.left || xPosition + ball->getOrigin().x > arenaBounds.left + arenaBounds.width) {
                ball->positionVelocity.x *= -1;
                ball->setPosition(
                    std::clamp(xPosition, xOrigin, arenaBounds.width - xOrigin),
                    std::clamp(yPosition, yOrigin, arenaBounds.height - yOrigin)
                );

                ball->reset();
            }

            // Check for vertical collisions with board boundaries
            if (yPosition - ball->getOrigin().y < arenaBounds.top || yPosition + ball->getOrigin().y > arenaBounds.top + arenaBounds.height) {
                ball->positionVelocity.y *= -1;
                ball->setPosition(
                    std::clamp(xPosition, xOrigin, arenaBounds.width - xOrigin),
                    std::clamp(yPosition, yOrigin, arenaBounds.height - yOrigin)
                );
            }
        }

        void onDraw(std::shared_ptr<const Object> object, sf::RenderTarget& renderTarget, sf::RenderStates states) const override {
        }
    };
}