#include "entrePortaisEngine/Transform.hpp"

#include "utils/matrices.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/quaternion.hpp"

glm::mat4 entre_portais::Transform::getModelMatrix() {
    glm::mat4 model = matrices::Matrix_Identity();
    model = matrices::Matrix_Scale(scale_.x, scale_.y, scale_.z) * model;
    model = matrices::RotationFromQuat(rotation_) * model;
    model = matrices::Matrix_Translate(position_.x, position_.y, position_.z) * model;
    return model;
}

void entre_portais::Transform::setPosition(glm::vec3 position) {
    position_ = position;
}

entre_portais::Transform::Transform() {
    position_ = glm::vec3(0, 0, 0);
    rotation_ = glm::quat(glm::vec3(0, 0, 0));
    scale_ = glm::vec3(1, 1, 1);
}

void entre_portais::Transform::setScale(glm::vec3 scale) {
    scale_ = scale;
}

void entre_portais::Transform::setRotation(glm::vec3 rotation) {
    rotation_ = glm::quat(rotation);
}

float *entre_portais::Transform::getPositionPtr() {
    return glm::value_ptr(position_);
}

float *entre_portais::Transform::getRotationPtr() {
    return glm::value_ptr(rotation_);
}

float *entre_portais::Transform::getScalePtr() {
    return glm::value_ptr(scale_);
}

glm::vec4 entre_portais::Transform::getPosition() {
    return glm::vec4(position_, 1.0f);
}


glm::vec3 entre_portais::Transform::getScale() {
    return scale_;
}

void entre_portais::Transform::setRotation(glm::quat rotation) {
    rotation_ = normalize(rotation);
}

void entre_portais::Transform::setRotation(float x, float y, float z) {
    rotation_ = glm::quat(glm::vec3(x, y, z));
}

glm::quat entre_portais::Transform::getRotation() {
    return rotation_;
}

glm::vec3 entre_portais::Transform::getForward() {
//    return glm::conjugate(rotation_) * glm::vec3(0, 0, 1) * rotation_;
//    glm::vec3 vecZ(0.0, 0.0, 1.0);
//    glm::vec3 rotationXYZ (rotation_.x, rotation_.y, rotation_.z);
//    return vecZ + 2.0f * cross(rotationXYZ, cross(rotationXYZ, vecZ) + rotation_.w * vecZ);
    return matrices::RotationFromQuat(rotation_) * glm::vec4(0, 0, 1, 0);
}

glm::vec3 entre_portais::Transform::getRotationEuler() {
    return glm::eulerAngles(rotation_);
}


void entre_portais::Transform::renderImGui() {
    if (ImGui::TreeNode("Transform")) {
        glm::vec3 rotation = getRotationEuler();
        ImGui::DragFloat3("Position", getPositionPtr(), 0.02f);
        bool changed = (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.02f));
        ImGui::Checkbox("Lock Rotation", &lock_rotation_);
        if (changed) {
            if (lock_rotation_)
                rotateBy(rotation - getRotationEuler());
            else
                setRotation(rotation);
        }

        ImGui::DragFloat3("Scale", getScalePtr(), 0.02f);

        ImGui::TreePop();
    }
}

void entre_portais::Transform::rotateBy(glm::quat rotation) {
    rotation_ = rotation * rotation_;
}

void entre_portais::Transform::rotateBy(glm::vec3 rotation) {
    rotateBy(glm::quat(rotation));
}

void entre_portais::Transform::rotateBy(float x, float y, float z) {
    rotateBy(glm::quat(glm::vec3(x, y, z)));
}

glm::vec3 entre_portais::Transform::rotateVector(glm::vec3 vector) {
    return matrices::RotationFromQuat(rotation_) * glm::vec4(vector, 0);
}

void entre_portais::Transform::move(glm::vec3 vector) {
    position_ += vector;
}
