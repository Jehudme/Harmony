#include "PlayerTrail.h"
#include "Harmony/NodeEvent.h"

Asteroid::PlayerTrail::PlayerTrail(float angle, sf::Vector2f position)
{
	const float velocity = 300;

	this->drawable = std::make_shared<sf::CircleShape>();
	auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

	circle->setOutlineColor(sf::Color::White);
	circle->setFillColor(sf::Color::Red);
	circle->setOutlineThickness(0.5f);
	circle->setPointCount(Harmony::Utilities::generateRandomNumber<int>(3, 6));

	angle += Harmony::Utilities::generateRandomNumber<int>(-5, 5);
    rotationVelocity = Harmony::Utilities::generateRandomNumber<int>(-1000, 1000);
	
	setPosition(
		position.x + 20 * sin(-Harmony::Utilities::degreesToRadians(angle)),
		position.y + 20 * cos(-Harmony::Utilities::degreesToRadians(angle))

	);
	
	positionVelocity = { 
		velocity * sin(-Harmony::Utilities::degreesToRadians(angle)), 
		velocity * cos(-Harmony::Utilities::degreesToRadians(angle)) 
	};
}

void Asteroid::PlayerTrail::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
{
    // Constants
    constexpr float FadeDuration = .55f; // Total duration for fading (in seconds)
    constexpr float GrowthRate = 60.0f;  // Rate of radius growth (in pixels per second)
    const std::vector<sf::Color> FillColorStages = {
        { 255,   140,    0,      255 },         // Bright Orange
        { 255,   60,     0,      225 },         // Dark Orange
        { 220,   30,     0,      200 },         // Bright Red
        { 150,   10,     0,      175 },         // Dim Red
        { 100,   0,      0,      150 },         // Dark Red
        { 50,    50,     50,     125 },         // Grey (representing smoke)
        { 128,   128,    128,    100 },         // Grey (representing smoke)
        { 0,     0,      0,      50  },         // Black/Invisible (final transparency)
        { 0,     0,      0,      0   }          // Black/Invisible (final transparency)
    };

    const std::vector<sf::Color> OutlineColorStages = {
        { 255,   255,    255,    100 },
        { 255,   255,    255,    50 },         
    };

    // Retrieve the drawable as a CircleShape
    const std::shared_ptr<sf::CircleShape> circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable);
    if (!circle) return;

    const float elapseTime = clock.getElapsedTime().asSeconds();
    const float deltaTime = time.asSeconds();

    // Check if the node should be detached
    if (elapseTime > FadeDuration) {
        eventPool.addEvent(Harmony::Core::Object::create<Harmony::Event::DetachNode>(std::static_pointer_cast<SceneNode>(shared_from_this())));
        return;
    }

    // Increase the radius
    circle->setRadius(Harmony::Utilities::sinus(FadeDuration * 2, 20, elapseTime));
    setOrigin(circle->getRadius(), circle->getRadius());

    circle->setFillColor(Harmony::Utilities::getInterpolatedColor(elapseTime, FadeDuration, FillColorStages));
    circle->setOutlineColor(Harmony::Utilities::getInterpolatedColor(elapseTime, FadeDuration * 0.75f, OutlineColorStages));
}



