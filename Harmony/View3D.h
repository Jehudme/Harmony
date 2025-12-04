#pragma once
#include "ComponentsHandler.h"

namespace Harmony::Components {
	class View3D : public Camera3D
	{
	public:
		View3D(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~View3D();
	};
}

