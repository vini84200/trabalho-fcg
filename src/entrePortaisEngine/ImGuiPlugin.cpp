#include <cstdio>
#include <memory>
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/ImGuiPlugin.hpp"
#include "spdlog/spdlog.h"

void entre_portais::ImGuiPlugin::onAttach() {
    getLogger()->trace("onAttach()");
    InitializeImGui();

}

void entre_portais::ImGuiPlugin::onDetach() {
    getLogger()->trace("onDettach()");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void entre_portais::ImGuiPlugin::update(float deltaTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Mostra janela para a cena atual
    window_.lock()->GetScene()->renderImGui();

    // Mostra janela de debug
    if (demoWindow_) {
        ImGui::ShowDemoWindow(&demoWindow_);
    }

    // Mostra janela de performance
    ImGui::ShowMetricsWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void entre_portais::ImGuiPlugin::onEvent(Event &event) {
}

void entre_portais::ImGuiPlugin::InitializeImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;


    ImGui::StyleColorsDark();

    if (window_.expired()) {
        getLogger()->error("Window is expired");
        return;
    }
    GLFWwindow *window = window_.lock()->GetGLFWwindow();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

entre_portais::ImGuiPlugin::ImGuiPlugin() {
    logger_ = Logger("ImGuiPlugin");
    getLogger()->trace("ImGuiPlugin()");

}
