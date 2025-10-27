#pragma once

#include <memory>

namespace Harmony::Components
{
	/// @brief ParticleEmitter component - particle system for visual effects
	/// Uses composition to hide implementation details
	class ParticleEmitter
	{
	public:
		ParticleEmitter(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~ParticleEmitter();

		// Emission control
		void start();
		void stop();
		void pause();
		void resume();
		bool isEmitting() const;

		// Emission properties
		void setEmissionRate(float particlesPerSecond);
		float getEmissionRate() const;
		void setMaxParticles(size_t maxCount);
		size_t getMaxParticles() const;

		// Particle properties
		void setParticleLifetime(float minLifetime, float maxLifetime);
		void setParticleSpeed(float minSpeed, float maxSpeed);
		void setParticleSize(float minSize, float maxSize);
		void setParticleColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void setParticleColorRange(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
			unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2);

		// Emitter shape
		void setEmitterPosition(float x, float y);
		void getEmitterPosition(float& x, float& y) const;
		void setEmitterRadius(float radius);
		float getEmitterRadius() const;
		void setEmitterAngle(float minAngle, float maxAngle);

		// Gravity and forces
		void setGravity(float x, float y);
		void getGravity(float& x, float& y) const;

		// Update
		void update(float deltaTime);
		void clear();

		// Statistics
		size_t getActiveParticleCount() const;

		// Internal access for rendering
		void* getInternalParticles();
		const void* getInternalParticles() const;

	private:
		struct ParticleEmitterImpl;
		std::unique_ptr<ParticleEmitterImpl> impl_;
	};
}
