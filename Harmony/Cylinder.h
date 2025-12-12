#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Cylinder : public Renderable
	{
	public:
		Cylinder(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Cylinder();

		void onRender() override;
	
	public:
		float radiusTop;
		float radiusBottom;
		float height;
		int slices;
		Color color;
		Color frameColor;
	};
}
