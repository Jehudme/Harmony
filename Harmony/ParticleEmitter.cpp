#include "pch.h"
#include "ParticleEmitter.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include "RandomValue.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

HARMONY_REGISTER_COMPONENT_WITH_BASE(sf::Drawable, Harmony::Components::ParticleEmitter, ParticleEmitter)

namespace Harmony::Components
{
	struct Particle {
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Color color;
		float lifetime;
		float maxLifetime;
		float size;
		bool active;
	};

	// PImpl to hide particle system implementation
	struct ParticleEmitter::ParticleEmitterImpl : public sf::Drawable {
		std::vector<Particle> particles;
		sf::Vector2f emitterPosition{ 0.0f, 0.0f };
		sf::Vector2f gravity{ 0.0f, 0.0f };
		float emitterRadius = 0.0f;
		float minAngle = 0.0f;
		float maxAngle = 360.0f;
		float emissionRate = 10.0f;
		float timeSinceLastEmission = 0.0f;
		float minLifetime = 1.0f;
		float maxLifetime = 2.0f;
		float minSpeed = 50.0f;
		float maxSpeed = 100.0f;
		float minSize = 2.0f;
		float maxSize = 5.0f;
		sf::Color color1{ 255, 255, 255, 255 };
		sf::Color color2{ 255, 255, 255, 255 };
		size_t maxParticles = 1000;
		bool emitting = false;
		bool paused = false;

