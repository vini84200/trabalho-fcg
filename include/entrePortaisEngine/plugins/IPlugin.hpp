#ifndef ENTREPORTAIS_IPLUGIN_HPP
#define ENTREPORTAIS_IPLUGIN_HPP

#include "entrePortaisEngine/Event.hpp"
#include "entrePortaisEngine/Logger.hpp"
#include <memory>

namespace entre_portais {
    class Window;

    class IPlugin {
    public:
        virtual ~IPlugin() = default;

        virtual void onAttach() = 0;

        void SetWindow(const std::shared_ptr<Window> &window);

        virtual void onDetach() = 0;

        virtual void update(float deltaTime) = 0;

        virtual void render() = 0;

        virtual void onEvent(Event &event) = 0;

        virtual void onKey(int key, int scancode, int action, int mods) {};

        std::shared_ptr<spdlog::logger> getLogger() {
            return logger_.getLogger();
        }

    protected:
        std::weak_ptr<Window> window_;
        Logger logger_ = Logger("IPlugin");
    };
}

#endif //ENTREPORTAIS_IPLUGIN_HPP
