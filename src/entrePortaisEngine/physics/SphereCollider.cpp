#include "entrePortaisEngine/physics/SphereCollider.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"
#include "utils/matrices.h"
#include "imgui.h"

namespace entre_portais {
    SphereCollider::SphereCollider(glm::mat4 &modelMatrix) : ICollider(modelMatrix), radius_(1) {}

    collisions::PossibleCollision SphereCollider::isColliding(ICollider *other) {
        return other->isColliding(*this);
    }

    collisions::PossibleCollision SphereCollider::isColliding(glm::vec4 point) {
        return collisions::checkCollisionSpherePoint(modelMatrix_, point);
    }

    collisions::PossibleCollision SphereCollider::isColliding(BoxCollider box) {
        return collisions::checkCollisionBoxSphere(box.getTransform(), modelMatrix_);
    }

    collisions::PossibleCollision SphereCollider::isColliding(SphereCollider sphere) {
        return collisions::checkCollisionSphereSphere(modelMatrix_, radius_, sphere.modelMatrix_, sphere.radius_);
    }

    void SphereCollider::Draw() {

    }

    void SphereCollider::renderImGui(Camera *camera) {
        ImGui::Text("SphereCollider");
        ImGui::InputFloat("Radius", &radius_);
    }

    BoundingBox SphereCollider::getBoundingBox(glm::mat4 modelMatrix) {
        auto scalingMatrix = matrices::ScalingMatrixFromMatrix(modelMatrix);
        auto size = scalingMatrix * glm::vec4(radius_);

        return BoundingBox(
                matrices::PositionFromMatrix(modelMatrix) - size, // min
                matrices::PositionFromMatrix(modelMatrix) + size  // max
        );
    }

    BoundingBox SphereCollider::getBoundingBox() {
        auto scalingMatrix = matrices::ScalingMatrixFromMatrix(modelMatrix_);
        auto size = scalingMatrix * glm::vec4(radius_);

        return BoundingBox(
                matrices::PositionFromMatrix(modelMatrix_) - size,
                matrices::PositionFromMatrix(modelMatrix_) + size
        );
    }

    SphereCollider::SphereCollider(float radius, glm::mat4 &modelMatrix)
            : ICollider(modelMatrix) {
        radius_ = radius;
    }
} // entre_portais