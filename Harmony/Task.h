#pragma once
#include "Object.h"

namespace Harmony
{
	class SceneNode;
	struct Task_t : public Object
	{
		Task_t(const uint64_t& uniqueId = NULL);

		virtual void execute() = NULL;
	};


	struct SwitchStateTask : public Task_t
	{
		SwitchStateTask(const std::string& state, const std::string& engine);

		void execute() override;

		std::string state_;
		std::string engine_;
	};

	struct DetachNodeTask : public Task_t
	{
		DetachNodeTask(std::shared_ptr<SceneNode> node);
		void execute() override;

		std::shared_ptr<SceneNode> node;
	};

	struct AttachNodeTask : public Task_t
	{
		AttachNodeTask(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child);
		void execute() override;

		std::shared_ptr<SceneNode> parent, child;
	};

	struct QueueSceneTask : public Task_t
	{
		QueueSceneTask(const std::string& state, const std::string& scene);
		void execute() override;
		std::string scene;
		std::string state;
	};

	struct UnqueueSceneTask : public Task_t
	{
		UnqueueSceneTask(const std::string& state, const std::string& scene);
		void execute() override;

		std::string state;
		std::string scene;
	};
}
