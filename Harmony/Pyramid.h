#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Pyramid : public Renderable
	{
	public:
		Pyramid(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Pyramid();

		void onRender() override;
	
	public:
		Vector3 size;
		Color color;
		Color frameColor;
	};
}
