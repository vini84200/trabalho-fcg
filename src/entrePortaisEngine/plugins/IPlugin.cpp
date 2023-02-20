#include "entrePortaisEngine/plugins/IPlugin.hpp"

void entre_portais::IPlugin::SetWindow(const std::shared_ptr<entre_portais::Window> &window) {
    window_ = window;
}
