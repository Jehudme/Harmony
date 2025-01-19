#include "Bullet.h"
#include <Harmony/Scene.h>
#include <Harmony/NodeEvent.h>
#include <Harmony/Utilities.h>
#include "AsteroidGroup.h"
#include "Bullet.h"


namespace Asteroid
{
	static float degreesToRadians(float degrees) {
		return degrees * 3.1415926535f / 180.0f;
	}

	Asteroid::Bullet::Bullet(const float angle, sf::Vector2f position)
	{
		const float velocity = -800;

		this->drawable = std::make_shared<sf::CircleShape>();
		auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

		circle->setOutlineColor(sf::Color::White);
		circle->setFillColor(sf::Color::Black);
		circle->setOutlineThickness(2);
		circle->setPointCount(4);
		circle->setRadius(3);

		setPosition(position);
		setOrigin(3, 3);
		positionVelocity = { velocity * sin(-degreesToRadians(angle)), velocity * cos(-degreesToRadians(angle)) };
	}

	class DetachChildEvent : public harmony::core::Event_t
	{
	public:
		DetachChildEvent(const std::shared_ptr<harmony::core::SceneNode> node) :
			m_node(node) {}

		void execute() override {
			m_node->detachChild();
		}

	private:
		std::shared_ptr<harmony::core::SceneNode> m_node;
	};

	void Asteroid::Bullet::onUpdate(const sf::Time& time, harmony::core::EventPool& eventPool)
	{
		const sf::FloatRect field = harmony::utilities::getViewBounds(currentScene->view);

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
		if (clock.getElapsedTime().asSeconds() > 2) {
			eventPool.addEvent(harmony::utilities::create<DetachChildEvent>(std::static_pointer_cast<SceneNode>(shared_from_this())));
			clock.restart();
		}

		auto asteroidGroup = harmony::utilities::find<harmony::core::SceneNode>(AsteroidGroupUniqueId);

		for (auto node : asteroidGroup->children) {
			if (intersect(node)) {
				eventPool.addEvent(harmony::utilities::create<harmony::Event::DetachNode>(node));
				eventPool.addEvent(harmony::utilities::create<harmony::Event::DetachNode>(std::static_pointer_cast<SceneNode>(shared_from_this())));
				return;
			}
		}

		const std::vector<sf::Color> OutlineColorStages = {
			{ 255,   255,    255,    255 },
			{ 255,   255,    255,    0 },
		};

		std::static_pointer_cast<sf::CircleShape>(drawable)->setOutlineColor(harmony::utilities::getInterpolatedColor(clock.getElapsedTime().asSeconds(), 2, OutlineColorStages));

	}
}
