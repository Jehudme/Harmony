#include <Harmony/fwd.h>
#include <Harmony/Script.h>
#include <Harmony/Circle.h>
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

namespace Sandbox::PhysicsPropertiesDemo
{
	using namespace Harmony::Components;

	/// @brief Script for a bouncy ball (high restitution)
	struct BouncyBallScript : public Script
	{
	public:
		BouncyBallScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<PhysicsBody>(getEntityId());
			
			// Create a circle fixture with high restitution (bounciness)
			FixtureProperties props;
			props.density = 1.0f;
			props.friction = 0.3f;
			props.restitution = 0.9f;  // Very bouncy!
			props.isSensor = false;
			
			physicsBody_->createCircleFixture(25.0f, props);
		}

		void onDestroy() {}
		void onPreUpdate() {}
		void onPostUpdate() {}

	private:
		Harmony::Utilities::OptionalReference<PhysicsBody> physicsBody_;
	};

	/// @brief Script for a sticky ball (no bounce, high friction)
	struct StickyBallScript : public Script
	{
	public:
		StickyBallScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<PhysicsBody>(getEntityId());
			
			// Create a circle fixture with no bounce and high friction
			FixtureProperties props;
			props.density = 1.0f;
			props.friction = 1.0f;     // Maximum friction
			props.restitution = 0.0f;  // No bounce
			props.isSensor = false;
			
			physicsBody_->createCircleFixture(25.0f, props);
		}

		void onDestroy() {}
		void onPreUpdate() {}
		void onPostUpdate() {}

	private:
		Harmony::Utilities::OptionalReference<PhysicsBody> physicsBody_;
	};

	/// @brief Script for a frictionless box (slides easily)
	struct FrictionlessBoxScript : public Script
	{
	public:
		FrictionlessBoxScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<PhysicsBody>(getEntityId());
			
			// Create a box fixture with no friction (like ice)
			FixtureProperties props;
			props.density = 1.0f;
			props.friction = 0.0f;     // No friction - slides like ice
			props.restitution = 0.5f;
			props.isSensor = false;
			
			physicsBody_->createBoxFixture(50.0f, 50.0f, props);
		}

		void onDestroy() {}
		void onPreUpdate() {}
		void onPostUpdate() {}

	private:
		Harmony::Utilities::OptionalReference<PhysicsBody> physicsBody_;
	};

	/// @brief Script for a heavy box (high density)
	struct HeavyBoxScript : public Script
	{
	public:
		HeavyBoxScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<PhysicsBody>(getEntityId());
			
			// Create a box fixture with high density (heavy)
			FixtureProperties props;
			props.density = 10.0f;     // 10x heavier than normal
			props.friction = 0.3f;
			props.restitution = 0.2f;
			props.isSensor = false;
			
			physicsBody_->createBoxFixture(50.0f, 50.0f, props);
		}

		void onDestroy() {}
		void onPreUpdate() {}
		void onPostUpdate() {}

	private:
		Harmony::Utilities::OptionalReference<PhysicsBody> physicsBody_;
	};

	/// @brief Script for ground platform (medium friction)
	struct GroundScript : public Script
	{
	public:
		GroundScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<PhysicsBody>(getEntityId());
			
			// Create a box fixture with medium friction
			FixtureProperties props;
			props.density = 1.0f;
			props.friction = 0.5f;
			props.restitution = 0.0f;
			props.isSensor = false;
			
			physicsBody_->createBoxFixture(1000.0f, 50.0f, props);
		}

		void onDestroy() {}
		void onPreUpdate() {}
		void onPostUpdate() {}

	private:
		Harmony::Utilities::OptionalReference<PhysicsBody> physicsBody_;
	};

	/// @brief Script for a sloped ground (to test friction on slopes)
	struct SlopedGroundScript : public Script
	{
	public:
		SlopedGroundScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate()
		{
			physicsBody_ = getScene().getComponent<PhysicsBody>(getEntityId());
			
			// Create a sloped box fixture with low friction
			FixtureProperties props;
			props.density = 1.0f;
			props.friction = 0.2f;     // Low friction slope
			props.restitution = 0.0f;
			props.isSensor = false;
			
			physicsBody_->createBoxFixture(300.0f, 50.0f, props);
		}

		void onDestroy() {}
		void onPreUpdate() {}
		void onPostUpdate() {}

	private:
		Harmony::Utilities::OptionalReference<PhysicsBody> physicsBody_;
	};
}

HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsPropertiesDemo::BouncyBallScript, bouncy_ball_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsPropertiesDemo::StickyBallScript, sticky_ball_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsPropertiesDemo::FrictionlessBoxScript, frictionless_box_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsPropertiesDemo::HeavyBoxScript, heavy_box_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsPropertiesDemo::GroundScript, ground_script)
HARMONY_REGISTER_SCRIPT(Sandbox::PhysicsPropertiesDemo::SlopedGroundScript, sloped_ground_script)
