#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Cone : public Renderable
	{
	public:
		Cone(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Cone();

		void onRender() override;
	
	public:
		float radius;
		float height;
		int slices;
		Color color;
		Color frameColor;
	};
}
