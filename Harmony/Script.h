#pragma once
#include "ComponentsHandler.h"

namespace Harmony::Components {
	class Script
	{
	public:
		Script(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		virtual ~Script();

		virtual void onCreate();
		virtual void onDestroy();

		virtual void onPreUpdate();
		virtual void onPostUpdate();

		virtual void onPreRender();
		virtual void onPostRender();
	};
}

#define HARMONY_REGISTER_SCRIPT(scriptClass, scriptName) \
	HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Script, scriptClass, scriptName)
