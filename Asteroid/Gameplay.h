#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
	class Gameplay : public harmony::core::Scene
	{
	public:
		Gameplay() = default;

		void onCreate() override;
		void onDestroy() override;
	};
}

