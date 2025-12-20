#pragma once


namespace Harmony {
	class Engine;
	class TaskDispatcher;

	struct EngineContext {
		friend class Engine;
	
	private:
		EngineContext(Engine& engine, TaskDispatcher& TaskDispatcher);

	public:
		EngineContext(const EngineContext&) = default;
		EngineContext& operator=(const EngineContext&) = default;

		Engine& engine;
		TaskDispatcher& taskDispatcher;
	};

} // namespace Harmony