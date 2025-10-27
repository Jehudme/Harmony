#pragma once

#include <memory>

namespace Harmony::Components
{
	/// @brief PhysicsBody component - basic physics properties for entities
	class PhysicsBody
	{
	public:
		PhysicsBody(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~PhysicsBody();

		// Velocity
		void setVelocity(float x, float y);
		void getVelocity(float& x, float& y) const;
		void addVelocity(float x, float y);

		// Acceleration
		void setAcceleration(float x, float y);
		void getAcceleration(float& x, float& y) const;

		// Force accumulation
		void applyForce(float x, float y);
		void applyImpulse(float x, float y);
		void clearForces();

		// Mass and drag
		void setMass(float mass);
		float getMass() const;
		void setDrag(float drag);
		float getDrag() const;

		// Angular properties
		void setAngularVelocity(float velocity);
		float getAngularVelocity() const;
		void setAngularDrag(float drag);
		float getAngularDrag() const;

		// Gravity scale
		void setGravityScale(float scale);
		float getGravityScale() const;

		// Update physics
		void update(float deltaTime);

		// Kinematic flag (not affected by forces)
		void setKinematic(bool kinematic);
		bool isKinematic() const;

	private:
		struct PhysicsBodyImpl;
		std::unique_ptr<PhysicsBodyImpl> impl_;
	};
}
