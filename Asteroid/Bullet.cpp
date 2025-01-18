#include "Bullet.h"


namespace Asteroid
{
	static float degreesToRadians(float degrees) {
		return degrees * 3.1415926535f / 180.0f;
	}

	Asteroid::Bullet::Bullet(const float angle, sf::Vector2f position)
	{
		const float velocity = -500;

		this->drawable = std::make_shared<sf::CircleShape>();
		auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

		circle->setOutlineColor(sf::Color::White);
		circle->setFillColor(sf::Color::Black);
		circle->setOutlineThickness(2);
		circle->setPointCount(5);
		circle->setRadius(3);

		setPosition(position);
		setOrigin(3, 3);
		positionVelocity = { velocity * sin(-degreesToRadians(angle)), velocity * cos(-degreesToRadians(angle)) };
	}

	class DetachChildEvent : public Harmony::Core::Event_t
	{
	public:
		DetachChildEvent(const std::shared_ptr<Harmony::Core::SceneNode> node) :
			m_node(node) {}

		void execute() override {
			m_node->detachChild();
		}

	private:
		std::shared_ptr<Harmony::Core::SceneNode> m_node;
	};

	void Asteroid::Bullet::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
	{
		if (clock.getElapsedTime().asSeconds() > 2) {
			eventPool.addEvent(Harmony::Core::Object::create<DetachChildEvent>(std::static_pointer_cast<SceneNode>(shared_from_this())));
			clock.restart();
		}
	}
}
