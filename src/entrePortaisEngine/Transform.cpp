#include "entrePortaisEngine/Transform.hpp"
#include "utils/matrices.h"

glm::mat4 entre_portais::Transform::getModelMatrix() {
    glm::mat4 model = matrices::Matrix_Identity();
    model = matrices::Matrix_Scale(sx, sy, sz) * model;
    model = matrices::Matrix_Rotate_X(glm::radians(rx)) * model;
    model = matrices::Matrix_Rotate_Y(glm::radians(ry)) * model;
    model = matrices::Matrix_Rotate_Z(glm::radians(rz)) * model;
    model = matrices::Matrix_Translate(x, y, z) * model;
    return model;
}

void entre_portais::Transform::setPosition(glm::vec3 position) {
    x = position.x;
    y = position.y;
    z = position.z;
}

entre_portais::Transform::Transform() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    rx = 0.0f;
    ry = 0.0f;
    rz = 0.0f;
    sx = 1.0f;
    sy = 1.0f;
    sz = 1.0f;
}

void entre_portais::Transform::setScale(glm::vec3 scale) {
    sx = scale.x;
    sy = scale.y;
    sz = scale.z;
}

void entre_portais::Transform::setRotation(glm::vec3 rotation) {
    rx = rotation.x;
    ry = rotation.y;
    rz = rotation.z;
}

float *entre_portais::Transform::getPositionPtr() {
    return &x;
}

float *entre_portais::Transform::getRotationPtr() {
    return &rx;
}

float *entre_portais::Transform::getScalePtr() {
    return &sx;
}
