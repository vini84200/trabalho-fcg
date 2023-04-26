#ifndef ENTREPORTAIS_TASKHANDLER_HPP
#define ENTREPORTAIS_TASKHANDLER_HPP

#include "ITask.hpp"

namespace entre_portais {

/** Task Handler
 ** Uma referencia para referenciar uma task
 * @param taskID: ID da task
 * @param task: Ponteiro para a task
 */
    class TaskHandler {
    public:
        TaskHandler() {

        }

        TaskHandler(int taskID, class ITask *task) : taskID_(taskID), task_(task), sync_(false) {
            queueTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
        }

        TaskHandler(int taskID, class ITask *task, bool sync) : taskID_(taskID), task_(task), sync_(sync) {
            queueTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
        }

        // Copy
        TaskHandler(const TaskHandler &other) : taskID_(other.taskID_), task_(other.task_) {
            queueTime_ = other.queueTime_;
        }

        bool operator==(const TaskHandler &other) const {
            return taskID_ == other.taskID_;
        }

        bool operator<(const TaskHandler &other) const {
            return taskID_ < other.taskID_;
        }

        int getTaskID() const {
            return taskID_;
        }

        ITask *getTask() const {
            return task_;
        }

        TaskStatus getStatus() const {
            return task_->getStatus();
        }

        bool isSync() const {
            return sync_;
        }

        long int getQueueTime() const {
            return queueTime_;
        }

        float getWaitTime() const {
            long int const currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            return (static_cast<float>(currentTime - queueTime_)) / 1000.0f;
        }

    private:
        int taskID_;
        ITask *task_;
        bool sync_;
        long int queueTime_;
    };

} // entre_portais

#endif //ENTREPORTAIS_TASKHANDLER_HPP
