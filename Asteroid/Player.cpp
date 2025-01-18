#include "Player.h"
#include "Bullet.h"
#include "Harmony/Scene.h"

void Asteroid::Player::onEnterCurrent(Harmony::Core::Scene& scene)
{
	this->drawable = std::make_shared<sf::CircleShape>();
	auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

	circle->setOutlineColor(sf::Color::White);
	circle->setFillColor(sf::Color::Black);
	circle->setOutlineThickness(3);
	circle->setPointCount(3);
	circle->setRadius(20);

	setPosition(scene.view.getCenter());
	setOrigin(20, 0);

}

void Asteroid::Player::onExitCurrent(Harmony::Core::Scene& scene)
{
}

static float degreesToRadians(float degrees) {
	return degrees * 3.1415926535f / 180.0f;
}

void Asteroid::Player::updateCurrent(const sf::Time& time)
{
	const float deltaTime = time.asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bulletClock.getElapsedTime().asSeconds() > .5f) {
  		currentScene->sceneGraph->attachChild(Harmony::Core::Object::create<Bullet>(getRotation()));
		currentScene->sceneGraph->attachChild(Harmony::Core::Object::create<Bullet>(getRotation()));
		bulletClock.restart();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		rotate(-120 * deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		rotate(120 * deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))			{ acceleration = -140; } 
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{ acceleration = +140; } 
	
	else {
		if (velocity < - 10)									{ acceleration = 140; } 
		else if (velocity > 10)									{ acceleration = -140;} 
		else													{ acceleration = 0; }
	}

	velocity += acceleration * deltaTime;

	if (velocity > 300)											{ velocity = 300; } 
	else if (velocity < -300)									{ velocity = -300; }

	if (velocity < - 5 && velocity > 5)							{ velocity = 0; }

	positionVelocity = { velocity * sin(-degreesToRadians(getRotation())), velocity * cos(-degreesToRadians(getRotation())) };
}