#include <csignal>
#include "testGame/WaitAndFinishTask.hpp"
#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {
    TaskRunResult WaitAndFinishTask::Run() {
        usleep(5000000); // 5 seconds
        {
            std::lock_guard<std::mutex> lock(mutex_);
            executionsLeft_--;
            if (executionsLeft_ == 0) {
                return TaskRunResult::SUCCESS;
            }
        }
        return TaskRunResult::REPEAT;
    }

    void WaitAndFinishTask::OnFinish() {
        spdlog::info("Task finished");
    }

    void WaitAndFinishTask::OnCancel() {
        spdlog::info("Task canceled");
    }

} // entre_portais