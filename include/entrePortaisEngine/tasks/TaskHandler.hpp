#ifndef ENTREPORTAIS_TASKHANDLER_HPP
#define ENTREPORTAIS_TASKHANDLER_HPP

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

        TaskHandler(int taskID, class ITask *task) : taskID_(taskID), task_(task) {}

        // Copy
        TaskHandler(const TaskHandler &other) : taskID_(other.taskID_), task_(other.task_) {}

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

    private:
        int taskID_;
        ITask *task_;
    };

} // entre_portais

#endif //ENTREPORTAIS_TASKHANDLER_HPP
