//
// Created by vini84200 on 1/19/23.
//

#ifndef ENTREPORTAIS_DEPENDENCYTASK_HPP
#define ENTREPORTAIS_DEPENDENCYTASK_HPP

#include <memory>
#include <optional>
#include "entrePortaisEngine/tasks/ITask.hpp"
#include "entrePortaisEngine/tasks/TaskHandler.hpp"
#include "testGame/WaitAndFinishTask.hpp"

namespace entre_portais {

    class DependencyTask : public ITask {
    public:
        DependencyTask() {};

        TaskRunResult Run() override;

        void OnFinish() override;

        void OnCancel() override;

    private:
        bool createdTask_ = false;
        std::optional<TaskHandler> waitAndFinishTask_;
    };

} // entre_portais

#endif //ENTREPORTAIS_DEPENDENCYTASK_HPP
