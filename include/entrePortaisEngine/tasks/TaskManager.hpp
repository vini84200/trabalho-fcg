#ifndef ENTREPORTAIS_TASKMANAGER_HPP
#define ENTREPORTAIS_TASKMANAGER_HPP

#include <map>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
#include "ITask.hpp"
#include "TaskHandler.hpp"
#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {


    struct WorkerStruct {
        int id;
        std::thread thread;
        bool isRunning = false;
        std::mutex mutex;
        ITask *currentTask;

        WorkerStruct(int id) : id(id) {}

    };

    /** Task Manager
     * É um gerenciador de tarefas de longa duração que serão executadas
     * em outras threads. É um singleton.
     * Para adicionar uma tarefa, use o método addTask.
     * Para remover uma tarefa, use o método removeTask.
     *
     * Para usar criar uma tarefa, crie uma classe que herde de Task.
     *
     * Exemplo:
     * ``` cpp
     * class MyTask : public Task {
     * public:
     *    MyTask() : Task() {}
     *    ~MyTask() override = default;
     *    void Run() override {
     *    // Faz alguma coisa
     *    }
     *    void OnFinish() override {
     *    // Faz alguma coisa quando a tarefa termina
     *    }
     *    void OnCancel() override {
     *    // Faz alguma coisa quando a tarefa é cancelada
     *    }
     *
     *    };
     *    ```
     *
     * */
    class TaskManager {
    public:
        static TaskManager *getInstance() {
            if (instance_ == nullptr) {
                instance_ = new TaskManager();
            }
            return instance_;
        }

        ~TaskManager();

        // Remove construtores e operadores de cópia e movimentação
        TaskManager(const TaskManager &other) = delete;

        TaskManager &operator=(const TaskManager &other) = delete;

        TaskManager(TaskManager &&other) = delete;

        TaskManager &operator=(TaskManager &&other) = delete;

        // Adds a task to the task manager, using the taskType and its args,
        // and returns a TaskHandler to the task.
        template<typename T, typename... Args>
        TaskHandler addTask(Args &&... args) {
            static_assert(std::is_base_of<ITask, T>::value, "T must inherit from ITask");
            int taskID;

            {
                std::lock_guard<std::mutex> lock(nextTaskIDMutex_);
                taskID = nextTaskID_++;
            }

            T *task = new T(std::forward<Args>(args)...);
            TaskHandler taskHandler(taskID, task);

            {
                std::lock_guard<std::mutex> lock(tasksMutex_);
                tasks_[taskHandler] = task;
            }

            {
                std::lock_guard<std::mutex> lock(queueMutex_);
                taskQueue_.push(taskHandler);
                queueCondition_.notify_one();
            }

            logger_.getLogger()->info("Task added to the queue");
            task->setStatus(TaskStatus::WAITING); // setStatus is thread-safe
            task->setID(taskID); // setID is thread-safe
            return taskHandler;
        }

        void removeTask(TaskHandler &taskHandler);

        TaskStatus isFinished(TaskHandler &taskHandler);

    private:
        TaskManager();

        void Initialize();

        void OnTaskFinished(TaskHandler &taskHandler);

        void WorkerThread(WorkerStruct &signal);

        TaskHandler GetNextTask();

        ITask *GetTask(TaskHandler &taskHandler);

        TaskHandler GetTaskHandler(int id);

        static TaskManager *instance_;
        std::map<TaskHandler, ITask *> tasks_;
        std::mutex tasksMutex_;
        int nextTaskID_ = 0;
        std::mutex nextTaskIDMutex_;

        // Thread pool
        std::vector<std::shared_ptr<WorkerStruct>> threads_;

        // Thread safe queue
        std::queue<TaskHandler> taskQueue_;
        std::mutex queueMutex_;
        std::condition_variable queueCondition_;

        // Finished tasks
        std::queue<TaskHandler> finishedTasksQueue_;
        std::mutex finishedTasksMutex_;

        // Blocking queue
        std::map<TaskHandler, std::vector<TaskHandler>> blockingTasks_ = {};
        std::mutex blockedTasksMutex_;

        // Logger
        Logger logger_ = Logger("TaskManager");
    };


} // entre_portais

#endif //ENTREPORTAIS_TASKMANAGER_HPP
