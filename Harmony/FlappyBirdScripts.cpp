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
		// Check ceiling collision (y < 20)
		if (birdY > 600.0f || birdY < 20.0f)
		{
			HARMONY_WARN("Bird hit ground or ceiling! Game Over!");
			GameManagerScript::gameOverState();
			return;
		}

		// Check pipe collisions with all entities that have rectangles
		auto& registry = Scenes::getRegistryFromScene(scene);
		auto pipeView = registry.view<Components::Rectangle>();
		
		for (auto entity : pipeView)
		{
			try
			{
				auto& pipeTransform = scene.componentReference<Components::Transform>(static_cast<Scenes::EntityID>(entity));
				float pipeX, pipeY;
				pipeTransform.getPosition(pipeX, pipeY);

				// Skip if this is not a pipe (ground, background, etc)
				// Pipes are in the x range 400-2000 initially
				if (pipeX < 0 || pipeX > 2000)
					continue;

				// Simple AABB collision detection
				// Bird bounds: 40x40 centered at (birdX, birdY)
				// Pipe bounds: 80x(various heights) centered at (pipeX, pipeY)
				float birdRadius = 20.0f;
				float pipeHalfWidth = 40.0f;
				
				float birdLeft = birdX - birdRadius;
				float birdRight = birdX + birdRadius;
				float birdTop = birdY - birdRadius;
				float birdBottom = birdY + birdRadius;

				float pipeLeft = pipeX - pipeHalfWidth;
				float pipeRight = pipeX + pipeHalfWidth;

				// Check if bird is in pipe's horizontal range
				if (birdRight > pipeLeft && birdLeft < pipeRight)
				{
					// Get pipe height from rectangle component
					auto& pipeRect = registry.get<Components::Rectangle>(entity);
					// Pipes positioned so their center is at pipeY
					// Top pipes have their bottom edge at pipeY + height/2
					// Bottom pipes have their top edge at pipeY - height/2
					
					// Check if bird collides with the pipe rectangle
					// If pipeY < 300, it's a top pipe, if > 400, it's a bottom pipe
					if (pipeY < 300.0f)
					{
						// Top pipe - check if bird hits the bottom of it
						float pipeBottom = pipeY + 125.0f; // Approximate half height of top pipes
						if (birdTop < pipeBottom)
						{
							HARMONY_WARN("Bird hit top pipe! Game Over!");
							GameManagerScript::gameOverState();
							return;
						}
					}
					else if (pipeY > 400.0f)
					{
						// Bottom pipe - check if bird hits the top of it
						float pipeTop = pipeY - 125.0f; // Approximate half height of bottom pipes
						if (birdBottom > pipeTop)
						{
							HARMONY_WARN("Bird hit bottom pipe! Game Over!");
							GameManagerScript::gameOverState();
							return;
						}
					}
				}
			}
			catch (...)
			{
				// Entity doesn't have transform, skip it
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

		// Check if pipe passed the bird (bird is at x=150)
		// Only count score for bottom pipes (y > 400) to avoid double counting
		if (!scoreCounted_ && x < 150.0f && y > 400.0f)
		{
			GameManagerScript::currentScore++;
			scoreCounted_ = true;
			HARMONY_INFO("Score: {}", GameManagerScript::currentScore);
		}

		// Recycle pipe when off screen - move to the right
		if (x < -100.0f)
		{
			x = 1400.0f; // Move back to the right side
			scoreCounted_ = false; // Reset score counting
			
			// Randomize pipe height slightly for variety
			// Keep y roughly the same but add some variation
			float variation = (rand() % 100) - 50.0f; // -50 to +50
			y += variation;
			
			// Clamp to reasonable values
			if (y < 100.0f) y = 100.0f;
			if (y > 600.0f) y = 600.0f;
		}

		transform.setPosition(x, y);
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
