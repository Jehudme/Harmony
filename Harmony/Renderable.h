#pragma once
#include "ComponentsHandler.h"
#include "Scene.h"

namespace Harmony::Components {
	class Renderable
	{
	public:
		Renderable(Internals::Scene& scene, Internals::EntityID entityId_);
		virtual ~Renderable();

		virtual void onRender() = 0;
		void preRender();
		void postRender();

	protected:
		Internals::Scene& scene_;
		Internals::EntityID entityId_;
	};
}

