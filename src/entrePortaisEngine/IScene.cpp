#include "entrePortaisEngine/IScene.hpp"

void entre_portais::IScene::renderImGui(bool *p_open) {
    if (ImGui::Begin(getName(), p_open)) {
        ImGui::Text("Scene");
        if (ImGui::CollapsingHeader("Objects")) {
            for (auto &child: children_) {
                child->renderImGui(nullptr);
            }
        }
        ImGui::End();
    } else {
        ImGui::End();
    }
}
