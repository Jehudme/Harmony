#include "Asteroid.h"
#include "Harmony/Scene.h"
#include "Player.h"

namespace Asteroid
{
	Asteroid_t::Asteroid_t(const sf::View& sceneView)
	{
		const float velocity = Harmony::Utilities::generateRandomNumber<int>(100, 500);
		const float angle = Harmony::Utilities::generateRandomNumber<int>(0, 360);

		rotationVelocity = Harmony::Utilities::generateRandomNumber<int>(-500, 500);

		this->drawable = std::make_shared<sf::CircleShape>();
		auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

		circle->setOutlineColor(sf::Color::White);
		circle->setFillColor(sf::Color::Black);
		circle->setOutlineThickness(2);
		circle->setPointCount(6);
		circle->setRadius(30);
		setOrigin(circle->getRadius(), circle->getRadius());


		setPosition(
			Harmony::Utilities::getViewBounds(sceneView).left - circle->getRadius(),
			Harmony::Utilities::getViewBounds(sceneView).top - circle->getRadius()
		);

		positionVelocity = { velocity * sin(-Harmony::Utilities::degreesToRadians(angle)), velocity * cos(-Harmony::Utilities::degreesToRadians(angle)) };
	}

	void Asteroid_t::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
	{
		const sf::FloatRect field = Harmony::Utilities::getViewBounds(currentScene->view);

		const sf::Vector2f curentPosition = getPosition();
		const sf::Vector2f currentViewSize = currentScene->view.getSize();

		const std::shared_ptr<sf::CircleShape> circle = std::static_pointer_cast<sf::CircleShape>(drawable);

		// Wrap the asteroid's position around the screen boundaries
		float newX = curentPosition.x;
		float newY = curentPosition.y;

		if (curentPosition.x < field.left)
			newX += field.width;
		else if (curentPosition.x > field.left + field.width)
			newX -= field.width;

		if (curentPosition.y < field.top)
			newY += field.height;
		else if (curentPosition.y > field.top + field.height)
			newY -= field.height;

		setPosition(newX, newY);

		if (intersect(Harmony::Core::Object::find<Player>(PlayerUniqueId)))
		{
			//exit(0);
		}
	}
}
