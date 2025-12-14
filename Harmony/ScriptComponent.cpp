#include "pch.h"
#include "ScriptComponent.h"

namespace Harmony::Components {
	ScriptComponent::ScriptComponent() = default;
	ScriptComponent::~ScriptComponent() = default;

	void Harmony::Components::ScriptComponent::onCreate() {}
	void ScriptComponent::onDestroy() {}
	void ScriptComponent::onUpdate() {}
	void ScriptComponent::onPreUpdate() {}
	void ScriptComponent::onPostUpdate() {}
	void ScriptComponent::onRender() {}
	void ScriptComponent::onPreRender() {}
	void ScriptComponent::onPostRender() {}
}
