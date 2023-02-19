#include <memory>
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/plugins/ImGuiPlugin.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"

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

void entre_portais::ImGuiPlugin::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (debugConfigWindow_) {
        renderDebugConfigWindow();
    }

    // Mostra janela para a cena atual
    if (sceneDebugWindow_) {
        auto scene = window_.lock()->GetScene();
        if (scene) {
            scene->renderImGui(&sceneDebugWindow_);
        }
    }

    // Mostra janela de debug
    if (demoWindow_) {
        ImGui::ShowDemoWindow(&demoWindow_);
    }

    // Mostra janela de performance
    if (mestricsWindow_) {
        ImGui::ShowMetricsWindow(&mestricsWindow_);
    }

    if (taskManagerWindow_) {
        auto taskManager = entre_portais::TaskManager::getInstance();
        renderTaskManagerImGui(taskManager);
    }

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

void entre_portais::ImGuiPlugin::renderTaskManagerImGui(entre_portais::TaskManager *pManager) {
    // Create window
    ImGui::Begin("Task Manager", &taskManagerWindow_);
    // Threads list
    ImGui::Text("Worker Threads");

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("Threads", ImVec2(0, 230), true, window_flags);
    for (auto &worker: pManager->getWorkers()) {
        ImGui::Text("Thread %d", worker->id);
        ImGui::SameLine();
        ImGui::Text("Running: %s", worker->isRunning ? "true" : "false");
        ImGui::SameLine();
        auto currentTask = worker->currentTask;
        if (currentTask) {
            ImGui::Text("Task: %s", currentTask->getTaskName().c_str());
        } else {
            ImGui::Text("Task: None");
        }
    }
    ImGui::EndChild();

    // Size of the queue
    ImGui::Text("Queued Tasks:");
    ImGui::Text("Tasks in the queue: %d", pManager->getQueue().size());

    ImGui::End();
}

void entre_portais::ImGuiPlugin::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        debugConfigWindow_ = !debugConfigWindow_;
    }
}

void entre_portais::ImGuiPlugin::renderDebugConfigWindow() {
    ImGui::Begin("Debug", &debugConfigWindow_);

    ImGui::Checkbox("Mostar janela da Scene atual", &sceneDebugWindow_);
    ImGui::Checkbox("Mostar janela de Demo ImGui", &demoWindow_);
    ImGui::Checkbox("Mostar janela de Performance ImGui", &mestricsWindow_);
    ImGui::Checkbox("Mostar janela de Task Manager", &taskManagerWindow_);
    ImGui::Checkbox("Mostar janela de Debug(this)", &debugConfigWindow_);

    ImGui::Separator();
    // Titulo: Métricas de performance
    ImGui::Text("Métricas de performance");
    // FPS
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    // TODO: Calcular UPS

    ImGui::End();
}

void entre_portais::ImGuiPlugin::update(float deltaTime) {
}

