#ifndef ENTREPORTAIS_LAMBDATASK_HPP
#define ENTREPORTAIS_LAMBDATASK_HPP

#include <functional>
#include <variant>
#include "ITask.hpp"

namespace entre_portais {
    struct EmptyState {
    };

    template<typename State>
    class LambdaTask : public entre_portais::ITask {
    public:
        LambdaTask(std::function<entre_portais::TaskRunResult(State & )> lambda, const std::string name,
                   State initialState) : lambda_(lambda), state_(initialState) {
            setTaskName(name);
        }

        LambdaTask(std::function<entre_portais::TaskRunResult(State & )> lambda, const std::string name) : lambda_(
                lambda), state_(State()) {
            setTaskName(name);
        }


        entre_portais::TaskRunResult Run() override {
            std::lock_guard<std::mutex> lock(stateMutex_);
            return lambda_(state_);
        }

        void OnFinish() override {

        }

        void OnCancel() override {

        }

        State getState() {
            std::lock_guard<std::mutex> lock(stateMutex_);
            return state_;
        }

        void setState(State state) {
            std::lock_guard<std::mutex> lock(stateMutex_);
            state_ = state;
        }

    private:
        std::function<entre_portais::TaskRunResult(State & )> lambda_;
        State state_;
        std::mutex stateMutex_;
    };
}

#endif //ENTREPORTAIS_LAMBDATASK_HPP
