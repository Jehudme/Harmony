#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Cube : public Renderable
	{
	public:
		Cube(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Cube();

		void onRender() override;
	
	public:
		Vector3 size;
		Color color;
		Color frameColor;
	};
}

