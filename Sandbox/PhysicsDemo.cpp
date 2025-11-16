#include <Harmony/fwd.h>
#include <Harmony/Script.h>
#include <Harmony/Rectangle.h>
#include <Harmony/Engine.h>
#include <Harmony/Transform.h>
#include <Harmony/PhysicsBody.h>
#include <Harmony/PhysicsWorld.h>
#include <Harmony/ComponentManagement.h>
#include <Harmony/InputManager.h>
#include <Harmony/OptionalReference.h>

#include <SFML/System/InputStream.hpp>
#include <box2d/box2d.h>

namespace Sandbox::PhysicsDemo
{
	/// @brief Script for a box that can be pushed with arrow keys
	struct BoxScript : public Harmony::Components::Script
	{
	public:
		BoxScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			transform_ = getScene().getComponent<Harmony::Components::Transform>(getEntityId());
			physicsBody_ = getScene().getComponent<Harmony::Components::PhysicsBody>(getEntityId());
		}

		void onDestroy()
		{
		}

		void onPreUpdate()
		{
			// Apply forces based on keyboard input
			b2Vec2 force(0.0f, 0.0f);
			const float forceAmount = 5000.0f;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				force.x -= forceAmount;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				force.x += forceAmount;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				force.y -= forceAmount;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				force.y += forceAmount;

			// Apply force at center of mass
			if (force.x != 0.0f || force.y != 0.0f) {
				b2Vec2 position = physicsBody_->getPosition();
				physicsBody_->applyForce(force, position, true);
			}
		}

		void onPostUpdate()
		{
		}

	private:
		Harmony::Utilities::OptionalReference<Harmony::Components::Transform> transform_;
		Harmony::Utilities::OptionalReference<Harmony::Components::PhysicsBody> physicsBody_;
	};

	/// @brief Script for a static ground platform
	struct GroundScript : public Harmony::Components::Script
	{
	public:
		GroundScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			// Just setup, physics handles rest
			physicsBody_ = getScene().getComponent<Harmony::Components::PhysicsBody>(getEntityId());
			
			// Create a box fixture for the ground
			physicsBody_->createBoxFixture(400.0f, 10.0f, 1.0f);
		}

		void onDestroy()
		{
		}

		void onPreUpdate()
		{
		}

		void onPostUpdate()
		{
		}

	private:
		Harmony::Utilities::OptionalReference<Harmony::Components::PhysicsBody> physicsBody_;
	};

	/// @brief Script for a falling dynamic box
	struct FallingBoxScript : public Harmony::Components::Script
	{
	public:
		FallingBoxScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<Harmony::Components::PhysicsBody>(getEntityId());
			
			// Create a box fixture for collision
			physicsBody_->createBoxFixture(25.0f, 25.0f, 1.0f);
		}

		void onDestroy()
		{
		}

		void onPreUpdate()
		{
		}

		void onPostUpdate()
		{
		}

	private:
		Harmony::Utilities::OptionalReference<Harmony::Components::PhysicsBody> physicsBody_;
	};
}

HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsDemo::BoxScript, physics_box_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsDemo::GroundScript, physics_ground_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsDemo::FallingBoxScript, physics_falling_box_script)
