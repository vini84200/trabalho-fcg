#include "entrePortaisEngine/IObject.hpp"
#include "utils/matrices.h"
#include "entrePortaisEngine/Logger.hpp"
#include "imgui.h"

void entre_portais::IObject::render() {
    // Set uniforms
    mesh_->UseShader();
    glm::mat4 modelMatrix;
    if (getParentModelMatrix() != nullptr) {
        modelMatrix = (*getParentModelMatrix()) * transform_.getModelMatrix();
    } else {
        modelMatrix = transform_.getModelMatrix();
    }
    mesh_->GetShader()->setUniformMat4("model", modelMatrix);
    Draw();
    mesh_->UnbindShader();
}

std::shared_ptr<entre_portais::IScene> entre_portais::IObject::getScene() {
    if (scene_.expired()) {
        throw std::runtime_error("A cena não existe mais.");
    }
    return scene_.lock();
}

void entre_portais::IObject::setScene(std::shared_ptr<IScene> scene) {
    scene_ = scene;
}

bool entre_portais::IObject::hasScene() {
    return !scene_.expired();
}

void entre_portais::IObject::renderImGui(bool *p_open) {
    if (ImGui::TreeNode(getName())) {
        ImGui::Checkbox("Visible", &visible_);
        transform_.renderImGui();
        if (ImGui::TreeNode("Mesh")) {
            ImGui::Text("Mesh GUI not implemented");
            ImGui::TreePop();
        }
        CustomImGui();
        if (ImGui::TreeNode("Children")) {
            for (auto &child: children_) {
                child->renderImGui(nullptr);
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

}

void entre_portais::IObject::SetVisibility(bool visible) {
    visible_ = visible;
}

void entre_portais::IObject::Hide() {
    visible_ = false;
}

void entre_portais::IObject::Show() {
    visible_ = true;

}

void entre_portais::IObject::ToggleVisibility() {
    visible_ = !visible_;
}

bool entre_portais::IObject::IsVisible() {
    return visible_;
}