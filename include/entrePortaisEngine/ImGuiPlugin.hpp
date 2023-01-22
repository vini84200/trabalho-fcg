#ifndef ENTREPORTAIS_IMGUIPLUGIN_HPP
#define ENTREPORTAIS_IMGUIPLUGIN_HPP

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "IPlugin.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"

namespace entre_portais {
    class ImGuiPlugin : public IPlugin {
    public:
        ImGuiPlugin();

        void onAttach() override;

        void onDetach() override;

        void update(float deltaTime) override;

        void onEvent(Event &event) override;

        void InitializeImGui();

        void renderTaskManagerImGui(entre_portais::TaskManager *pManager);

        bool demoWindow_ = false;
        bool taskManagerWindow_ = true;

    };
}

#endif //ENTREPORTAIS_IMGUIPLUGIN_HPP
