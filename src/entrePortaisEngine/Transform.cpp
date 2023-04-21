#include "entrePortaisEngine/Transform.hpp"

#include "utils/matrices.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"

glm::mat4 entre_portais::Transform::getLocalModelMatrix() const {
    glm::mat4 model = matrices::Matrix_Identity();
    model = matrices::Matrix_Scale(scale_.x, scale_.y, scale_.z) * model;
    model = glm::toMat4(rotation_) * model;
    model = matrices::Matrix_Translate(position_.x, position_.y, position_.z) * model;
    return model;
}

void entre_portais::Transform::setPosition(glm::vec3 position) {
    position_ = position;
    handleTransformChange();
}

entre_portais::Transform::Transform() {
    position_ = glm::vec3(0, 0, 0);
    rotation_ = glm::quat(1, 0, 0, 0);
    scale_ = glm::vec3(1, 1, 1);
    handleTransformChange();
}

void entre_portais::Transform::setScale(glm::vec3 scale) {
    scale_ = scale;
    handleTransformChange();
}

void entre_portais::Transform::setRotationZYX(glm::vec3 rotation) {
    rotation_ = glm::quat(rotation);
    handleTransformChange();
}

glm::vec4 entre_portais::Transform::getPosition() const {
    return glm::vec4(position_, 1.0f);
}


glm::vec3 entre_portais::Transform::getScale() const {
    return scale_;
}

void entre_portais::Transform::setRotation(glm::quat rotation) {
    rotation_ = normalize(rotation);
    handleTransformChange();
}

void entre_portais::Transform::setRotationZYX(float x, float y, float z) {
    rotation_ = glm::quat(glm::vec3(x, y, z));
    handleTransformChange();
}

glm::quat entre_portais::Transform::getRotation() const {
    return rotation_;
}

glm::vec3 entre_portais::Transform::getForward() const {
    return matrices::RotationFromQuat(rotation_) * glm::vec4(1, 0, 0, 0);
}

//glm::vec3 entre_portais::Transform::getForward() const {
//    return rotation_ * glm::vec3(0, 0, -1);
//}

glm::vec3 entre_portais::Transform::getRotationEulerZYX() const {
    return glm::eulerAngles(rotation_);
}


void entre_portais::Transform::renderImGui() {
    if (ImGui::TreeNode("Transform")) {
        glm::vec3 position = getPosition();
        glm::quat rotation = getRotation();
        glm::vec3 scale = getScale();

        bool pos_change = ImGui::DragFloat3("Position", glm::value_ptr(position), 0.02f);
        // Allow user to chose between euler and quaternion
        int rotation_type = ImGui::GetStateStorage()->GetInt(ImGui::GetID("##RotationType"), 0);
        if (ImGui::RadioButton("Euler Radians", &rotation_type, 0)) {
            ImGui::GetStateStorage()->SetInt(ImGui::GetID("##RotationType"), 0);
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Quaternion", &rotation_type, 1)) {
            ImGui::GetStateStorage()->SetInt(ImGui::GetID("##RotationType"), 1);
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Euler Degrees", &rotation_type, 2)) {
            ImGui::GetStateStorage()->SetInt(ImGui::GetID("##RotationType"), 2);
        }

        switch (rotation_type) {
            case 0: {
                glm::vec3 in_euler = glm::eulerAngles(rotation);
                bool euler_changed = ImGui::DragFloat3("Rotation", glm::value_ptr(in_euler), 0.02f);
                if (euler_changed) {
                    setRotationZYX(in_euler);
                }
                break;
            }
            case 1: {
                bool quat_changed = ImGui::DragFloat4("Rotation", glm::value_ptr(rotation), 0.02f);
                if (quat_changed) {
                    setRotation(rotation);
                }
                break;
            }
            case 2: { // DEGREES
                glm::vec3 in_euler = glm::eulerAngles(rotation);
                in_euler = glm::degrees(in_euler);
                bool euler_changed = ImGui::DragFloat3("Rotation", glm::value_ptr(in_euler), .2f);
                if (euler_changed) {
                    in_euler = glm::radians(in_euler);
                    setRotationZYX(in_euler);
                }
                break;
            }
        }


        bool scale_changed = ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.02f);

        // HANDLE CHANGES

        if (pos_change) {
            setPosition(position);
        }
        if (scale_changed) {
            setScale(scale);
        }

        ImGui::TreePop();
    }
}

void entre_portais::Transform::rotateBy(glm::quat rotation) {
    rotation_ = rotation * rotation_;
    handleTransformChange();
}

void entre_portais::Transform::rotateBy(glm::vec3 rotation) {
    rotateBy(glm::quat(rotation));
    handleTransformChange();
}

void entre_portais::Transform::rotateBy(float x, float y, float z) {
    rotateBy(glm::quat(glm::vec3(x, y, z)));
    handleTransformChange();
}

glm::vec3 entre_portais::Transform::rotateVector(glm::vec3 vector) const {
    return matrices::RotationFromQuat(rotation_) * glm::vec4(vector, 0);
}

void entre_portais::Transform::move(glm::vec3 vector) {
    setPosition(position_ + vector);
}

void entre_portais::Transform::handleTransformChange() const {
    for (auto callback: on_transform_change_callbacks_) {
        callback();
    }
}

void entre_portais::Transform::onTransformChange(std::function<void()> callback) {
    on_transform_change_callbacks_.push_back(callback);
    // Não chama o callback agora, apenas adiciona ele na lista de callbacks
    // Essa não é uma das situacoes em que o callback é chamado
    // Ele só é chamado quando o transform é alterado
}
