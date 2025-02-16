#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
	class GameState : public harmony::core::State
	{
	public:
		GameState() = default;

		void onEnter() override;
		void onExit() override;
	};
}

