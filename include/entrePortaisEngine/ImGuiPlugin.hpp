#ifndef ENTREPORTAIS_IMGUIPLUGIN_HPP
#define ENTREPORTAIS_IMGUIPLUGIN_HPP

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "IPlugin.hpp"

namespace entre_portais {
    class ImGuiPlugin : public IPlugin {
    public:
        ImGuiPlugin() = default;

        void onAttach() override;

        void onDetach() override;

        void update(float deltaTime) override;

        void onEvent(Event &event) override;

        void InitializeImGui();

        bool demoWindow_ = false;
    };
}

#endif //ENTREPORTAIS_IMGUIPLUGIN_HPP
