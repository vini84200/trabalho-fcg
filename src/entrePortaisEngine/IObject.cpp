#include "entrePortaisEngine/IObject.hpp"
#include "utils/matrices.h"
#include "entrePortaisEngine/Logger.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "imgui.h"

void entre_portais::IObject::render(RenderPass current_pass) {
    if (!IsVisible()) return;
    if (current_pass == RenderPass::FOREGROUND) {
        if (mesh_ != nullptr) {
            mesh_->Draw(getShader(), current_pass);
        }
    }
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
                mesh_->RenderImGui(nullptr);
            } else {
                ImGui::Text("No mesh");
            }
            ImGui::TreePop();
        }
        if (rigidBody_ != nullptr) {
            if (ImGui::TreeNode("RigidBody")) {
                rigidBody_->renderImGui(this->scene_.lock()->getCamera().get());
                ImGui::TreePop();
            }
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

const std::shared_ptr<entre_portais::IMesh> &entre_portais::IObject::getMesh() const {
    return mesh_;
}

void entre_portais::IObject::setMesh(const std::shared_ptr<IMesh> &mesh) {
    mesh_ = mesh;
}

const std::unique_ptr<entre_portais::RigidBody> &entre_portais::IObject::getRigidBody() const {
    return rigidBody_;
}

void entre_portais::IObject::setRigidBody(std::unique_ptr<RigidBody> rigidBody) {
    rigidBody_ = std::move(rigidBody);
    rigidBody_->setTransformPtr(&modelMatrix_);
}
