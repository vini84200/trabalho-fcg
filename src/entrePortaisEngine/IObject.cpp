#include "entrePortaisEngine/IObject.hpp"
#include "utils/matrices.h"
#include "entrePortaisEngine/Logger.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "imgui.h"

void entre_portais::IObject::render() {
    if (!IsVisible()) return;
    Draw();
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
            if (mesh_ != nullptr) {
                mesh_->RenderImGui();
            } else {
                ImGui::Text("No mesh");
            }
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
    if (getParent() != nullptr) {
        IObject *parent = dynamic_cast<IObject *>(getParent().get());
        if (parent != nullptr) return visible_ && parent->IsVisible();
    }
    return visible_;
}

glm::mat4 &entre_portais::IObject::getModelMatrix() {
    return modelMatrix_;
}

entre_portais::IObject::~IObject() {
    if (mesh_ != nullptr) {
        mesh_.reset();
    }
    if (rigidBody_ != nullptr) {
        if (!scene_.expired())
            scene_.lock()->getPhysicsEngine()->removeRigidBody(rigidBody_.get());
        rigidBody_.reset();
    }
}

void entre_portais::IObject::onTransformChange() {
    if (rigidBody_ != nullptr) {
        rigidBody_->onChange();
    }

}
