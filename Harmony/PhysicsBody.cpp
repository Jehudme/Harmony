#include "pch.h"
#include "PhysicsBody.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include <cmath>

HARMONY_REGISTER_COMPONENT(Harmony::Components::PhysicsBody, PhysicsBody)

namespace Harmony::Components
{
	struct PhysicsBodyImpl {
		float velocityX = 0.0f;
		float velocityY = 0.0f;
		float accelerationX = 0.0f;
		float accelerationY = 0.0f;
		float forceX = 0.0f;
		float forceY = 0.0f;
		float mass = 1.0f;
		float drag = 0.0f;
		float angularVelocity = 0.0f;
		float angularDrag = 0.0f;
		float gravityScale = 1.0f;
		bool kinematic = false;
	};

	PhysicsBody::PhysicsBody(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<PhysicsBodyImpl>())
	{
		try {
			impl_->velocityX = configuration.get<float>({ "velocity", "x" }).value_or(0.0f);
			impl_->velocityY = configuration.get<float>({ "velocity", "y" }).value_or(0.0f);
			impl_->accelerationX = configuration.get<float>({ "acceleration", "x" }).value_or(0.0f);
			impl_->accelerationY = configuration.get<float>({ "acceleration", "y" }).value_or(0.0f);
			impl_->mass = configuration.get<float>({ "mass" }).value_or(1.0f);
			impl_->drag = configuration.get<float>({ "drag" }).value_or(0.0f);
			impl_->angularVelocity = configuration.get<float>({ "angular_velocity" }).value_or(0.0f);
			impl_->angularDrag = configuration.get<float>({ "angular_drag" }).value_or(0.0f);
			impl_->gravityScale = configuration.get<float>({ "gravity_scale" }).value_or(1.0f);
			impl_->kinematic = configuration.get<bool>({ "kinematic" }).value_or(false);

			HARMONY_DEBUG("PhysicsBody component initialized with mass {}", impl_->mass);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize PhysicsBody component: {}", e.what());
			throw;
		}
	}

	PhysicsBody::~PhysicsBody() = default;

	void PhysicsBody::setVelocity(float x, float y) {
		impl_->velocityX = x;
		impl_->velocityY = y;
	}

	void PhysicsBody::getVelocity(float& x, float& y) const {
		x = impl_->velocityX;
		y = impl_->velocityY;
	}

	void PhysicsBody::addVelocity(float x, float y) {
		impl_->velocityX += x;
		impl_->velocityY += y;
	}

	void PhysicsBody::setAcceleration(float x, float y) {
		impl_->accelerationX = x;
		impl_->accelerationY = y;
	}

	void PhysicsBody::getAcceleration(float& x, float& y) const {
		x = impl_->accelerationX;
		y = impl_->accelerationY;
	}

	void PhysicsBody::applyForce(float x, float y) {
		if (!impl_->kinematic) {
			impl_->forceX += x;
			impl_->forceY += y;
		}
	}

	void PhysicsBody::applyImpulse(float x, float y) {
		if (!impl_->kinematic && impl_->mass > 0.0f) {
			impl_->velocityX += x / impl_->mass;
			impl_->velocityY += y / impl_->mass;
		}
	}

	void PhysicsBody::clearForces() {
		impl_->forceX = 0.0f;
		impl_->forceY = 0.0f;
	}

	void PhysicsBody::setMass(float mass) {
		impl_->mass = std::max(0.001f, mass);
	}

	float PhysicsBody::getMass() const {
		return impl_->mass;
	}

	void PhysicsBody::setDrag(float drag) {
		impl_->drag = drag;
	}

	float PhysicsBody::getDrag() const {
		return impl_->drag;
	}

	void PhysicsBody::setAngularVelocity(float velocity) {
		impl_->angularVelocity = velocity;
	}

	float PhysicsBody::getAngularVelocity() const {
		return impl_->angularVelocity;
	}

	void PhysicsBody::setAngularDrag(float drag) {
		impl_->angularDrag = drag;
	}

	float PhysicsBody::getAngularDrag() const {
		return impl_->angularDrag;
	}

	void PhysicsBody::setGravityScale(float scale) {
		impl_->gravityScale = scale;
	}

	float PhysicsBody::getGravityScale() const {
		return impl_->gravityScale;
	}

	void PhysicsBody::update(float deltaTime) {
		if (impl_->kinematic) return;

		// Apply forces to acceleration (F = ma, so a = F/m)
		if (impl_->mass > 0.0f) {
			impl_->accelerationX += impl_->forceX / impl_->mass;
			impl_->accelerationY += impl_->forceY / impl_->mass;
		}

		// Update velocity with acceleration
		impl_->velocityX += impl_->accelerationX * deltaTime;
		impl_->velocityY += impl_->accelerationY * deltaTime;

		// Apply drag
		if (impl_->drag > 0.0f) {
			float dragFactor = 1.0f / (1.0f + impl_->drag * deltaTime);
			impl_->velocityX *= dragFactor;
			impl_->velocityY *= dragFactor;
		}

		// Apply angular drag
		if (impl_->angularDrag > 0.0f) {
			float dragFactor = 1.0f / (1.0f + impl_->angularDrag * deltaTime);
			impl_->angularVelocity *= dragFactor;
		}

		// Clear forces after applying them
		clearForces();
	}

	void PhysicsBody::setKinematic(bool kinematic) {
		impl_->kinematic = kinematic;
		if (kinematic) {
			clearForces();
		}
	}

	bool PhysicsBody::isKinematic() const {
		return impl_->kinematic;
	}
}
