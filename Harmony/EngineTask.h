#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	class SetFPS : public Tasks::Task
	{
	public:
		SetFPS(const unsigned int targetFPS);
		void run() override;

	private:
		const unsigned int targetFPS_;
	};
}

