#pragma once
#include "Script.h"
#include <SFML/Window/Keyboard.hpp>

namespace Harmony::Scripts
{
	/// @brief Flappy Bird player script - handles bird physics and input
	class FlappyBirdScript : public Components::Script
	{
	public:
		FlappyBirdScript(const Utilities::Configuration& configuration, Scenes::Scene& scene);

		void onCreate() override;
		void onPreUpdate() override;
		void onDestroy() override;

	private:
		float velocity_ = 0.0f;
		const float gravity_ = 980.0f;      // pixels per second squared
		const float jumpStrength_ = -350.0f; // pixels per second
		const float maxVelocity_ = 600.0f;   // terminal velocity
		bool gameOver_ = false;
		int score_ = 0;
		bool spacePressed_ = false;

		void checkCollisions();
		void updateScore();
	};

	/// @brief Pipe obstacle script - handles pipe movement and collision
	class PipeScript : public Components::Script
	{
	public:
		PipeScript(const Utilities::Configuration& configuration, Scenes::Scene& scene);

		void onCreate() override;
		void onPreUpdate() override;
		void onDestroy() override;

	private:
		const float scrollSpeed_ = 200.0f; // pixels per second
		bool scoreCounted_ = false;
		float pipeX_ = 0.0f;
	};

	/// @brief Ground script - handles scrolling ground
	class GroundScript : public Components::Script
	{
	public:
		GroundScript(const Utilities::Configuration& configuration, Scenes::Scene& scene);

		void onCreate() override;
		void onPreUpdate() override;

	private:
		const float scrollSpeed_ = 200.0f; // pixels per second
		float groundX_ = 0.0f;
	};

	/// @brief Background script - handles scrolling background
	class BackgroundScript : public Components::Script
	{
	public:
		BackgroundScript(const Utilities::Configuration& configuration, Scenes::Scene& scene);

		void onCreate() override;
		void onPreUpdate() override;

	private:
		const float scrollSpeed_ = 50.0f; // pixels per second
		float bgX_ = 0.0f;
	};

	/// @brief Game manager script - handles game state and UI
	class GameManagerScript : public Components::Script
	{
	public:
		GameManagerScript(const Utilities::Configuration& configuration, Scenes::Scene& scene);

		void onCreate() override;
		void onPreUpdate() override;
		void onDestroy() override;

		static bool isGameOver;
		static int currentScore;
		static void resetGame();
		static void gameOverState();

	private:
		bool rKeyPressed_ = false;
		float timeSinceLastSpawn_ = 0.0f;
		const float spawnInterval_ = 3.0f; // Spawn pipes every 3 seconds
	};
}
