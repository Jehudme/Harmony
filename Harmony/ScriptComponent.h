#pragma once
#include "ComponentsHandler.h"

namespace Harmony::Components {
	class ScriptComponent
	{
	public:
		ScriptComponent();
		virtual ~ScriptComponent();

		virtual void onCreate();
		virtual void onDestroy();

		virtual void onUpdate();
		virtual void onPreUpdate();
		virtual void onPostUpdate();

		virtual void onRender();
		virtual void onPreRender();
		virtual void onPostRender();
	};
}

#define HARMONY_REGISTER_SCRIPT(scriptClass, scriptName) \
	HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::ScriptComponent, scriptClass, scriptName)
