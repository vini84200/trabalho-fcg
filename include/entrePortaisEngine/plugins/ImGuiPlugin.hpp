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

        void render() override;

        void onEvent(Event &event) override;

        bool onKey(int key, int scancode, int action, int mods) override;

        bool onMouseButton(int button, int action, int mods) override;

        bool onMouseMove(double xpos, double ypos) override;

        void InitializeImGui();

        void renderDebugConfigWindow();

        void renderTaskManagerImGui(entre_portais::TaskManager *pManager);

    private:

        bool debugConfigWindow_ = false;
        bool demoWindow_ = false;
        bool taskManagerWindow_ = false;
        bool mestricsWindow_ = false;
        bool sceneDebugWindow_ = false;
    };
}

#endif //ENTREPORTAIS_IMGUIPLUGIN_HPP
