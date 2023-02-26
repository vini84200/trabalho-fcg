#include "entrePortaisEngine/Transform.hpp"

#include "utils/matrices.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/quaternion.hpp"

glm::mat4 entre_portais::Transform::getLocalModelMatrix() const {
    glm::mat4 model = matrices::Matrix_Identity();
    model = matrices::Matrix_Scale(scale_.x, scale_.y, scale_.z) * model;
    model = matrices::RotationFromQuat(rotation_) * model;
    model = matrices::Matrix_Translate(position_.x, position_.y, position_.z) * model;
    return model;
}

void entre_portais::Transform::setPosition(glm::vec3 position) {
    position_ = position;
    handleTransformChange();
}

entre_portais::Transform::Transform() {
    position_ = glm::vec3(0, 0, 0);
    rotation_ = glm::quat(glm::vec3(0, 0, 0));
    scale_ = glm::vec3(1, 1, 1);
    handleTransformChange();
}

void entre_portais::Transform::setScale(glm::vec3 scale) {
    scale_ = scale;
    handleTransformChange();
}

void entre_portais::Transform::setRotation(glm::vec3 rotation) {
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

void entre_portais::Transform::setRotation(float x, float y, float z) {
    rotation_ = glm::quat(glm::vec3(x, y, z));
    handleTransformChange();
}

glm::quat entre_portais::Transform::getRotation() const {
    return rotation_;
}

glm::vec3 entre_portais::Transform::getForward() const {
    return matrices::RotationFromQuat(rotation_) * glm::vec4(0, 0, 1, 0);
}

glm::vec3 entre_portais::Transform::getRotationEuler() const {
    return glm::eulerAngles(rotation_);
}


void entre_portais::Transform::renderImGui() {
    if (ImGui::TreeNode("Transform")) {
        glm::vec3 position = getPosition();
        glm::vec3 rotation = getRotationEuler();
        glm::vec3 scale = getScale();

        bool pos_change = ImGui::DragFloat3("Position", glm::value_ptr(position), 0.02f);
        bool rotation_changed = (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.02f));
        ImGui::Checkbox("Lock Rotation", &lock_rotation_);
        bool scale_changed = ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.02f);

        // HANDLE CHANGES

        if (pos_change) {
            setPosition(position);
        }
        if (rotation_changed) {
            if (lock_rotation_)
                rotateBy(rotation - getRotationEuler());
            else
                setRotation(rotation);
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
