#include "BackgroundStar.h"

void Asteroid::BackgroundStar::onEnterCurrent(Harmony::Core::Scene& scene)
{
    this->drawable = std::make_shared<sf::CircleShape>();
    auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

    setPosition(
        Harmony::Utilities::generateRandomNumber<int>(0, static_cast<int>(currentScene->view.getSize().x)),
        Harmony::Utilities::generateRandomNumber<int>(0, static_cast<int>(currentScene->view.getSize().y))
    );

    circle->setPointCount(10);
    circle->setRadius(Harmony::Utilities::generateRandomNumber<int>(1, 3));

    offset = static_cast<float>(Harmony::Utilities::generateRandomNumber<int>(0, 100)) / 10;
    duration = static_cast<float>(Harmony::Utilities::generateRandomNumber<int>(0, 10));
}

void Asteroid::BackgroundStar::onExitCurrent(Harmony::Core::Scene& scene)
{
}

void Asteroid::BackgroundStar::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
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
    circle->setFillColor(Harmony::Utilities::getInterpolatedColor(elapsedTime, duration + FadeDuration, FillColorStages));
}

