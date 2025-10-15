#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	class SetFPS : public Internals::Task
	{
	public:
		SetFPS(const unsigned int targetFPS);

	private:
		const unsigned int targetFPS_;
	};

	class GetFPS : public Internals::Task
	{
	public:
		GetFPS();

	public:
		std::shared_ptr<std::promise<unsigned int>> value;
	};
}

