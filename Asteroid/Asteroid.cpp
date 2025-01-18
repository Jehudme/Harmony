#include "Asteroid.h"


Asteroid::Asteroid::Asteroid() 
{
	this->drawable = std::make_shared<sf::CircleShape>();
	auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

	circle->setOutlineColor(sf::Color::White);
	circle->setFillColor(sf::Color::Black);
	circle->setOutlineThickness(3);
	circle->setPointCount(5);
	circle->setRadius(40);

	setOrigin(40, 40);
}

void Asteroid::Asteroid::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
{
}
