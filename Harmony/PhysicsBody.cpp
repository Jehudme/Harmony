#include "pch.h"
#include "PhysicsBody.h"
#include "PhysicsWorld.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"

HARMONY_REGISTER_COMPONENT(Harmony::Components::PhysicsBody, PhysicsBody)

namespace Harmony::Components
{
	PhysicsBody::PhysicsBody(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: body_(nullptr), world_(nullptr)
	{
		// Get the PhysicsWorld component from the scene
		// The PhysicsWorld should be a global component
		try
		{
			PhysicsWorld& physicsWorld = scene.getGlobalComponent<PhysicsWorld>();
			world_ = physicsWorld.get();
		}
		catch (...)
		{
			HARMONY_ERROR("PhysicsBody component requires a PhysicsWorld global component in the scene");
			return;
		}

		// Get body configuration
		b2BodyDef bodyDef;

		// Position
		float x = 0.0f, y = 0.0f;
		if (std::optional<float> posX = configuration.get<float>({ "position", "x" }))
			x = posX.value();
		else
			HARMONY_WARN("PhysicsBody component missing position x configuration, using default 0.0");

		if (std::optional<float> posY = configuration.get<float>({ "position", "y" }))
			y = posY.value();
		else
			HARMONY_WARN("PhysicsBody component missing position y configuration, using default 0.0");

		bodyDef.position.Set(x, y);

		// Angle
		if (std::optional<float> angle = configuration.get<float>({ "angle" }))
			bodyDef.angle = angle.value();
		else
			HARMONY_WARN("PhysicsBody component missing angle configuration, using default 0.0");

		// Body type
		if (std::optional<std::string> typeStr = configuration.get<std::string>({ "type" }))
		{
			if (typeStr.value() == "static")
				bodyDef.type = b2_staticBody;
			else if (typeStr.value() == "kinematic")
				bodyDef.type = b2_kinematicBody;
			else if (typeStr.value() == "dynamic")
				bodyDef.type = b2_dynamicBody;
			else
			{
				HARMONY_WARN("PhysicsBody component has invalid type '{}', using dynamic", typeStr.value());
				bodyDef.type = b2_dynamicBody;
			}
		}
		else
		{
			HARMONY_WARN("PhysicsBody component missing type configuration, using dynamic");
			bodyDef.type = b2_dynamicBody;
		}

		// Linear velocity
		if (std::optional<float> velX = configuration.get<float>({ "linear_velocity", "x" }))
			bodyDef.linearVelocity.x = velX.value();

		if (std::optional<float> velY = configuration.get<float>({ "linear_velocity", "y" }))
			bodyDef.linearVelocity.y = velY.value();

		// Angular velocity
		if (std::optional<float> angVel = configuration.get<float>({ "angular_velocity" }))
			bodyDef.angularVelocity = angVel.value();

		// Fixed rotation
		if (std::optional<bool> fixedRot = configuration.get<bool>({ "fixed_rotation" }))
			bodyDef.fixedRotation = fixedRot.value();

		// Linear damping
		if (std::optional<float> linDamp = configuration.get<float>({ "linear_damping" }))
			bodyDef.linearDamping = linDamp.value();

		// Angular damping
		if (std::optional<float> angDamp = configuration.get<float>({ "angular_damping" }))
			bodyDef.angularDamping = angDamp.value();

		// Create the body
		if (world_)
		{
			PhysicsWorld& physicsWorld = scene.getGlobalComponent<PhysicsWorld>();
			body_ = physicsWorld.createBody(&bodyDef);

			if (!body_)
			{
				HARMONY_ERROR("Failed to create physics body");
				return;
			}

			HARMONY_INFO("PhysicsBody component created at position ({}, {})", x, y);
		}
	}

	PhysicsBody::~PhysicsBody()
	{
		if (body_ && world_)
		{
			world_->DestroyBody(body_);
			body_ = nullptr;
		}
	}

	PhysicsBody::PhysicsBody(PhysicsBody&& other) noexcept
		: body_(other.body_), world_(other.world_)
	{
		other.body_ = nullptr;
		other.world_ = nullptr;
	}

	PhysicsBody& PhysicsBody::operator=(PhysicsBody&& other) noexcept
	{
		if (this != &other)
		{
			// Clean up existing body
			if (body_ && world_)
			{
				world_->DestroyBody(body_);
			}

			body_ = other.body_;
			world_ = other.world_;

			other.body_ = nullptr;
			other.world_ = nullptr;
		}
		return *this;
	}

	void PhysicsBody::setTransform(const b2Vec2& position, float angle)
	{
		if (body_)
		{
			body_->SetTransform(position, angle);
		}
	}

	b2Vec2 PhysicsBody::getPosition() const
	{
		if (body_)
		{
			return body_->GetPosition();
		}
		return b2Vec2(0.0f, 0.0f);
	}

	float PhysicsBody::getAngle() const
	{
		if (body_)
		{
			return body_->GetAngle();
		}
		return 0.0f;
	}

	void PhysicsBody::setLinearVelocity(const b2Vec2& velocity)
	{
		if (body_)
		{
			body_->SetLinearVelocity(velocity);
		}
	}

	b2Vec2 PhysicsBody::getLinearVelocity() const
	{
		if (body_)
		{
			return body_->GetLinearVelocity();
		}
		return b2Vec2(0.0f, 0.0f);
	}

	void PhysicsBody::setAngularVelocity(float omega)
	{
		if (body_)
		{
			body_->SetAngularVelocity(omega);
		}
	}

	float PhysicsBody::getAngularVelocity() const
	{
		if (body_)
		{
			return body_->GetAngularVelocity();
		}
		return 0.0f;
	}

	void PhysicsBody::applyForce(const b2Vec2& force, const b2Vec2& point, bool wake)
	{
		if (body_)
		{
			body_->ApplyForce(force, point, wake);
		}
	}

	void PhysicsBody::applyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point, bool wake)
	{
		if (body_)
		{
			body_->ApplyLinearImpulse(impulse, point, wake);
		}
	}

