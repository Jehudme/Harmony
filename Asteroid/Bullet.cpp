#include "Bullet.h"

static float degreesToRadians(float degrees) {
	return degrees * 3.1415926535f / 180.0f;
}

Asteroid::Bullet::Bullet(const float angle)
{
	const float velocity = 300;

	this->drawable = std::make_shared<sf::CircleShape>();
	auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

	circle->setOutlineColor(sf::Color::White);
	circle->setFillColor(sf::Color::Black);
	circle->setOutlineThickness(3);
	circle->setPointCount(5);
	circle->setRadius(5);

	positionVelocity = { velocity * sin(-degreesToRadians(getRotation())), velocity * cos(-degreesToRadians(getRotation())) };
}

void Asteroid::Bullet::updateCurrent(const sf::Time& time)
{
	if (clock.getElapsedTime().asSeconds() > 2) { parentNode->detachChild(*this); }
}
