#include "pch.h"
#include "PhysicsWorld.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"

HARMONY_REGISTER_COMPONENT(Harmony::Components::PhysicsWorld, PhysicsWorld)

namespace Harmony::Components
{
	// Helper class for AABB query callback
	class AABBQueryCallback : public b2QueryCallback
	{
	public:
		explicit AABBQueryCallback(std::function<bool(b2Fixture*)> callback)
			: callback_(std::move(callback)) {}

		bool ReportFixture(b2Fixture* fixture) override
		{
			return callback_(fixture);
		}

	private:
		std::function<bool(b2Fixture*)> callback_;
	};

	// Helper class for ray cast callback
	class RayCastCallback : public b2RayCastCallback
	{
	public:
		explicit RayCastCallback(std::function<float(b2Fixture*, const b2Vec2&, const b2Vec2&, float)> callback)
			: callback_(std::move(callback)) {}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			return callback_(fixture, point, normal, fraction);
		}

	private:
		std::function<float(b2Fixture*, const b2Vec2&, const b2Vec2&, float)> callback_;
	};

	PhysicsWorld::PhysicsWorld(const Utilities::Configuration& configuration, Scenes::Scene& scene)
	{
		// Get pixels-to-meters ratio from configuration (default: 30.0)
		// This means 30 pixels = 1 meter, which is a good scale for Box2D
		pixelsToMeters_ = configuration.get<float>({ "pixels_to_meters" }).value_or(30.0f);

		// Get gravity from configuration
		// Default is Earth's gravity: 9.81 m/s² downward
		float gravityX = 0.0f, gravityY = 9.81f;

		if (std::optional<float> gx = configuration.get<float>({ "gravity", "x" }))
			gravityX = gx.value();
		else
			HARMONY_WARN("PhysicsWorld component missing gravity x configuration, using default 0.0");

		if (std::optional<float> gy = configuration.get<float>({ "gravity", "y" }))
			gravityY = gy.value();
		else
			HARMONY_WARN("PhysicsWorld component missing gravity y configuration, using default 9.81");

		// Create the world with gravity in m/s²
		b2Vec2 gravity(gravityX, gravityY);
		world_ = std::unique_ptr<b2World, WorldDeleter>(new b2World(gravity));

		HARMONY_INFO("PhysicsWorld component created with gravity ({}, {}) m/s², scale {} pixels/meter", 
			gravityX, gravityY, pixelsToMeters_);
	}

	PhysicsWorld::~PhysicsWorld() = default;

	PhysicsWorld::PhysicsWorld(PhysicsWorld&& other) noexcept
		: world_(std::move(other.world_)), pixelsToMeters_(other.pixelsToMeters_)
	{
	}

	PhysicsWorld& PhysicsWorld::operator=(PhysicsWorld&& other) noexcept
	{
		if (this != &other)
		{
			world_ = std::move(other.world_);
			pixelsToMeters_ = other.pixelsToMeters_;
		}
		return *this;
	}

	void PhysicsWorld::step(float timeStep, int32_t velocityIterations, int32_t positionIterations)
	{
		if (world_)
		{
			world_->Step(timeStep, velocityIterations, positionIterations);
		}
	}

	b2Body* PhysicsWorld::createBody(const b2BodyDef* def)
	{
		if (world_)
		{
			return world_->CreateBody(def);
		}
		return nullptr;
	}

	void PhysicsWorld::destroyBody(b2Body* body)
	{
		if (world_ && body)
		{
			world_->DestroyBody(body);
		}
	}

	void PhysicsWorld::setGravity(const b2Vec2& gravity)
	{
		if (world_)
		{
			world_->SetGravity(gravity);
		}
	}

	b2Vec2 PhysicsWorld::getGravity() const
	{
		if (world_)
		{
			return world_->GetGravity();
		}
		return b2Vec2(0.0f, 0.0f);
	}

	void PhysicsWorld::queryAABB(std::function<bool(b2Fixture*)> callback, const b2AABB& aabb)
	{
		if (world_)
		{
			AABBQueryCallback queryCallback(std::move(callback));
			world_->QueryAABB(&queryCallback, aabb);
		}
	}

	void PhysicsWorld::rayCast(std::function<float(b2Fixture*, const b2Vec2&, const b2Vec2&, float)> callback,
		const b2Vec2& point1, const b2Vec2& point2)
	{
		if (world_)
		{
			RayCastCallback rayCastCallback(std::move(callback));
			world_->RayCast(&rayCastCallback, point1, point2);
		}
	}

	int32_t PhysicsWorld::getBodyCount() const
	{
		if (world_)
		{
			return world_->GetBodyCount();
		}
		return 0;
	}

	void PhysicsWorld::setContactListener(b2ContactListener* listener)
	{
		if (world_)
		{
			world_->SetContactListener(listener);
		}
	}
}
