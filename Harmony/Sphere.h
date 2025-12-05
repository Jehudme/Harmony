#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Sphere : public Renderable
	{
	public:
		Sphere(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Sphere();

		void onRender() override;
	
	public:
		float radius;
		int rings;
		int slices;
		Color color;
		Color frameColor;
	};
}
