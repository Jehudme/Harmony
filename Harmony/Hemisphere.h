#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Hemisphere : public Renderable
	{
	public:
		Hemisphere(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Hemisphere();

		void onRender() override;
	
	public:
		float radius;
		int rings;
		int slices;
		Color color;
		Color frameColor;
	};
}
