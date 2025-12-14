#include "pch.h"
#include "IRenderableComponent.h"

namespace Harmony::Components 
{
	Harmony::Components::IRenderableComponent::IRenderableComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		m_entityId(entityId), m_scene(scene)
	{
	}

	IRenderableComponent::~IRenderableComponent() = default;
}
