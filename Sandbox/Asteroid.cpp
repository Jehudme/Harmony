#include <Harmony/fwd.h>
#include <Harmony/Script.h>
#include <Harmony/Circle.h>
#include <Harmony/Engine.h>
#include <Harmony/Transform.h>
#include <Harmony/ComponentManagement.h>
#include <Harmony/InputManager.h>
#include <Harmony/OptionalReference.h>

#include <SFML/System/InputStream.hpp>

namespace Sandbox::Asteroid 
{
	struct PlayerScript : public Harmony::Components::Script
	{
	public:
		PlayerScript(const Harmony::Utilities::Configuration&, Harmony::Scenes::Scene&)
		{
		}

		void onCreate() 
		{
			deltaTime_ = getScene().engine.getDeltaTime();
			transform_ = getScene().getComponent<Harmony::Components::Transform>(getEntityId());
			circle_ = dynamic_cast<Harmony::Components::Circle&>(getScene().getComponent<sf::Drawable>(getEntityId()));

			const float radius = circle_->getRadius();
			circle_->setOrigin(circle_->getRadius(), circle_->getRadius());
			circle_->rotate(90.0f);
		};

		void onDestroy() 
		{
		};

		void onPreUpdate()
		{
			rotationVelocity = 0.0f;
			positionAcceleration = 0.0f;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))		rotationVelocity -= 100.0f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	rotationVelocity += 100.0f;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))		positionAcceleration = 500.0f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))		positionAcceleration = -500.0f;
		};

		void onPostUpdate()
		{
			const float dt = deltaTime_->asSeconds();

			transform_->rotate(rotationVelocity * dt);

			const float angle = transform_->getRotation() * 3.14159f / 180.0f;

			const float xpositionAcceleration = positionAcceleration * std::cos(angle);
			const float ypositionAcceleration = positionAcceleration * std::sin(angle);

			// Update velocity with acceleration
			xpositionVelocity += xpositionAcceleration * dt;
			ypositionVelocity += ypositionAcceleration * dt;

			// Apply friction
			xpositionVelocity -= friction * xpositionVelocity * dt;
			ypositionVelocity -= friction * ypositionVelocity * dt;

			// Clamp values
			xpositionVelocity = std::clamp(xpositionVelocity, minVelocity, maxVelocity);
			ypositionVelocity = std::clamp(ypositionVelocity, minVelocity, maxVelocity);

			// Correct displacement formula
			const float xPositionDelta = xpositionVelocity * dt + 0.5f * xpositionAcceleration * dt * dt;
			const float yPositionDelta = ypositionVelocity * dt + 0.5f * ypositionAcceleration * dt * dt;

			transform_->move(xPositionDelta, yPositionDelta);		
		}

	private:
		float positionAcceleration = 0.0f, rotationVelocity = 0.0f;
		float xpositionVelocity = .0f, ypositionVelocity = .0f;

		static inline const float maxVelocity = 500.0f;
		static inline const float minVelocity = -500.0f;
		static inline const float friction = 0.30f;

		Harmony::Utilities::OptionalReference<const sf::Time> deltaTime_;
		Harmony::Utilities::OptionalReference<Harmony::Components::Circle> circle_;
		Harmony::Utilities::OptionalReference<Harmony::Components::Transform> transform_;
	};
}


HARMONY_REGISTER_SCRIPT(Sandbox::Asteroid::PlayerScript, player_script)