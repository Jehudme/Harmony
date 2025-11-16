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

		// Fixture properties
		if (std::optional<float> density = configuration.get<float>({ "fixture", "density" }))
			fixtureProperties_.density = density.value();

		if (std::optional<float> friction = configuration.get<float>({ "fixture", "friction" }))
			fixtureProperties_.friction = friction.value();

		if (std::optional<float> restitution = configuration.get<float>({ "fixture", "restitution" }))
			fixtureProperties_.restitution = restitution.value();

		if (std::optional<bool> isSensor = configuration.get<bool>({ "fixture", "is_sensor" }))
			fixtureProperties_.isSensor = isSensor.value();

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

	b2Fixture* PhysicsBody::createFixture(const b2Shape* shape, const FixtureProperties& properties)
	{
		if (body_)
		{
			b2FixtureDef fixtureDef;
			fixtureDef.shape = shape;
			fixtureDef.density = properties.density;
			fixtureDef.friction = properties.friction;
			fixtureDef.restitution = properties.restitution;
			fixtureDef.isSensor = properties.isSensor;
			return body_->CreateFixture(&fixtureDef);
		}
		return nullptr;
	}

	b2Fixture* PhysicsBody::createFixture(const b2Shape* shape, float density)
	{
		FixtureProperties properties = fixtureProperties_;
		properties.density = density;
		return createFixture(shape, properties);
	}

	b2Fixture* PhysicsBody::createBoxFixture(float width, float height, const FixtureProperties& properties)
	{
		if (body_)
		{
			b2PolygonShape boxShape;
			// Box2D's SetAsBox takes half-widths, so we divide by 2
			boxShape.SetAsBox(width / 2.0f, height / 2.0f);
			return createFixture(&boxShape, properties);
		}
		return nullptr;
	}

	b2Fixture* PhysicsBody::createBoxFixture(float width, float height, float density)
	{
		FixtureProperties properties = fixtureProperties_;
		properties.density = density;
		return createBoxFixture(width, height, properties);
	}

	b2Fixture* PhysicsBody::createCircleFixture(float radius, const FixtureProperties& properties, const b2Vec2& center)
	{
		if (body_)
		{
			b2CircleShape circleShape;
			circleShape.m_radius = radius;
			circleShape.m_p = center;
			return createFixture(&circleShape, properties);
		}
		return nullptr;
	}

	b2Fixture* PhysicsBody::createCircleFixture(float radius, float density, const b2Vec2& center)
	{
		FixtureProperties properties = fixtureProperties_;
		properties.density = density;
		return createCircleFixture(radius, properties, center);
	}

	b2Fixture* PhysicsBody::createPolygonFixture(const std::vector<b2Vec2>& points, const FixtureProperties& properties)
	{
		if (!body_)
		{
			return nullptr;
		}

		// Box2D polygons must have at least 3 points and at most 8 points
		if (points.size() < 3 || points.size() > 8)
		{
			HARMONY_ERROR("PhysicsBody::createPolygonFixture: polygon must have 3-8 points, got {}", points.size());
			return nullptr;
		}

		b2PolygonShape polygonShape;
		polygonShape.Set(points.data(), static_cast<int32>(points.size()));
		return createFixture(&polygonShape, properties);
	}

	b2Fixture* PhysicsBody::createPolygonFixture(const std::vector<b2Vec2>& points, float density)
	{
		FixtureProperties properties = fixtureProperties_;
		properties.density = density;
		return createPolygonFixture(points, properties);
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

	void PhysicsBody::setFixtureProperties(const FixtureProperties& properties)
	{
		fixtureProperties_ = properties;
	}

	const FixtureProperties& PhysicsBody::getFixtureProperties() const
	{
		return fixtureProperties_;
	}

	void PhysicsBody::setFriction(float friction)
	{
		fixtureProperties_.friction = friction;
	}

	float PhysicsBody::getFriction() const
	{
		return fixtureProperties_.friction;
	}

	void PhysicsBody::setRestitution(float restitution)
	{
		fixtureProperties_.restitution = restitution;
	}

	float PhysicsBody::getRestitution() const
	{
		return fixtureProperties_.restitution;
	}

	void PhysicsBody::setDensity(float density)
	{
		fixtureProperties_.density = density;
	}

	float PhysicsBody::getDensity() const
	{
		return fixtureProperties_.density;
	}

	void PhysicsBody::setIsSensor(bool isSensor)
	{
		fixtureProperties_.isSensor = isSensor;
	}

	bool PhysicsBody::getIsSensor() const
	{
		return fixtureProperties_.isSensor;
	}
}
