#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	// Task to load a resource asynchronously
	class LoadResourceTask : public Tasks::Task
	{
	public:
		LoadResourceTask(const std::string& resourceType, const Utilities::UUID resourceId);

	private:
		void run() override;

	private:
		const std::string resourceType_;
		const Utilities::UUID resourceId_;
	};

	// Task to unload a resource asynchronously
	class UnloadResourceTask : public Tasks::Task
	{
	public:
		UnloadResourceTask(const Utilities::UUID resourceId);

	private:
		void run() override;

	private:
		const Utilities::UUID resourceId_;
	};

	// Task to unload all resources
	class UnloadAllResourcesTask : public Tasks::Task
	{
	public:
		UnloadAllResourcesTask();

	private:
		void run() override;
	};

	// Task to batch load multiple resources
	class BatchLoadResourcesTask : public Tasks::Task
	{
	public:
		BatchLoadResourcesTask(const std::vector<std::pair<std::string, Utilities::UUID>>& resources);

	private:
		void run() override;

	private:
		const std::vector<std::pair<std::string, Utilities::UUID>> resources_;
	};
}