	void PhysicsBody::applyTorque(float torque, bool wake)
	{
		if (body_)
		{
			body_->ApplyTorque(torque, wake);
		}
	}

	void PhysicsBody::applyAngularImpulse(float impulse, bool wake)
	{
		if (body_)
		{
			body_->ApplyAngularImpulse(impulse, wake);
		}
	}

	float PhysicsBody::getMass() const
	{
		if (body_)
		{
			return body_->GetMass();
		}
		return 0.0f;
	}

	float PhysicsBody::getInertia() const
	{
		if (body_)
		{
			return body_->GetInertia();
		}
		return 0.0f;
	}

	b2Fixture* PhysicsBody::createFixture(const b2Shape* shape, float density)
	{
		if (body_)
		{
			b2FixtureDef fixtureDef;
			fixtureDef.shape = shape;
			fixtureDef.density = density;
			return body_->CreateFixture(&fixtureDef);
		}
		return nullptr;
	}

	b2Fixture* PhysicsBody::createBoxFixture(float halfWidth, float halfHeight, float density)
	{
		if (body_)
		{
			b2PolygonShape boxShape;
			boxShape.SetAsBox(halfWidth, halfHeight);
			return createFixture(&boxShape, density);
		}
		return nullptr;
	}

	void PhysicsBody::setType(b2BodyType type)
	{
		if (body_)
		{
			body_->SetType(type);
		}
	}

	b2BodyType PhysicsBody::getType() const
	{
		if (body_)
		{
			return body_->GetType();
		}
		return b2_staticBody;
	}

	void PhysicsBody::setAwake(bool awake)
	{
		if (body_)
		{
			body_->SetAwake(awake);
		}
	}

	bool PhysicsBody::isAwake() const
	{
		if (body_)
		{
			return body_->IsAwake();
		}
		return false;
	}

	void PhysicsBody::setFixedRotation(bool fixed)
	{
		if (body_)
		{
			body_->SetFixedRotation(fixed);
		}
	}

	bool PhysicsBody::hasFixedRotation() const
	{
		if (body_)
		{
			return body_->IsFixedRotation();
		}
		return false;
	}
}
