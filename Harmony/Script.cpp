#include "pch.h"
#include "Script.h"


HARMONY_REGISTER_SCRIPT(Harmony::Components::Script, BasicScript);

namespace Harmony::Components
{
	Script::Script(const Utilities::Configuration& configuration, Scenes::Scene& scene)
	{
	}

	Script::~Script() = default;

	void Script::onCreate()
	{
	}

	void Script::onDestroy()
	{
	}

	void Script::onPreUpdate()
	{
	}

	void Script::onPostUpdate()
	{
	}

	Scenes::Scene& Script::getScene() { return scene_->get(); }
}