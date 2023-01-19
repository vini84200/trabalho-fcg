#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {
    TaskManager *TaskManager::instance_ = nullptr;


    void TaskManager::removeTask(TaskHandler &taskHandler) {
        tasks_.erase(taskHandler);

        // TODO: Cancelar a task
        logger_.getLogger()->warn("TaskManager::removeTask() not implemented yet");
    }

    TaskStatus TaskManager::isFinished(TaskHandler &taskHandler) {
        if (tasks_.find(taskHandler) != tasks_.end()) {
            return tasks_[taskHandler]->getStatus();
        }
        throw std::runtime_error("Task not found");
    }

    TaskManager::TaskManager() {
        Initialize();
    }

    void TaskManager::Initialize() {
        // Initialize the thread pool
        int numThreads = std::thread::hardware_concurrency();
        for (int i = 0; i < numThreads; i++) {
            std::shared_ptr<WorkerStruct> signal = std::make_shared<WorkerStruct>(i);
            std::thread thread(&TaskManager::WorkerThread, this, std::ref(*signal));
            {
                std::lock_guard<std::mutex> lock(signal->mutex);
                signal->thread = std::move(thread);
            }
            threads_.push_back(signal);
        }

    }

    void TaskManager::WorkerThread(WorkerStruct &signal) {
        bool continueRunning = true;
        char name[80];
        {
            std::unique_lock<std::mutex> lock(signal.mutex);
            sprintf(name, "WorkerThread-%d", signal.id);
        }
        Logger logger(name);
        {
            std::unique_lock<std::mutex> lock(signal.mutex);
            signal.isRunning = 1;
        }
        logger.getLogger()->info("Worker thread started");
        while (continueRunning) {
            TaskHandler taskHandler = GetNextTask();
            ITask *task = GetTask(taskHandler);
            {
                std::unique_lock<std::mutex> lock(signal.mutex);
                signal.currentTask = task;
            }
            if (task == nullptr) {
                logger.getLogger()->info("Task is null");
                continue;
            }
            logger.getLogger()->info("Running task {}", taskHandler.getTaskID());
            TaskRunResult result;
            try {
                result = task->Run();
                logger.getLogger()->info("Task {} finished", taskHandler.getTaskID());
                OnTaskFinished(taskHandler);
            } catch (std::exception &e) {
                logger.getLogger()->error("Exception in task {}: {}", taskHandler.getTaskID(), e.what());
                result = TaskRunResult::FAILURE;
            } catch (...) {
                logger.getLogger()->error("Unknown exception in task {}", taskHandler.getTaskID());
                result = TaskRunResult::FAILURE;
            }

            switch (result) {
                case TaskRunResult::SUCCESS:
                    task->setStatus(TaskStatus::SUCCESS);
                    task->OnFinish();
                    break;
                case TaskRunResult::FAILURE:
                    task->setStatus(TaskStatus::FAILURE);
                    break;
                case TaskRunResult::BLOCKED:
                    task->setStatus(TaskStatus::BLOCKED);
                    // TODO: Verificar se a task está bloqueada por tempo ou por outra task
                    // e colocar na fila de acordo
                    break;
                case TaskRunResult::REPEAT:
                    task->setStatus(TaskStatus::RUNNING);
                    {
                        std::lock_guard<std::mutex> lock(queueMutex_);
                        taskQueue_.push(taskHandler);
                        queueCondition_.notify_one();
                    }
                    break;
            }
            {
                std::unique_lock<std::mutex> lock(signal.mutex);
                signal.currentTask = nullptr;

            }

            {
                std::unique_lock<std::mutex> lock(signal.mutex);
                signal.currentTask = nullptr;
            }

        }

    }

    TaskHandler TaskManager::GetNextTask() {
        std::unique_lock<std::mutex> lock(queueMutex_);
        queueCondition_.wait(lock, [this] { return !taskQueue_.empty(); });
        TaskHandler taskHandler = taskQueue_.front();
        taskQueue_.pop();
        return taskHandler;
    }

    TaskManager::~TaskManager() {

    }

    void TaskManager::OnTaskFinished(TaskHandler &taskHandler) {
        ITask *task = GetTask(taskHandler);
        if (task == nullptr) {
            return;
        }
        task->setStatus(TaskStatus::SUCCESS);

    }

    ITask *TaskManager::GetTask(TaskHandler &taskHandler) {
        if (tasks_.find(taskHandler) != tasks_.end()) {
            return tasks_[taskHandler];
        }
        return nullptr;
    }

} // entre_portais