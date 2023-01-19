//
// Created by vini84200 on 1/19/23.
//

#ifndef ENTREPORTAIS_WAITANDFINISHTASK_HPP
#define ENTREPORTAIS_WAITANDFINISHTASK_HPP

#include "entrePortaisEngine/tasks/ITask.hpp"

namespace entre_portais {

    class WaitAndFinishTask : public ITask {
    public:
        ~WaitAndFinishTask() override = default;

        TaskRunResult Run() override;

        void OnFinish() override;

        void OnCancel() override;

    private:
        int executionsLeft_ = 5;
    };

} // entre_portais

#endif //ENTREPORTAIS_WAITANDFINISHTASK_HPP
