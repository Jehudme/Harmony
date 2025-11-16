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
			
			// Create a box fixture for the ground (full width and height)
			physicsBody_->createBoxFixture(5000.0f, 50.0f, 1.0f);
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
			
			// Create a box fixture for collision (full width and height)
			physicsBody_->createBoxFixture(50.0f, 50.0f, 1.0f);
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

	/// @brief Script for a falling circle
	struct FallingCircleScript : public Harmony::Components::Script
	{
	public:
		FallingCircleScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<Harmony::Components::PhysicsBody>(getEntityId());
			
			// Create a circle fixture for collision
			physicsBody_->createCircleFixture(25.0f, 1.0f);
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

HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsDemo::GroundScript, physics_ground_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsDemo::FallingBoxScript, physics_falling_box_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsDemo::FallingCircleScript, physics_falling_circle_script)