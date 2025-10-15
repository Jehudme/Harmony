#include "pch.h"
#include "TaskManagement.h"
#include "Engine.h"
#include "Task.h"

namespace Harmony::Internals 
{
	////////////////////////////////////////////////////////////////
	// Definition                                                 //
	////////////////////////////////////////////////////////////////

	struct TaskManagement::WorkerPool {
	public:
		WorkerPool();
		~WorkerPool();

		void submit(std::unique_ptr<Task> task);

	private:
		struct Worker;

		std::mutex mutex_;
		std::condition_variable condition_;

		bool running_ = false;

		std::vector<Worker> workers_;
		std::queue<std::unique_ptr<Task>> tasks_;
	};

	struct TaskManagement::WorkerPool::Worker {
	public:
		friend struct WorkerPool;

		Worker(std::queue<std::unique_ptr<Task>>& tasks, bool& running_, std::mutex& mutex, std::condition_variable& condition);
		~Worker();

		// Prevent copying
		Worker(const Worker&) = delete;
		Worker& operator=(const Worker&) = delete;

		// Allow moving
		Worker(Worker&&) noexcept = default;
		Worker& operator=(Worker&&) noexcept = default;


	private:
		static void run(Worker& worker);

	private:
		std::queue<std::unique_ptr<Task>>& tasks_;
		std::unique_ptr<Task> currentTask_;

		std::thread thread_;
		bool& running_;

		std::mutex& mutex_;
		std::condition_variable& condition_;
	};

	/////////////////////////////////////////////////////////////////
	// Implementation                                              //
	/////////////////////////////////////////////////////////////////

	TaskManagement::WorkerPool::WorkerPool()
	{
		for (unsigned int workerIndex = 0; workerIndex < std::thread::hardware_concurrency() + 2; workerIndex++)
			workers_.emplace_back(tasks_, running_, mutex_, condition_);
	}

	TaskManagement::WorkerPool::~WorkerPool()
	{
		running_ = false;
		condition_.notify_all();
	}

	TaskManagement::WorkerPool::Worker::Worker(std::queue<std::unique_ptr<Task>>& tasks, bool& running_, std::mutex& mutex, std::condition_variable& condition) :
		tasks_(tasks), running_(running_), mutex_(mutex), condition_(condition), thread_(run, std::ref(*this))
	{
	}

	void TaskManagement::WorkerPool::submit(std::unique_ptr<Task> task) {
		std::lock_guard<std::mutex> lock(mutex_);
		tasks_.emplace(std::move(task));
		condition_.notify_one();
	}

	Harmony::Internals::TaskManagement::WorkerPool::Worker::~Worker()
	{
		if (thread_.joinable())
			thread_.join();
	}

	void TaskManagement::WorkerPool::Worker::run(Worker& worker)
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(worker.mutex_);
			worker.condition_.wait(lock, [&worker] { return !worker.running_ || !worker.tasks_.empty(); });

			if (!worker.running_ && worker.tasks_.empty())
				return;

			worker.currentTask_ = std::move(worker.tasks_.front());
			lock.unlock();

			worker.currentTask_->start();
		}
	}

	TaskManagement::TaskManagement(Engine& engine) :
		engine(engine), workerPool_(std::make_unique<WorkerPool>()) {}

	TaskManagement::~TaskManagement() = default;

	void TaskManagement::submit(std::unique_ptr<Task> task)
	{
		if (task->priority == 0)
		{
			handleTask(std::move(task));
			return;
		}

		tasks_.emplace(std::move(task));
	}

	void TaskManagement::handleTasks() 
	{
		while (!tasks_.empty())
		{
			std::unique_ptr<Task> task;
			{
				std::lock_guard<std::mutex> lock(mutex_);
				task = std::move(const_cast<std::unique_ptr<Task>&>(tasks_.top()));
				tasks_.pop();
			}

			handleTask(std::move(task));
		}
	}

	inline void TaskManagement::handleTask(std::unique_ptr<Task> task)
	{
		if (task->multiThreaded)
		{
			std::unique_ptr<Task> task = std::move(const_cast<std::unique_ptr<Task>&>(tasks_.top()));

			workerPool_->submit(std::move(task));
			return;
		}

		task->start();
	}
}