		mutable sf::VertexArray vertices{ sf::Quads };

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			if (vertices.getVertexCount() == 0) return;
			target.draw(vertices, states);
		}
	};

	ParticleEmitter::ParticleEmitter(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<ParticleEmitterImpl>())
	{
		try {
			// Emitter position
			float x = configuration.get<float>({ "position", "x" }).value_or(0.0f);
			float y = configuration.get<float>({ "position", "y" }).value_or(0.0f);
			impl_->emitterPosition = sf::Vector2f(x, y);

			// Emission rate
			impl_->emissionRate = configuration.get<float>({ "emission_rate" }).value_or(10.0f);
			impl_->maxParticles = configuration.get<int>({ "max_particles" }).value_or(1000);

			// Particle lifetime
			impl_->minLifetime = configuration.get<float>({ "min_lifetime" }).value_or(1.0f);
			impl_->maxLifetime = configuration.get<float>({ "max_lifetime" }).value_or(2.0f);

			// Particle speed
			impl_->minSpeed = configuration.get<float>({ "min_speed" }).value_or(50.0f);
			impl_->maxSpeed = configuration.get<float>({ "max_speed" }).value_or(100.0f);

			// Particle size
			impl_->minSize = configuration.get<float>({ "min_size" }).value_or(2.0f);
			impl_->maxSize = configuration.get<float>({ "max_size" }).value_or(5.0f);

			// Color
			int r = configuration.get<int>({ "color", "r" }).value_or(255);
			int g = configuration.get<int>({ "color", "g" }).value_or(255);
			int b = configuration.get<int>({ "color", "b" }).value_or(255);
			int a = configuration.get<int>({ "color", "a" }).value_or(255);
			impl_->color1 = impl_->color2 = sf::Color(r, g, b, a);

			// Emitter shape
			impl_->emitterRadius = configuration.get<float>({ "radius" }).value_or(0.0f);
			impl_->minAngle = configuration.get<float>({ "min_angle" }).value_or(0.0f);
			impl_->maxAngle = configuration.get<float>({ "max_angle" }).value_or(360.0f);

			// Gravity
			float gravX = configuration.get<float>({ "gravity", "x" }).value_or(0.0f);
			float gravY = configuration.get<float>({ "gravity", "y" }).value_or(0.0f);
			impl_->gravity = sf::Vector2f(gravX, gravY);

			// Auto start
			bool autoStart = configuration.get<bool>({ "auto_start" }).value_or(false);
			if (autoStart) {
				impl_->emitting = true;
			}

			// Reserve particle memory
			impl_->particles.resize(impl_->maxParticles);
			for (auto& p : impl_->particles) {
				p.active = false;
			}

			HARMONY_DEBUG("ParticleEmitter component initialized with {} max particles", impl_->maxParticles);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize ParticleEmitter component: {}", e.what());
			throw;
		}
	}

	ParticleEmitter::~ParticleEmitter() = default;

	void ParticleEmitter::start() {
		impl_->emitting = true;
		impl_->paused = false;
	}

	void ParticleEmitter::stop() {
		impl_->emitting = false;
	}

	void ParticleEmitter::pause() {
		impl_->paused = true;
	}

	void ParticleEmitter::resume() {
		impl_->paused = false;
	}

	bool ParticleEmitter::isEmitting() const {
		return impl_->emitting && !impl_->paused;
	}

	void ParticleEmitter::setEmissionRate(float particlesPerSecond) {
		impl_->emissionRate = particlesPerSecond;
	}

	float ParticleEmitter::getEmissionRate() const {
		return impl_->emissionRate;
	}

	void ParticleEmitter::setMaxParticles(size_t maxCount) {
		impl_->maxParticles = maxCount;
		impl_->particles.resize(maxCount);
		for (auto& p : impl_->particles) {
			p.active = false;
		}
	}

	size_t ParticleEmitter::getMaxParticles() const {
		return impl_->maxParticles;
	}

	void ParticleEmitter::setParticleLifetime(float minLifetime, float maxLifetime) {
		impl_->minLifetime = minLifetime;
		impl_->maxLifetime = maxLifetime;
	}

	void ParticleEmitter::setParticleSpeed(float minSpeed, float maxSpeed) {
		impl_->minSpeed = minSpeed;
		impl_->maxSpeed = maxSpeed;
	}

	void ParticleEmitter::setParticleSize(float minSize, float maxSize) {
		impl_->minSize = minSize;
		impl_->maxSize = maxSize;
	}

	void ParticleEmitter::setParticleColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		impl_->color1 = impl_->color2 = sf::Color(r, g, b, a);
	}

	void ParticleEmitter::setParticleColorRange(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
		unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2) {
		impl_->color1 = sf::Color(r1, g1, b1, a1);
		impl_->color2 = sf::Color(r2, g2, b2, a2);
	}

	void ParticleEmitter::setEmitterPosition(float x, float y) {
		impl_->emitterPosition = sf::Vector2f(x, y);
	}

	void ParticleEmitter::getEmitterPosition(float& x, float& y) const {
		x = impl_->emitterPosition.x;
		y = impl_->emitterPosition.y;
	}

	void ParticleEmitter::setEmitterRadius(float radius) {
		impl_->emitterRadius = radius;
	}

	float ParticleEmitter::getEmitterRadius() const {
		return impl_->emitterRadius;
	}

	void ParticleEmitter::setEmitterAngle(float minAngle, float maxAngle) {
		impl_->minAngle = minAngle;
		impl_->maxAngle = maxAngle;
	}

	void ParticleEmitter::setGravity(float x, float y) {
		impl_->gravity = sf::Vector2f(x, y);
	}

	void ParticleEmitter::getGravity(float& x, float& y) const {
		x = impl_->gravity.x;
		y = impl_->gravity.y;
	}

	void ParticleEmitter::update(float deltaTime) {
		if (impl_->paused) return;

		// Emit new particles
		if (impl_->emitting && impl_->emissionRate > 0.0f) {
			impl_->timeSinceLastEmission += deltaTime;
			float emissionInterval = 1.0f / impl_->emissionRate;

			while (impl_->timeSinceLastEmission >= emissionInterval) {
				impl_->timeSinceLastEmission -= emissionInterval;

				// Find inactive particle
				for (auto& p : impl_->particles) {
					if (!p.active) {
						p.active = true;

						// Random position within emitter radius
						float angle = Utilities::random_value<float>(0.0f, 360.0f) * 3.14159f / 180.0f;
						float dist = Utilities::random_value<float>(0.0f, impl_->emitterRadius);
						p.position = impl_->emitterPosition + sf::Vector2f(
							std::cos(angle) * dist, std::sin(angle) * dist);

						// Random velocity
						float speedAngle = Utilities::random_value<float>(impl_->minAngle, impl_->maxAngle) * 3.14159f / 180.0f;
						float speed = Utilities::random_value<float>(impl_->minSpeed, impl_->maxSpeed);
						p.velocity = sf::Vector2f(std::cos(speedAngle) * speed, std::sin(speedAngle) * speed);

						// Random lifetime
						p.maxLifetime = Utilities::random_value<float>(impl_->minLifetime, impl_->maxLifetime);
						p.lifetime = p.maxLifetime;

						// Random size
						p.size = Utilities::random_value<float>(impl_->minSize, impl_->maxSize);

						// Random color
						float t = Utilities::random_value<float>(0.0f, 1.0f);
						p.color = sf::Color(
							static_cast<sf::Uint8>(impl_->color1.r * (1 - t) + impl_->color2.r * t),
							static_cast<sf::Uint8>(impl_->color1.g * (1 - t) + impl_->color2.g * t),
							static_cast<sf::Uint8>(impl_->color1.b * (1 - t) + impl_->color2.b * t),
							static_cast<sf::Uint8>(impl_->color1.a * (1 - t) + impl_->color2.a * t));

						break;
					}
				}
			}
		}

		// Update active particles
		impl_->vertices.clear();
		for (auto& p : impl_->particles) {
			if (p.active) {
				p.lifetime -= deltaTime;
				if (p.lifetime <= 0.0f) {
					p.active = false;
					continue;
				}

				// Apply gravity
				p.velocity += impl_->gravity * deltaTime;

				// Update position
				p.position += p.velocity * deltaTime;

				// Fade alpha based on lifetime
				float lifetimeRatio = p.lifetime / p.maxLifetime;
				sf::Color color = p.color;
				color.a = static_cast<sf::Uint8>(color.a * lifetimeRatio);

				// Create quad for particle
				float halfSize = p.size * 0.5f;
				impl_->vertices.append(sf::Vertex(p.position + sf::Vector2f(-halfSize, -halfSize), color));
				impl_->vertices.append(sf::Vertex(p.position + sf::Vector2f(halfSize, -halfSize), color));
				impl_->vertices.append(sf::Vertex(p.position + sf::Vector2f(halfSize, halfSize), color));
				impl_->vertices.append(sf::Vertex(p.position + sf::Vector2f(-halfSize, halfSize), color));
			}
		}
	}

	void ParticleEmitter::clear() {
		for (auto& p : impl_->particles) {
			p.active = false;
		}
		impl_->vertices.clear();
	}

	size_t ParticleEmitter::getActiveParticleCount() const {
		size_t count = 0;
		for (const auto& p : impl_->particles) {
			if (p.active) count++;
		}
		return count;
	}

	void* ParticleEmitter::getInternalParticles() {
		return impl_.get();
	}

	const void* ParticleEmitter::getInternalParticles() const {
		return impl_.get();
	}
}
