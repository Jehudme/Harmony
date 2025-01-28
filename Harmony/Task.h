#pragma once
#include "Object.h"

namespace Harmony 
{
	struct Task : public Object
	{
		Task(const uint64_t& uniqueId = NULL);

		virtual void execute() = NULL;
	};
}
