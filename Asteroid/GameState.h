#pragma once
#include "Harmony/State.h"

namespace Asteroid
{
	class GameState : public Harmony::Core::State
	{
	public:
		GameState() = default;

		void onEnter() override;
		void onExit() override;
	};
}

