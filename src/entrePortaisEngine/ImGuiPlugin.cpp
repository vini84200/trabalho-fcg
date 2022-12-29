#include <cstdio>
#include <memory>
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/ImGuiPlugin.hpp"

void entre_portais::ImGuiPlugin::onAttach() {
    printf("ImGuiPlugin::onAttach\n");
    InitializeImGui();

}

void entre_portais::ImGuiPlugin::onDetach() {
    printf("ImGuiPlugin::onDetach\n");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void entre_portais::ImGuiPlugin::update(float deltaTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    window_.lock()->GetScene()->renderImGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void entre_portais::ImGuiPlugin::onEvent(Event &event) {
    printf("ImGuiPlugin::onEvent\n");

}

void entre_portais::ImGuiPlugin::InitializeImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;


    ImGui::StyleColorsDark();

    if (window_.expired()) {
        printf("ERROR: Window expired");
        return;
    }
    GLFWwindow *window = window_.lock()->GetGLFWwindow();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
