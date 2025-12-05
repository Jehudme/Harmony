#pragma once
#include "ComponentsHandler.h"
#include "Renderable.h"
#include "raymath.h"


namespace Harmony::Components{
	class Capsule : public Renderable
	{
	public:
		Capsule(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Capsule();

		void onRender() override;
	
	public:
		float radius;
		float height;
		int slices;
		int rings;
		Color color;
		Color frameColor;
	};
}
