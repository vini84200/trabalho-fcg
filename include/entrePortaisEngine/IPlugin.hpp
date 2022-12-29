#ifndef ENTREPORTAIS_IPLUGIN_HPP
#define ENTREPORTAIS_IPLUGIN_HPP

#include "Event.hpp"
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

        virtual void onEvent(Event &event) = 0;

    protected:
        std::weak_ptr<Window> window_;
    };
}

#endif //ENTREPORTAIS_IPLUGIN_HPP
