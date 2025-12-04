#include "pch.h"
#include "Script.h"

namespace Harmony::Components {

	Script::~Script() = default;

	void Harmony::Components::Script::onCreate() {}
	void Script::onDestroy() {}
	void Script::onPreUpdate() {}
	void Script::onPostUpdate() {}
	void Script::onPreRender() {}
	void Script::onPostRender() {}
}
