#include "pch.h"
#include "Script.h"
#include "ComponentManagement.h"

HARMONY_REGISTER_SCRIPT(Harmony::Components::Script, script)

namespace Harmony::Components
{
	Script::Script(const Utilities::Configuration& configuration, Scenes::Scene& scene) {}
	Script::Script() = default;
	Script::~Script() = default;

	void Script::onCreate() {}
	void Script::onDestroy() {}
	void Script::onPreUpdate() {}
	void Script::onPostUpdate() {}

	Scenes::Scene& Script::getScene() { return scene_->get(); }
	const EntityID Script::getEntityId() const { return entityId_; }
}