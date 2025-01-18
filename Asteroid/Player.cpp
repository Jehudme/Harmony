#include "Player.h"
#include "Bullet.h"
#include "Harmony/Scene.h"

namespace Asteroid
{
	void Asteroid::Player::onEnterCurrent(Harmony::Core::Scene& scene)
	{
		this->drawable = std::make_shared<sf::CircleShape>();
		auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

		circle->setOutlineColor(sf::Color::White);
		circle->setFillColor(sf::Color::Black);
		circle->setOutlineThickness(3);
		circle->setPointCount(3);
		circle->setRadius(10);

		setPosition(scene.view.getCenter());
		setOrigin(10, 0);

	}

	void Asteroid::Player::onExitCurrent(Harmony::Core::Scene& scene)
	{
	}

	static float degreesToRadians(float degrees) {
		return degrees * 3.1415926535f / 180.0f;
	}

	class AttachChildEvent : public Harmony::Core::Event_t
	{
	public:
		AttachChildEvent(
			const std::shared_ptr<Harmony::Core::SceneNode> parent,
			const std::shared_ptr<Harmony::Core::SceneNode> child) :
			m_parent(parent), m_child(child) {}

		void execute() override {
			m_parent->attachChild(m_child);
		}

	private:
		std::shared_ptr<Harmony::Core::SceneNode> m_parent;
		std::shared_ptr<Harmony::Core::SceneNode> m_child;
	};

	void Asteroid::Player::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
	{
		const float deltaTime = time.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bulletClock.getElapsedTime().asSeconds() > .5f) {
			eventPool.addEvent(Harmony::Core::Object::create<AttachChildEvent>(
				std::static_pointer_cast<SceneNode>(parentNode->shared_from_this()),
				Harmony::Core::Object::create<Bullet>(getRotation(), getGlobalPosition()))
			);
			bulletClock.restart();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			rotate(-120 * deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			rotate(120 * deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { acceleration = -140; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { acceleration = +140; }

		else {
			if (velocity < -10) { acceleration = 140; }
			else if (velocity > 10) { acceleration = -140; }
			else { acceleration = 0; }
		}

		velocity += acceleration * deltaTime;

		if (velocity > 300) { velocity = 300; }
		else if (velocity < -300) { velocity = -300; }

		if (velocity < -5 && velocity > 5) { velocity = 0; }

		positionVelocity = { velocity * sin(-degreesToRadians(getRotation())), velocity * cos(-degreesToRadians(getRotation())) };
	}
}
