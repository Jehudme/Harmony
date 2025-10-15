#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	class PushState : Internals::Task
	{
	public:
		PushState(const Utilities::UUID stateId);

	private:
		void run() override;

	private:
		const Utilities::UUID stateId_;

	};

	class PopState : Internals::Task
	{
	public:
		PopState();

	private:
		void run() override;

	};
}

