namespace Harmony {
	// LambdaTask template constructor implementation
	template<typename Function>
	LambdaTask::LambdaTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, Function&& func)
		: ITask(mode, priority, ctx), m_func(std::forward<Function>(func)) {
	}
}
