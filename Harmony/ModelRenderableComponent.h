#pragma once
#include "ComponentsHandler.h"
#include "IRenderableComponent.h"

namespace Harmony::Components
{
	class ModelRenderableComponent : public IRenderableComponent
	{
	public:
		ModelRenderableComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~ModelRenderableComponent();

		void onRender() override;
		void onPreRender() override;
		void onPostRender() override;
	};
}

