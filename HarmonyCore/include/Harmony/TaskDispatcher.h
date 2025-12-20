#pragma once
#include "Harmony/Properties.h"
#include "Harmony/ITask.h"
#include <memory>

namespace Harmony {
	class Engine;

	class TaskDispatcher {
		friend class Engine;

	public:
		TaskDispatcher();
		~TaskDispatcher();

		template<typename Task, typename... Args>
		void submit(Args&&... args);

		void submit(std::unique_ptr<ITask> task);

		void startPool();
		void stopPool();

	private:
		void executeAll();
		void execute(std::unique_ptr<ITask> task);

	private:
		struct Internal;
		std::unique_ptr<Internal> m_internal;
	};

	template<typename Task, typename ...Args>
	inline void TaskDispatcher::submit(Args && ...args) {
		auto task = std::make_unique<Task>(std::forward<Args>(args)...);
		submit(std::move(task));
	}
}