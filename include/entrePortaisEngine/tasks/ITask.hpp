#ifndef ENTREPORTAIS_ITASK_HPP
#define ENTREPORTAIS_ITASK_HPP

#include <mutex>

namespace entre_portais {
    enum class TaskStatus {
        WAITING,
        RUNNING,
        SUCCESS,
        FAILURE,
        CANCLELED,
        BLOCKED
    };


    enum class TaskPriority {
        LOW,
        MEDIUM,
        HIGH
    };

    enum class TaskRunResult {
        SUCCESS,
        FAILURE,
        BLOCKED,
        REPEAT
    };


    class ITask {
    public:
        virtual ~ITask() = default;

        virtual TaskRunResult Run() = 0;

        virtual void OnFinish() = 0;

        virtual void OnCancel() = 0;


        void setStatus(TaskStatus status) {
            std::lock_guard<std::mutex> lock(mutex_);
            status_ = status;
        }

        TaskStatus getStatus() {
            std::lock_guard<std::mutex> lock(mutex_);
            return status_;
        }

        void setID(int id) {
            std::lock_guard<std::mutex> lock(mutex_);
            id_ = id;
        }

        std::mutex mutex_;
    private:
        TaskStatus status_ = TaskStatus::RUNNING;
        int id_;
    };
}
#endif //ENTREPORTAIS_ITASK_HPP
