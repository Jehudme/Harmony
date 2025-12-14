#pragma once
#include "Transformable.h"
#include "ComponentsHandler.h"

namespace Harmony::Components {
	class TransformComponent : public Utilities::Transformable
	{
	public:
		TransformComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~TransformComponent();
	};
}

