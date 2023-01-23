#ifndef ENTREPORTAIS_ITASK_HPP
#define ENTREPORTAIS_ITASK_HPP

#include <mutex>
#include <vector>
#include "imgui.h"

namespace entre_portais {
    class TaskHandler;


    enum class TaskStatus {
        WAITING,
        RUNNING,
        SUCCESS,
        FAILURE,
        CANCELED,
        BLOCKED
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

        int getID() {
            std::lock_guard<std::mutex> lock(mutex_);
            return id_;
        }

        virtual std::vector<int> getDependencies() {
            std::lock_guard<std::mutex> lock(mutex_);
            return dependencies_;
        }

        virtual void addDependency(int taskId) {
            std::lock_guard<std::mutex> lock(mutex_);
            dependencies_.push_back(taskId);
            blockCount_++;
        }

        virtual void unblockDependency(int taskId) {
            std::lock_guard<std::mutex> lock(mutex_);
            for (auto it = dependencies_.begin(); it != dependencies_.end(); it++) {
                if (*it == taskId) {
                    dependencies_.erase(it);
                    blockCount_--;
                    break;
                }
            }
        }

        virtual int getBlockCount() {
            std::lock_guard<std::mutex> lock(mutex_);
            return blockCount_;
        }

        void setTaskName(std::string name) {
            std::lock_guard<std::mutex> lock(mutex_);
            name_ = name;
        }

        std::string getTaskName() {
            std::lock_guard<std::mutex> lock(mutex_);
            return name_;
        }

        std::mutex mutex_;
        std::mutex runMutex_;
    protected:
        std::string name_ = "Unnamed Task";
    private:
        TaskStatus status_ = TaskStatus::RUNNING;
        int id_;
        int blockCount_ = 0;
        std::vector<int> dependencies_;
    };
}
#endif //ENTREPORTAIS_ITASK_HPP
