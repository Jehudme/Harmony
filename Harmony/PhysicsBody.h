#pragma once

#include <box2d/box2d.h>
#include <memory>
#include <vector>

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Components
{
	/// @brief Structure to hold fixture properties
	struct FixtureProperties
	{
		float density = 1.0f;         ///< Density of the fixture (kg/m^2)
		float friction = 0.2f;        ///< Friction coefficient (0.0 = no friction, 1.0 = high friction)
		float restitution = 0.0f;     ///< Restitution/bounciness (0.0 = no bounce, 1.0 = perfect bounce)
		bool isSensor = false;        ///< Whether the fixture is a sensor (doesn't cause collisions)
	};

	/// @brief PhysicsBody component - wraps Box2D b2Body for rigid body physics
	/// Uses RAII to manage body lifetime
	class PhysicsBody
	{
	public:
		PhysicsBody(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~PhysicsBody();

		// Non-copyable
		PhysicsBody(const PhysicsBody&) = delete;
		PhysicsBody& operator=(const PhysicsBody&) = delete;

		// Movable
		PhysicsBody(PhysicsBody&&) noexcept;
		PhysicsBody& operator=(PhysicsBody&&) noexcept;

		// Transform methods
		/// @brief Set the position and angle of the body
		/// @param position Position in world coordinates
		/// @param angle Angle in radians
		void setTransform(const b2Vec2& position, float angle);

		/// @brief Get the body's position
		/// @return Position in world coordinates
		b2Vec2 getPosition() const;

		/// @brief Get the body's angle
		/// @return Angle in radians
		float getAngle() const;

		// Velocity methods
		/// @brief Set the linear velocity
		/// @param velocity Linear velocity
		void setLinearVelocity(const b2Vec2& velocity);

		/// @brief Get the linear velocity
		/// @return Linear velocity
		b2Vec2 getLinearVelocity() const;

		/// @brief Set the angular velocity
		/// @param omega Angular velocity in radians per second
		void setAngularVelocity(float omega);

		/// @brief Get the angular velocity
		/// @return Angular velocity in radians per second
		float getAngularVelocity() const;

		// Force and impulse methods
		/// @brief Apply a force at a world point
		/// @param force Force vector
		/// @param point Point in world coordinates
		/// @param wake Whether to wake up the body
		void applyForce(const b2Vec2& force, const b2Vec2& point, bool wake = true);

		/// @brief Apply a linear impulse at a world point
		/// @param impulse Impulse vector
		/// @param point Point in world coordinates
		/// @param wake Whether to wake up the body
		void applyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point, bool wake = true);

		/// @brief Apply a torque
		/// @param torque Torque value
		/// @param wake Whether to wake up the body
		void applyTorque(float torque, bool wake = true);

		/// @brief Apply an angular impulse
		/// @param impulse Angular impulse
		/// @param wake Whether to wake up the body
		void applyAngularImpulse(float impulse, bool wake = true);

		// Mass and inertia methods
		/// @brief Get the total mass of the body
		/// @return Mass in kilograms
		float getMass() const;

		/// @brief Get the rotational inertia of the body
		/// @return Inertia value
		float getInertia() const;

		// Fixture methods
		/// @brief Create a fixture from a shape with custom properties
		/// @param shape Shape to attach
		/// @param properties Fixture properties (density, friction, restitution, isSensor)
		/// @return Pointer to the created fixture
		b2Fixture* createFixture(const b2Shape* shape, const FixtureProperties& properties);

		/// @brief Create a fixture from a shape (legacy method with density only)
		/// @param shape Shape to attach
		/// @param density Density of the fixture
		/// @return Pointer to the created fixture
		b2Fixture* createFixture(const b2Shape* shape, float density);

		/// @brief Create a box fixture from width and height
		/// @param width Full width of the box
		/// @param height Full height of the box
		/// @param properties Fixture properties (density, friction, restitution, isSensor)
		/// @return Pointer to the created fixture
		b2Fixture* createBoxFixture(float width, float height, const FixtureProperties& properties);

		/// @brief Create a box fixture from width and height (legacy method with density only)
		/// @param width Full width of the box
		/// @param height Full height of the box
		/// @param density Density of the fixture
		/// @return Pointer to the created fixture
		b2Fixture* createBoxFixture(float width, float height, float density);

		/// @brief Create a circle fixture from radius
		/// @param radius Radius of the circle
		/// @param properties Fixture properties (density, friction, restitution, isSensor)
		/// @param center Optional center position (default is origin)
		/// @return Pointer to the created fixture
		b2Fixture* createCircleFixture(float radius, const FixtureProperties& properties, const b2Vec2& center = b2Vec2(0.0f, 0.0f));

		/// @brief Create a circle fixture from radius (legacy method with density only)
		/// @param radius Radius of the circle
		/// @param density Density of the fixture
		/// @param center Optional center position (default is origin)
		/// @return Pointer to the created fixture
		b2Fixture* createCircleFixture(float radius, float density, const b2Vec2& center = b2Vec2(0.0f, 0.0f));

		/// @brief Create a polygon fixture from a vector of points
		/// @param points Vector of points defining the polygon (must be convex, counter-clockwise, max 8 points)
		/// @param properties Fixture properties (density, friction, restitution, isSensor)
		/// @return Pointer to the created fixture, or nullptr if points are invalid
		b2Fixture* createPolygonFixture(const std::vector<b2Vec2>& points, const FixtureProperties& properties);

		/// @brief Create a polygon fixture from a vector of points (legacy method with density only)
		/// @param points Vector of points defining the polygon (must be convex, counter-clockwise, max 8 points)
		/// @param density Density of the fixture
		/// @return Pointer to the created fixture, or nullptr if points are invalid
		b2Fixture* createPolygonFixture(const std::vector<b2Vec2>& points, float density);

		// Body state methods
		/// @brief Set the body type (static, kinematic, dynamic)
		/// @param type Body type
		void setType(b2BodyType type);

		/// @brief Get the body type
		/// @return Body type
		b2BodyType getType() const;

		/// @brief Set the awake state
		/// @param awake Whether the body should be awake
		void setAwake(bool awake);

		/// @brief Check if the body is awake
		/// @return True if awake
		bool isAwake() const;

		/// @brief Set whether the body has fixed rotation
		/// @param fixed Whether rotation is fixed
		void setFixedRotation(bool fixed);

		/// @brief Check if the body has fixed rotation
		/// @return True if rotation is fixed
		bool hasFixedRotation() const;

		/// @brief Get raw body pointer
		/// @return Pointer to the underlying b2Body
		b2Body* get() { return body_; }
		const b2Body* get() const { return body_; }

		// Fixture properties methods
		/// @brief Set the default fixture properties for this body
		/// @param properties Fixture properties to set
		void setFixtureProperties(const FixtureProperties& properties);

		/// @brief Get the default fixture properties for this body
		/// @return Current fixture properties
		const FixtureProperties& getFixtureProperties() const;

		/// @brief Set the friction coefficient
		/// @param friction Friction coefficient (0.0 = no friction, 1.0 = high friction)
		void setFriction(float friction);

		/// @brief Get the friction coefficient
		/// @return Friction coefficient
		float getFriction() const;

		/// @brief Set the restitution (bounciness)
		/// @param restitution Restitution value (0.0 = no bounce, 1.0 = perfect bounce)
		void setRestitution(float restitution);

		/// @brief Get the restitution (bounciness)
		/// @return Restitution value
		float getRestitution() const;

		/// @brief Set the density
		/// @param density Density value (kg/m^2)
		void setDensity(float density);

		/// @brief Get the density
		/// @return Density value
		float getDensity() const;

		/// @brief Set whether fixtures are sensors
		/// @param isSensor True if fixtures should be sensors
		void setIsSensor(bool isSensor);

		/// @brief Get whether fixtures are sensors
		/// @return True if fixtures are sensors
		bool getIsSensor() const;

	private:
		b2Body* body_;
		b2World* world_;
		FixtureProperties fixtureProperties_;
	};
}
