#include "entrePortaisEngine/physics/RigidBody.hpp"
#include "imgui.h"
#include "spdlog/spdlog.h"
#include "glm/gtx/string_cast.hpp"

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

    bool RigidBody::onCollision(RigidBody *const other) {
        return true;
    }

    void RigidBody::resolveCollision(RigidBody *const other, const collisions::PossibleCollision &possibleCollision,
                                     glm::vec3 velDiff) {
        // Outro objeto é dinâmico
        // http://www.chrishecker.com/images/e/e7/Gdmphys3.pdf, pag 4
        glm::vec3 diff = velDiff;
        float e = glm::min(other->getRestitution(), getRestitution());
        float j = -(1 + e) * glm::dot(diff, possibleCollision.normal) /
                  (1 / mass_ + 1 / other->mass_);

        glm::vec3 impulse = j * possibleCollision.normal;
//        applyForce(impulse);
        velocity_ += j * possibleCollision.normal / mass_;
        // TODO: Friction
        // TODO: Do rotation

    }

    void RigidBody::resolveCollisionWithStatic(RigidBody *const other,
                                               const collisions::PossibleCollision &possibleCollision) {
        // Outro objeto é estático
        // http://www.chrishecker.com/images/e/e7/Gdmphys3.pdf, pag 4

        glm::vec3 diff = velocity_;
        float e = 1.0f; // Estático não tem restituição, para evitar que o objeto fique "rebobinando"
        float j = -(1 + e) * glm::dot(diff, possibleCollision.normal) /
                  (1 / mass_); // Outro objeto é estático, logo a massa dele é infinita
        glm::vec3 impulse = j * possibleCollision.normal;
        spdlog::info("Impulse: {} J: {} Normal: {}", glm::to_string(impulse), j,
                     glm::to_string(possibleCollision.normal));
//        applyForce(impulse);
        velocity_ += j * possibleCollision.normal / mass_;
    }

    float RigidBody::getMass() const {
        return mass_;
    }

    float RigidBody::getRestitution() const {
        return restitution_;
    }

    float RigidBody::getFriction() const {
        return friction_;
    }

    const glm::vec3 &RigidBody::getVelocity() const {
        return velocity_;
    }
} // entre_portais