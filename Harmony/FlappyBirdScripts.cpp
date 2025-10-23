#include "pch.h"
#include "FlappyBirdScripts.h"
#include "Transform.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Text.h"
#include "Scene.h"
#include "Engine.h"

// Register all Flappy Bird scripts
HARMONY_REGISTER_SCRIPT(Harmony::Scripts::FlappyBirdScript, FlappyBirdScript);
HARMONY_REGISTER_SCRIPT(Harmony::Scripts::PipeScript, PipeScript);
HARMONY_REGISTER_SCRIPT(Harmony::Scripts::GroundScript, GroundScript);
HARMONY_REGISTER_SCRIPT(Harmony::Scripts::BackgroundScript, BackgroundScript);
HARMONY_REGISTER_SCRIPT(Harmony::Scripts::GameManagerScript, GameManagerScript);

namespace Harmony::Scripts
{
	// Static members for GameManager
	bool GameManagerScript::isGameOver = false;
	int GameManagerScript::currentScore = 0;

	void GameManagerScript::resetGame()
	{
		isGameOver = false;
		currentScore = 0;
	}

	void GameManagerScript::gameOverState()
	{
		isGameOver = true;
	}

	// ========== FlappyBirdScript ==========
	FlappyBirdScript::FlappyBirdScript(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: Script(configuration, scene)
	{
	}

	void FlappyBirdScript::onCreate()
	{
		HARMONY_INFO("Flappy Bird created!");
		velocity_ = 0.0f;
		gameOver_ = false;
	}

	void FlappyBirdScript::onPreUpdate()
	{
		if (GameManagerScript::isGameOver)
			return;

		auto& scene = getScene();
		float deltaTime = scene.engine.getDeltaTime();

		// Get transform component
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);

		// Handle jump input
		bool spaceCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		if (spaceCurrentlyPressed && !spacePressed_)
		{
			velocity_ = jumpStrength_;
			HARMONY_DEBUG("Bird jump! Velocity: {}", velocity_);
		}
		spacePressed_ = spaceCurrentlyPressed;

		// Apply gravity
		velocity_ += gravity_ * deltaTime;

		// Clamp velocity
		if (velocity_ > maxVelocity_)
			velocity_ = maxVelocity_;

		// Update position
		y += velocity_ * deltaTime;
		transform.setPosition(x, y);

		// Check boundaries and collisions
		checkCollisions();
	}

	void FlappyBirdScript::checkCollisions()
	{
		auto& scene = getScene();
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float birdX, birdY;
		transform.getPosition(birdX, birdY);

		// Check ground collision (y > 620 is approximately ground level)
		if (birdY > 620.0f || birdY < -20.0f)
		{
			HARMONY_WARN("Bird hit ground or ceiling! Game Over!");
			GameManagerScript::gameOverState();
			return;
		}

		// Check pipe collisions - iterate through all entities with PipeScript
		auto& registry = Scenes::getRegistryFromScene(scene);
		auto view = registry.view<Components::Script>();
		
		for (auto entity : view)
		{
			auto& script = view.get<Components::Script>(entity);
			// Check if this is a pipe entity by comparing position range
			auto& pipeTransform = scene.componentReference<Components::Transform>(static_cast<Scenes::EntityID>(entity));
			float pipeX, pipeY;
			pipeTransform.getPosition(pipeX, pipeY);

			// Simple AABB collision detection
			// Bird bounds: approximately 40x40 at (birdX, birdY)
			// Pipe bounds: approximately 80 width
			float birdLeft = birdX - 20.0f;
			float birdRight = birdX + 20.0f;
			float birdTop = birdY - 20.0f;
			float birdBottom = birdY + 20.0f;

			float pipeLeft = pipeX - 40.0f;
			float pipeRight = pipeX + 40.0f;

			// Check if bird is in pipe's horizontal range
			if (birdRight > pipeLeft && birdLeft < pipeRight)
			{
				// Check if bird hits top or bottom pipe
				// Gap is at around y=300-450 (150px gap)
				float gapTop = 250.0f;
				float gapBottom = 400.0f;

				if (birdTop < gapTop || birdBottom > gapBottom)
				{
					HARMONY_WARN("Bird hit pipe! Game Over!");
					GameManagerScript::gameOverState();
					return;
				}
			}
		}
	}

