#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Plane : public Renderable
	{
	public:
		Plane(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Plane();

		void onRender() override;
	
	public:
		Vector2 size;
		Color color;
		Color frameColor;
	};
}
