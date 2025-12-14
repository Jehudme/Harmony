#pragma once
#include "ComponentsHandler.h"

namespace Harmony::Components
{
	class IRenderableComponent
	{
	public:
		IRenderableComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		virtual ~IRenderableComponent();

		virtual void onRender() = 0;

		virtual void onPreRender() = 0;
		virtual void onPostRender() = 0;

	protected:
		Harmony::Internals::EntityID m_entityId;
		Harmony::Internals::Scene& m_scene;
	};
}

