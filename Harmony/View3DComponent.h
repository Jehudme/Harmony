#pragma once
#include "ComponentsHandler.h"

namespace Harmony::Components {
	class View3DComponent : public Camera3D
	{
	public:
		View3DComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~View3DComponent();
	};
}

