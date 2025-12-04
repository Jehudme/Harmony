#pragma once
#include "Transformable.h"
#include "ComponentsHandler.h"

namespace Harmony::Components {
	class Transform : public Utilities::Transformable
	{
		Transform(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~Transform() = default;
	};
}

