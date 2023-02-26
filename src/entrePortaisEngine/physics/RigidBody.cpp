#include "entrePortaisEngine/physics/RigidBody.hpp"
#include "imgui.h"

namespace entre_portais {
    BoundingBox RigidBody::getBoundingBox() {
        return collider_->getBoundingBox(*transform_);
    }

    PhysicsEngine &RigidBody::getPhysicsEngine() const {
        return physicsEngine_;
    }

    void RigidBody::setPhysicsEngine(PhysicsEngine &physicsEngine) {
        physicsEngine_ = physicsEngine;
    }

    void RigidBody::setId(int id) {
        id_ = id;
    }

    int RigidBody::getID() const {
        return id_;
    }

    RigidBody::RigidBody(
            glm::mat4 *transform,
            std::unique_ptr<ICollider> collisor,
            PhysicsEngine &pysEngine,
            Transform &transformToModify
    )
            : transform_(transform),
              physicsEngine_(pysEngine),
              transformToModify_(transformToModify) {
        collider_ = std::move(collisor);
        pysEngine.addRigidBody(this);
    }

    void RigidBody::onChange() {
        if (id_ != -1)
            physicsEngine_.updateRigidBody(this);
    }

    void RigidBody::updateDynamics(float deltaTime) {
        updateGravity(deltaTime);
        updateVelocity(deltaTime);
        updatePosition(deltaTime);

    }

    void RigidBody::updateGravity(float time) {
        applyForce(gravity_ * mass_);

    }

    void RigidBody::renderImGui() {
        ImGui::Text("RigidBody");
        ImGui::Text("ID: %d", id_);
        ImGui::Checkbox("Static", &isStatic_);
        ImGui::Text("Mass: %f", mass_);
        ImGui::Text("Velocity: %f, %f, %f", velocity_.x, velocity_.y, velocity_.z);
        ImGui::Text("Last Force: %f, %f, %f", lastForce_.x, lastForce_.y, lastForce_.z);
        ImGui::SliderFloat("Mass", &mass_, 0.0f, 100.0f);
        ImGui::SliderFloat("Gravity", &gravity_.y, -20.0f, 20.0f);

    }

    void RigidBody::updateVelocity(float deltaTime) {
        velocity_ += force_ / mass_ * deltaTime;
        lastForce_ = force_;
        force_ = glm::vec3(0.0f);
    }

    void RigidBody::updatePosition(float deltaTime) {
        transformToModify_.move(velocity_ * deltaTime);
    }

    void RigidBody::applyForce(glm::vec3 force) {
        force_ += force;
    }

    void RigidBody::onTransformChange() {

    }

    void RigidBody::update(float deltaTime) {
        if (!isStatic_) {
            updateDynamics(deltaTime);
        }
    }

    const std::unique_ptr<ICollider> &RigidBody::getCollider() const {
        return collider_;
    }

    bool RigidBody::isStatic() const {
        return isStatic_;
    }
} // entre_portais