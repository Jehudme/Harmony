#include <Harmony/Utilities.h>
#include "BackgroundStar.h"

void Asteroid::BackgroundStar::onCreate(harmony::core::Scene& scene)
{
    this->drawable = std::make_shared<sf::CircleShape>();
    auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

    setPosition(
        static_cast<float>(harmony::utilities::generateRandomNumber<int>(NULL, static_cast<int>(currentScene->view.getSize().x))),
        static_cast<float>(harmony::utilities::generateRandomNumber<int>(NULL, static_cast<int>(currentScene->view.getSize().y)))
    );

    circle->setPointCount(6);
    circle->setRadius(static_cast<float>(harmony::utilities::generateRandomNumber<int>(1, 10)) / 10);

    offset = static_cast<float>(harmony::utilities::generateRandomNumber<int>(0, 100)) / 10;
    duration = static_cast<float>(harmony::utilities::generateRandomNumber<int>(0, 10));
}

void Asteroid::BackgroundStar::onDestroy(harmony::core::Scene& scene)
{
}

void Asteroid::BackgroundStar::onUpdate(const sf::Time& time, harmony::core::EventQueue& eventQueue)
{
    const float FadeDuration = 2.0f;

    // Retrieve the drawable as a CircleShape
    const std::shared_ptr<sf::CircleShape> circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable);
    if (!circle) return;

    // Calculate elapsed time (do not reset the clock during this calculation)
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // Wrap elapsed time to create a periodic fade effect
    if (elapsedTime > duration + FadeDuration) {
        clock.restart();
        elapsedTime = 0.0f;
    }

    // Interpolate color based on elapsed time
    const std::vector<sf::Color> FillColorStages = {
        { 255, 255, 255, 150 }, // Bright white
        { 0,   0,   0,   50  }, // Dim black
        { 255, 255, 255, 150 }, // Bright white
    };

    // Get the interpolated color based on elapsed time and duration
    circle->setFillColor(harmony::utilities::getInterpolatedColor(elapsedTime, duration + FadeDuration, FillColorStages));
}