	void FlappyBirdScript::updateScore()
	{
		// Score is updated by pipes when bird passes them
	}

	void FlappyBirdScript::onDestroy()
	{
		HARMONY_INFO("Flappy Bird destroyed");
	}

	// ========== PipeScript ==========
	PipeScript::PipeScript(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: Script(configuration, scene)
	{
	}

	void PipeScript::onCreate()
	{
		HARMONY_DEBUG("Pipe created");
		scoreCounted_ = false;
		
		auto& scene = getScene();
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);
		pipeX_ = x;
	}

	void PipeScript::onPreUpdate()
	{
		if (GameManagerScript::isGameOver)
			return;

		auto& scene = getScene();
		float deltaTime = scene.engine.getDeltaTime();

		// Get transform and move left
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);

		x -= scrollSpeed_ * deltaTime;
		transform.setPosition(x, y);

		// Check if pipe passed the bird (bird is at x=150)
		if (!scoreCounted_ && x < 150.0f)
		{
			GameManagerScript::currentScore++;
			scoreCounted_ = true;
			HARMONY_INFO("Score: {}", GameManagerScript::currentScore);
		}

		// Destroy pipe when off screen
		if (x < -100.0f)
		{
			// Mark for destruction or recycle
			// For now, we'll just let it go off screen
		}
	}

	void PipeScript::onDestroy()
	{
		HARMONY_DEBUG("Pipe destroyed");
	}

	// ========== GroundScript ==========
	GroundScript::GroundScript(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: Script(configuration, scene)
	{
	}

	void GroundScript::onCreate()
	{
		HARMONY_DEBUG("Ground created");
		auto& scene = getScene();
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);
		groundX_ = x;
	}

	void GroundScript::onPreUpdate()
	{
		if (GameManagerScript::isGameOver)
			return;

		auto& scene = getScene();
		float deltaTime = scene.engine.getDeltaTime();

		// Get transform and move left
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);

		x -= scrollSpeed_ * deltaTime;

		// Wrap around when fully scrolled
		if (x < -640.0f) // Half the width of doubled ground
		{
			x += 640.0f;
		}

		transform.setPosition(x, y);
	}

	// ========== BackgroundScript ==========
	BackgroundScript::BackgroundScript(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: Script(configuration, scene)
	{
	}

	void BackgroundScript::onCreate()
	{
		HARMONY_DEBUG("Background created");
		auto& scene = getScene();
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);
		bgX_ = x;
	}

	void BackgroundScript::onPreUpdate()
	{
		if (GameManagerScript::isGameOver)
			return;

		auto& scene = getScene();
		float deltaTime = scene.engine.getDeltaTime();

		// Get transform and move left slowly (parallax effect)
		auto& transform = scene.componentReference<Components::Transform>(entityId);
		float x, y;
		transform.getPosition(x, y);

		x -= scrollSpeed_ * deltaTime;

		// Wrap around
		if (x < -640.0f)
		{
			x += 640.0f;
		}

		transform.setPosition(x, y);
	}

	// ========== GameManagerScript ==========
	GameManagerScript::GameManagerScript(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: Script(configuration, scene)
	{
	}

	void GameManagerScript::onCreate()
	{
		HARMONY_INFO("Game Manager created");
		resetGame();
	}

	void GameManagerScript::onPreUpdate()
	{
		auto& scene = getScene();
		
		// Update score display if it exists
		// We'll find the text entity and update it
		try
		{
			// Update score text (entity ID will be from config)
			// This is a simplified version - in reality we'd track the entity ID
		}
		catch (...)
		{
			// Score text not found or not accessible
		}

		// Check for restart key
		if (isGameOver)
		{
			bool rCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
			if (rCurrentlyPressed && !rKeyPressed_)
			{
				HARMONY_INFO("Restarting game...");
				// Reset the scene
				scene.reset();
			}
			rKeyPressed_ = rCurrentlyPressed;
		}
	}

	void GameManagerScript::onDestroy()
	{
		HARMONY_INFO("Game Manager destroyed");
	}
}
