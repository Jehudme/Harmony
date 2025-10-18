#include "pch.h"
#include "TaskManagement.h"
#include "Engine.h"
#include "Task.h"

namespace Harmony::Management
{
	////////////////////////////////////////////////////////////////
	// Definition                                                 //
	////////////////////////////////////////////////////////////////

	struct TaskManagemer::WorkerPool {
	public:
		WorkerPool();
		~WorkerPool();

		void submit(std::unique_ptr<Tasks::Task> task);

	private:
		struct Worker;

		std::mutex mutex_;
		std::condition_variable condition_;

		bool running_ = true;

		std::queue<std::unique_ptr<Tasks::Task>> tasks_;
		std::vector<std::unique_ptr<Worker>> workers_;
	};

	struct TaskManagemer::WorkerPool::Worker {
	public:
		friend struct WorkerPool;

		Worker(std::queue<std::unique_ptr<Tasks::Task>>& tasks, bool& running_, std::mutex& mutex, std::condition_variable& condition);
		~Worker();

		// Prevent copying
		Worker(const Worker&) = delete;
		Worker& operator=(const Worker&) = delete;

		// Allow moving
		Worker(Worker&&) noexcept = default;
		Worker& operator=(Worker&&) noexcept = default;


	private:
		static void run(Worker& worker);
		static void runTask(std::unique_ptr<Tasks::Task> task);

	private:
		std::queue<std::unique_ptr<Tasks::Task>>& tasks_;
		std::unique_ptr<Tasks::Task> currentTask_;

		std::mutex& mutex_;
		std::condition_variable& condition_;

		bool& running_;
		std::thread thread_;
	};

	/////////////////////////////////////////////////////////////////
	// Implementation                                              //
	/////////////////////////////////////////////////////////////////

	TaskManagemer::WorkerPool::WorkerPool()
	{
		for (unsigned int workerIndex = 0; workerIndex < std::thread::hardware_concurrency() + 2; workerIndex++)
		{
			std::unique_ptr<Worker> worker = std::make_unique<Worker>(tasks_, running_, mutex_, condition_);
			workers_.emplace_back(std::move(worker));
		}
	}

	TaskManagemer::WorkerPool::~WorkerPool()
	{
		running_ = false;
		condition_.notify_all();
	}

	TaskManagemer::WorkerPool::Worker::Worker(std::queue<std::unique_ptr<Tasks::Task>>& tasks, bool& running_, std::mutex& mutex, std::condition_variable& condition) :
		tasks_(tasks), 
		running_(running_), 
		mutex_(mutex), 
		condition_(condition), 
		thread_(run, std::ref(*this))
	{
	}

	void TaskManagemer::WorkerPool::submit(std::unique_ptr<Tasks::Task> task)
	{
		{
			std::lock_guard<std::mutex> lock(mutex_);
			tasks_.emplace(std::move(task));
		}
		condition_.notify_one();
	}

	Harmony::Management::TaskManagemer::WorkerPool::Worker::~Worker()
	{
		if (thread_.joinable())
			thread_.join();
	}

	void TaskManagemer::WorkerPool::Worker::run(Worker& worker)
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(worker.mutex_);
			worker.condition_.wait(lock, [&worker] { return !worker.running_ || !worker.tasks_.empty(); });

			if (!worker.running_ && worker.tasks_.empty())
				return;

			worker.currentTask_ = std::move(worker.tasks_.front());
			worker.tasks_.pop();
			lock.unlock();
			
			switch (worker.currentTask_->mode)
			{
			case Tasks::Task::FastMultiThreaded:
				runTask(std::move(worker.currentTask_)); break;

			case Tasks::Task::SlowMultiThreaded:
				std::thread(Worker::runTask, std::move(worker.currentTask_)).detach(); break;
			}
		}
	}

	void TaskManagemer::WorkerPool::Worker::runTask(std::unique_ptr<Tasks::Task> task)
	{
		task->start();
	}

	TaskManagemer::TaskManagemer(Engine& engine) :
		engine(engine), workerPool_(std::make_unique<WorkerPool>()) {}

	TaskManagemer::~TaskManagemer() = default;

	void TaskManagemer::submit(std::unique_ptr<Tasks::Task> task)
	{
		task->engine = std::make_optional<std::reference_wrapper<Engine>>(engine);

		if (task->priority == 0)
		{
			handleTask(std::move(task));
			return;
		}

		tasks_.emplace(std::move(task));
	}

	void TaskManagemer::handleTasks() 
	{
		while (!tasks_.empty())
		{
			std::unique_ptr<Tasks::Task> task;
			{
				std::lock_guard<std::mutex> lock(mutex_);
				task = std::move(const_cast<std::unique_ptr<Tasks::Task>&>(tasks_.top()));
				tasks_.pop();
			}

			handleTask(std::move(task));
		}
	}

	inline void TaskManagemer::handleTask(std::unique_ptr<Tasks::Task> task)
	{
		switch (task->mode)
		{
			case Tasks::Task::SingleThreaded:
				task->start();
				break;

			case Tasks::Task::FastMultiThreaded || Tasks::Task::SlowMultiThreaded:
				workerPool_->submit(std::move(task));
				break;

		default:
			break;
		}
	}
}

