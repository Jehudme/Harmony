#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Torus : public Renderable
	{
	public:
		Torus(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Torus();

		void onRender() override;
	
	public:
		float radius;
		float size;
		int radSeg;
		int sides;
		Color color;
		Color frameColor;
	};
}
