#include "pch.h"
#include "Task.h"
#include "Engine.h"
#include "StateStack.h"
#include "State.h"
#include "SceneNode.h"
#include "Scene.h"
#include "Configuration.h"

Harmony::Task_t::Task_t(const uint64_t& uniqueId) 
	: Object(uniqueId) {
}

Harmony::SwitchStateTask::SwitchStateTask(const std::string& state, const std::string& engine)
	: state_(state), engine_(engine)
{
}

void Harmony::SwitchStateTask::execute()
{
	auto engine = find<Harmony::Engine>(engine_);
	engine->getStateStack()->pop();
	engine->getStateStack()->push(state_);
}

Harmony::DetachNodeTask::DetachNodeTask(std::shared_ptr<SceneNode> node)
	: node(node)
{
}

void Harmony::DetachNodeTask::execute()
{
	node->detach();
}

Harmony::AttachNodeTask::AttachNodeTask(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child)
	: parent(parent), child(child)
{
}

void Harmony::AttachNodeTask::execute()
{
	parent->attachChild(child);
}

Harmony::QueueSceneTask::QueueSceneTask(const std::string& state, const std::string& scene)
	: scene(scene), state(state)
{
}

void Harmony::QueueSceneTask::execute()
{
	find<Harmony::State>(state)->queueScene(scene);
	find<Harmony::Scene>(scene)->onEnter();
}

Harmony::UnqueueSceneTask::UnqueueSceneTask(const std::string& state, const std::string& scene)
	: state(state), scene(scene)
{
}

void Harmony::UnqueueSceneTask::execute()
{
	find<Harmony::State>(state)->removeQueuedScene(scene);
	find<Harmony::Scene>(scene)->onExit();
}
