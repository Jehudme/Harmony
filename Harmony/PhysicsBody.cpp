#include "pch.h"
#include "PhysicsBody.h"
#include "PhysicsWorld.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include "Transform.h"
#include "Engine.h"
#include "TaskManagement.h"
#include "UtilityTask.h"
#include <limits>

HARMONY_REGISTER_COMPONENT(Harmony::Components::PhysicsBody, PhysicsBody)

namespace Harmony::Components
{
	PhysicsBody::PhysicsBody(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: body_(nullptr), world_(nullptr), physicsWorld_(nullptr)
	{
		// Get the PhysicsWorld component from the scene
		// The PhysicsWorld should be a global component
		try
		{
			PhysicsWorld& physicsWorld = scene.getGlobalComponent<PhysicsWorld>();
			world_ = physicsWorld.get();
			physicsWorld_ = &physicsWorld;
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

			// Submit a lambda task to update the Transform component origin before the next frame
			scene.engine.taskManagement->submit(
				std::make_unique<Tasks::LambdaTask>([this]() {
					try {
						// Only proceed if scene and entityId are set (should be set by Scene after construction)
						if (!scene_.has_value()) {
							HARMONY_ERROR("PhysicsBody: scene reference not set when trying to update Transform origin");
							return;
						}

						// Calculate the origin position within the bounding box
						b2Vec2 origin = getOriginInBoundingBox();
						
						// Get the Transform component for this entity
						Transform& transform = scene_->get().getComponent<Transform>(entityId_);
						
						// Convert from meters to pixels before setting origin
						if (physicsWorld_) {
							float originXPixels = physicsWorld_->metersToPixels(origin.x);
							float originYPixels = physicsWorld_->metersToPixels(origin.y);
							transform.setOrigin(originXPixels, originYPixels);
							
							HARMONY_DEBUG("Updated Transform origin to ({}, {}) pixels", originXPixels, originYPixels);
						}
					}
					catch (const std::exception& e) {
						HARMONY_ERROR("Failed to update Transform origin: {}", e.what());
					}
				}, 1) // Priority 1 to execute before next frame
			);
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
		: body_(other.body_), world_(other.world_), physicsWorld_(other.physicsWorld_),
		  entityId_(other.entityId_), scene_(other.scene_)
	{
		other.body_ = nullptr;
		other.world_ = nullptr;
		other.physicsWorld_ = nullptr;
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
			physicsWorld_ = other.physicsWorld_;
			entityId_ = other.entityId_;
			scene_ = other.scene_;

			other.body_ = nullptr;
			other.world_ = nullptr;
			other.physicsWorld_ = nullptr;
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
		if (body_ && physicsWorld_)
		{
			// Convert dimensions from pixels to meters
			float widthMeters = physicsWorld_->pixelsToMeters(width);
			float heightMeters = physicsWorld_->pixelsToMeters(height);
			
			b2PolygonShape boxShape;
			// Box2D's SetAsBox takes half-widths, so we divide by 2
			boxShape.SetAsBox(widthMeters / 2.0f, heightMeters / 2.0f);
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
		if (body_ && physicsWorld_)
		{
			// Convert radius from pixels to meters
			float radiusMeters = physicsWorld_->pixelsToMeters(radius);
			
			// Convert center from pixels to meters
			b2Vec2 centerMeters(
				physicsWorld_->pixelsToMeters(center.x),
				physicsWorld_->pixelsToMeters(center.y)
			);
			
			b2CircleShape circleShape;
			circleShape.m_radius = radiusMeters;
			circleShape.m_p = centerMeters;
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
		if (!body_ || !physicsWorld_)
		{
			return nullptr;
		}

		// Box2D polygons must have at least 3 points and at most 8 points
		if (points.size() < 3 || points.size() > 8)
		{
			HARMONY_ERROR("PhysicsBody::createPolygonFixture: polygon must have 3-8 points, got {}", points.size());
			return nullptr;
		}

		// Convert points from pixels to meters
		std::vector<b2Vec2> pointsMeters;
		pointsMeters.reserve(points.size());
		for (const b2Vec2& point : points)
		{
			pointsMeters.push_back(b2Vec2(
				physicsWorld_->pixelsToMeters(point.x),
				physicsWorld_->pixelsToMeters(point.y)
			));
		}

		b2PolygonShape polygonShape;
		polygonShape.Set(pointsMeters.data(), static_cast<int32>(pointsMeters.size()));
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

	b2Vec2 PhysicsBody::getBoundingBox() const
	{
		if (!body_)
		{
			return b2Vec2(0.0f, 0.0f);
		}

		// Initialize min and max with extreme values
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float maxY = std::numeric_limits<float>::lowest();

		bool hasFixtures = false;

		// Iterate through all fixtures and their shapes to find all vertices
		for (b2Fixture* fixture = body_->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
		{
			hasFixtures = true;
			const b2Shape* shape = fixture->GetShape();

			switch (shape->GetType())
			{
			case b2Shape::e_circle:
			{
				const b2CircleShape* circle = static_cast<const b2CircleShape*>(shape);
				b2Vec2 center = circle->m_p;
				float radius = circle->m_radius;

				minX = std::min(minX, center.x - radius);
				minY = std::min(minY, center.y - radius);
				maxX = std::max(maxX, center.x + radius);
				maxY = std::max(maxY, center.y + radius);
				break;
			}
			case b2Shape::e_polygon:
			{
				const b2PolygonShape* polygon = static_cast<const b2PolygonShape*>(shape);
				for (int32 i = 0; i < polygon->m_count; ++i)
				{
					const b2Vec2& vertex = polygon->m_vertices[i];
					minX = std::min(minX, vertex.x);
					minY = std::min(minY, vertex.y);
					maxX = std::max(maxX, vertex.x);
					maxY = std::max(maxY, vertex.y);
				}
				break;
			}
			case b2Shape::e_edge:
			{
				const b2EdgeShape* edge = static_cast<const b2EdgeShape*>(shape);
				minX = std::min(minX, std::min(edge->m_vertex1.x, edge->m_vertex2.x));
				minY = std::min(minY, std::min(edge->m_vertex1.y, edge->m_vertex2.y));
				maxX = std::max(maxX, std::max(edge->m_vertex1.x, edge->m_vertex2.x));
				maxY = std::max(maxY, std::max(edge->m_vertex1.y, edge->m_vertex2.y));
				break;
			}
			case b2Shape::e_chain:
			{
				const b2ChainShape* chain = static_cast<const b2ChainShape*>(shape);
				for (int32 i = 0; i < chain->m_count; ++i)
				{
					const b2Vec2& vertex = chain->m_vertices[i];
					minX = std::min(minX, vertex.x);
					minY = std::min(minY, vertex.y);
					maxX = std::max(maxX, vertex.x);
					maxY = std::max(maxY, vertex.y);
				}
				break;
			}
			default:
				break;
			}
		}

		if (!hasFixtures)
		{
			// No fixtures, return zero bounding box
			return b2Vec2(0.0f, 0.0f);
		}

		// Return the size of the bounding box
		float width = maxX - minX;
		float height = maxY - minY;
		return b2Vec2(width, height);
	}

	b2Vec2 PhysicsBody::getOriginInBoundingBox() const
	{
		if (!body_)
		{
			return b2Vec2(0.0f, 0.0f);
		}

		// Initialize min and max with extreme values
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float maxY = std::numeric_limits<float>::lowest();

		bool hasFixtures = false;

		// Iterate through all fixtures and their shapes to find all vertices
		for (b2Fixture* fixture = body_->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
		{
			hasFixtures = true;
			const b2Shape* shape = fixture->GetShape();

			switch (shape->GetType())
			{
			case b2Shape::e_circle:
			{
				const b2CircleShape* circle = static_cast<const b2CircleShape*>(shape);
				b2Vec2 center = circle->m_p;
				float radius = circle->m_radius;

				minX = std::min(minX, center.x - radius);
				minY = std::min(minY, center.y - radius);
				maxX = std::max(maxX, center.x + radius);
				maxY = std::max(maxY, center.y + radius);
				break;
			}
			case b2Shape::e_polygon:
			{
				const b2PolygonShape* polygon = static_cast<const b2PolygonShape*>(shape);
				for (int32 i = 0; i < polygon->m_count; ++i)
				{
					const b2Vec2& vertex = polygon->m_vertices[i];
					minX = std::min(minX, vertex.x);
					minY = std::min(minY, vertex.y);
					maxX = std::max(maxX, vertex.x);
					maxY = std::max(maxY, vertex.y);
				}
				break;
			}
			case b2Shape::e_edge:
			{
				const b2EdgeShape* edge = static_cast<const b2EdgeShape*>(shape);
				minX = std::min(minX, std::min(edge->m_vertex1.x, edge->m_vertex2.x));
				minY = std::min(minY, std::min(edge->m_vertex1.y, edge->m_vertex2.y));
				maxX = std::max(maxX, std::max(edge->m_vertex1.x, edge->m_vertex2.x));
				maxY = std::max(maxY, std::max(edge->m_vertex1.y, edge->m_vertex2.y));
				break;
			}
			case b2Shape::e_chain:
			{
				const b2ChainShape* chain = static_cast<const b2ChainShape*>(shape);
				for (int32 i = 0; i < chain->m_count; ++i)
				{
					const b2Vec2& vertex = chain->m_vertices[i];
					minX = std::min(minX, vertex.x);
					minY = std::min(minY, vertex.y);
					maxX = std::max(maxX, vertex.x);
					maxY = std::max(maxY, vertex.y);
				}
				break;
			}
			default:
				break;
			}
		}

		if (!hasFixtures)
		{
			// No fixtures, return origin at (0, 0)
			return b2Vec2(0.0f, 0.0f);
		}

		// The body's position is at (0, 0) in local coordinates
		// We need to find where (0, 0) is relative to the top-left corner of the bounding box
		// The top-left corner of the bounding box is at (minX, minY)
		// So the origin position within the bounding box is at (-minX, -minY)
		return b2Vec2(-minX, -minY);
	}
}
