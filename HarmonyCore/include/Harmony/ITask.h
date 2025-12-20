#pragma once
#include <memory>
#include <atomic>

namespace Harmony {
	class TaskDispatcher;

	enum class ExecutionMode{ 
		UndedicatedThreadMode,
		DedicatedThreadMode,
		MainThreadMode, 	
	};

	enum class ExecutionPriority : unsigned char { 
		Imediate	= 255, 
		High		= 170, 
		Normal		= 85, 
		Low			= 0
	};

	class ITask : std::enable_shared_from_this<ITask> {

	public:
		ITask(ExecutionMode mode, unsigned char priotity) :	
			mode(mode), priority(priotity) {}

		virtual ~ITask() = default;

	public:
		const ExecutionMode mode;
		const unsigned char priority;

	protected:
		virtual void run() = 0;
	};
}