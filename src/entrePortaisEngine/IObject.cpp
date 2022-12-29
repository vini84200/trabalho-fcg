#include "entrePortaisEngine/IObject.hpp"
#include "utils/matrices.h"
#include "entrePortaisEngine/Logger.hpp"
#include "imgui.h"


void entre_portais::IObject::render() {
    // Set uniforms
    mesh_->UseShader();
    mesh_->GetShader()->setUniformMat4("model", transform_.getModelMatrix());
    mesh_->GetShader()->setUniformMat4("view", matrices::Matrix_Identity());
    mesh_->GetShader()->setUniformMat4("projection", matrices::Matrix_Identity());
    Draw();
    mesh_->UnbindShader();
}

std::shared_ptr<entre_portais::IScene> entre_portais::IObject::getScene() {
    if (scene_.expired()) {
        log("IObject::getScene() - WARN: Scene is expired");
        return nullptr;
    }
    return scene_.lock();
}

void entre_portais::IObject::setScene(std::shared_ptr<IScene> scene) {
    scene_ = scene;
}

bool entre_portais::IObject::hasScene() {
    return !scene_.expired();
}

void entre_portais::IObject::renderImGui() {
    if (ImGui::TreeNode(getName())) {
        if (ImGui::TreeNode("Transform")) {
            ImGui::DragFloat3("Position", transform_.getPositionPtr(), 0.1f);
            ImGui::DragFloat3("Rotation", transform_.getRotationPtr(), 0.1f);
            ImGui::DragFloat3("Scale", transform_.getScalePtr(), 0.1f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Mesh")) {
            ImGui::Text("Mesh GUI not implemented");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Children")) {
            for (auto &child: children_) {
                child->renderImGui();
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

}
