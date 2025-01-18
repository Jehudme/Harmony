#pragma once
#include "Harmony/Scene.h"

namespace Asteroid
{
	class Gameplay : public Harmony::Core::Scene
	{
	public:
		Gameplay() = default;

		void onEnterCurrent() override;
		void onExitCurrent() override;
	};
}

