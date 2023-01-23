//
// Created by vini84200 on 1/19/23.
//

#include "testGame/DependencyTask.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"

namespace entre_portais {
    TaskRunResult DependencyTask::Run() {
        if (createdTask_ == false) {
            spdlog::info("Creating dependency task");
            sleep(3);
            auto taskManager = entre_portais::TaskManager::getInstance();
            waitAndFinishTask_ = taskManager->addTask<WaitAndFinishTask>();
            addDependency(waitAndFinishTask_.value().getTaskID());
            createdTask_ = true;
            return TaskRunResult::BLOCKED;
        }
        if (waitAndFinishTask_.value().getStatus() == TaskStatus::SUCCESS) {
            spdlog::info("Dependency task finished");
            sleep(5);
            return TaskRunResult::SUCCESS;
        }

        if (waitAndFinishTask_.value().getStatus() == TaskStatus::FAILURE) {
            return TaskRunResult::FAILURE;
        }

        if (waitAndFinishTask_.value().getStatus() == TaskStatus::CANCELED) {
            return TaskRunResult::FAILURE;
        }

        if (waitAndFinishTask_.value().getStatus() == TaskStatus::BLOCKED) {
            spdlog::error("Dependency task is blocked, and this should not run if the dependency is not finished");
            return TaskRunResult::FAILURE;
        }

        if (waitAndFinishTask_.value().getStatus() == TaskStatus::RUNNING) {
            spdlog::error("Dependency task is running, and this should not run if the dependency is not finished");
            return TaskRunResult::FAILURE;
        }

        if (waitAndFinishTask_.value().getStatus() == TaskStatus::WAITING) {
            spdlog::error("Dependency task is waiting, and this should not run if the dependency is not finished");
            return TaskRunResult::FAILURE;
        }

    }

    void DependencyTask::OnFinish() {
        spdlog::info("Dependency task finished");
    }

    void DependencyTask::OnCancel() {

    }
} // entre_portais