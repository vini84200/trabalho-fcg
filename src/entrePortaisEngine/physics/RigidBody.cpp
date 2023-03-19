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

    RigidBody::RigidBody( glm::mat4 *transform, std::unique_ptr<ICollider> collisor,
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
        if (isStatic_)
            return;
        updateGravity(deltaTime);
        updateVelocity(deltaTime);
    }

    void RigidBody::updateGravity(float time) {
        applyForceOnCM(gravity_ * mass_);

    }

    void RigidBody::renderImGui(Camera *camera) {
        ImGui::Text("RigidBody");
        ImGui::Text("ID: %d", id_);
        ImGui::Checkbox("Static", &isStatic_);
        ImGui::Text("Mass: %f", mass_);
        ImGui::Text("Velocity: %f, %f, %f", velocity_.x, velocity_.y, velocity_.z);
        if (ImGui::Button("Reset Velocity"))
            velocity_ = glm::vec3(0.0f);
        ImGui::Text("Angular Velocity: %f, %f, %f", angularVelocity_.x, angularVelocity_.y, angularVelocity_.z);
        if (ImGui::Button("Reset Angular Velocity"))
            angularVelocity_ = glm::vec3(0.0f);
        ImGui::Text("Last Force: %f, %f, %f", lastForce_.x, lastForce_.y, lastForce_.z);
        ImGui::SliderFloat("Mass", &mass_, 0.0f, 100.0f);
        ImGui::SliderFloat("Gravity", &gravity_.y, -20.0f, 20.0f);
        if (ImGui::TreeNode("Collider")) {
            collider_->renderImGui(camera);
            ImGui::TreePop();
        }
        // Apply force
        if (ImGui::TreeNode("Apply Force")) {
            float x = ImGui::GetStateStorage()->GetFloat(ImGui::GetID("ForceX"), 0.0f);
            float y = ImGui::GetStateStorage()->GetFloat(ImGui::GetID("ForceY"), 0.0f);
            float z = ImGui::GetStateStorage()->GetFloat(ImGui::GetID("ForceZ"), 0.0f);
            ImGui::SliderFloat("Force", &x, -100.0f, 100.0f);
            ImGui::SliderFloat("Force", &y, -100.0f, 100.0f);
            ImGui::SliderFloat("Force", &z, -100.0f, 100.0f);

            if (ImGui::Button("Apply on CM")) {
                applyForceOnCM(glm::vec3(x, y, z));
            }
            float px = ImGui::GetStateStorage()->GetFloat(ImGui::GetID("ForcePX"), 0.0f);
            float py = ImGui::GetStateStorage()->GetFloat(ImGui::GetID("ForcePY"), 0.0f);
            float pz = ImGui::GetStateStorage()->GetFloat(ImGui::GetID("ForcePZ"), 0.0f);

            ImGui::SliderFloat("Point X", &px, -10.0f, 10.0f);
            ImGui::SliderFloat("Point Y", &py, -10.0f, 10.0f);
            ImGui::SliderFloat("Point Z", &pz, -10.0f, 10.0f);
            if (ImGui::Button("Apply on Point")) {
                applyForceOnLocalPoint(glm::vec3(x, y, z), glm::vec3(px, py, pz));
            }
            ImGui::GetStateStorage()->SetFloat(ImGui::GetID("ForceX"), x);
            ImGui::GetStateStorage()->SetFloat(ImGui::GetID("ForceY"), y);
            ImGui::GetStateStorage()->SetFloat(ImGui::GetID("ForceZ"), z);

            ImGui::GetStateStorage()->SetFloat(ImGui::GetID("ForcePX"), px);
            ImGui::GetStateStorage()->SetFloat(ImGui::GetID("ForcePY"), py);
            ImGui::GetStateStorage()->SetFloat(ImGui::GetID("ForcePZ"), pz);
            ImGui::TreePop();
        }

    }


    glm::quat angularVelocityToSpin(glm::vec3 angularVelocity, glm::quat orientation) {
        glm::quat spin = glm::quat(0.0f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
        return 0.5f * spin * orientation;
    }

    void RigidBody::updateVelocity(float deltaTime) {
        velocity_ += force_ / mass_ * deltaTime;
        lastForce_ = force_;
        force_ = glm::vec3(0.0f);
        // Angular velocity
//        angularMomentum_ += torque_ * deltaTime;
        angularVelocity_ = getWorldInverseInertia() * (torque_ * deltaTime);
        torque_ = glm::vec3(0.0f);

        // glm::quat spin = angu

    }

    void RigidBody::updatePosition(float deltaTime) {
        if (isStatic_)
            return;

        transformToModify_.move(velocity_ * deltaTime);

        glm::quat orientation = transformToModify_.getRotation();
        glm::quat spin = angularVelocityToSpin(angularVelocity_, orientation);
        glm::quat newOrientation = orientation + spin * deltaTime;
        newOrientation = glm::normalize(newOrientation);
        transformToModify_.setRotation(newOrientation);
        // Reseta Accumaldores de Colisão
    }

    void RigidBody::applyForceOnCM(glm::vec3 force) {
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

    float RigidBody::resolveCollision(RigidBody *const other, collisions::PossibleCollision &possibleCollision,
                                      float dt) {
        // https://box2d.org/files/ErinCatto_SequentialImpulses_GDC2006.pdf
        float maxP = 0;

        assert(possibleCollision.contacts.size() > 0);
        assert(this->isStatic_ == false);
        assert(other->isStatic_ == false);

        int i = 0;
        for (auto &contact: possibleCollision.contacts) {
            assert(contact.normal != glm::vec3(0.0f));
            assert(glm::epsilonEqual(glm::length(contact.normal), 1.0f, 0.001f));

            if (contact.normal == glm::vec3(0.0f))
                continue;

            glm::vec3 r1Local = contact.pointA;
            glm::vec3 r2InOther = contact.pointB;

            glm::vec3 r1 = r1Local;
            glm::vec3 r2 = r2InOther;
            const glm::mat3 worldInverseInertiaA = getWorldInverseInertia();

            glm::vec3 veldiff = other->getVelocityAtPoint(r2) - getVelocityAtPoint(r1);


            float kn = (1 / getMass()) + (1 / other->getMass()) /*+
                       glm::dot(
                               glm::cross(worldInverseInertiaA * glm::cross(r1, contact.normal), r1) +
                               glm::cross(worldInverseInertiaB * glm::cross(r2, contact.normal), r2),
                               contact.normal)*/;
            if (kn == 0.0f) {
                spdlog::warn("kn is 0.0f");
                continue;
            }
            if (kn < 0.0f) {
                spdlog::warn("kn is negative");
                continue;
            }


            float beta = 0.2f;
            float bias = beta / dt;
            float slop = 0.1f;

            float v_bias = bias * glm::max(0.0f, contact.depth - slop);

            float restitution = std::max(getRestitution(), other->getRestitution());

            float v = glm::dot(-(1 + restitution) * veldiff, contact.normal);
            float pv = (v + v_bias) / kn;

            auto oldImpulse = contact.AccumulatedImpulse;
            contact.AccumulatedImpulse += pv;
            contact.AccumulatedImpulse = glm::max(contact.AccumulatedImpulse, 0.f);
            float Pn = contact.AccumulatedImpulse - oldImpulse;

            glm::vec3 t = glm::normalize(glm::cross(contact.normal, glm::cross(veldiff, contact.normal)));

            if (glm::length(t) < 0.01f || glm::isnan(glm::length(t)))
                t = glm::cross(contact.normal, glm::vec3(1.0f, 0.0f, 0.0f));


            float vt = glm::dot(-veldiff, t);

            float kt = (1 / getMass()) + (1 / other->getMass())/*+
                       glm::dot(glm::cross(getWorldInverseInertia() * glm::cross(r1, t), r1), t)*/;


            auto mi = 0.32f; // TODO: Calculate this based on the materials
            float Pt = vt / kt;

            oldImpulse = contact.AccumulatedFrictionImpulse;
            contact.AccumulatedFrictionImpulse = glm::clamp(contact.AccumulatedFrictionImpulse + Pt,
                                                            -mi * contact.AccumulatedImpulse,
                                                            mi * contact.AccumulatedImpulse);
            Pt = contact.AccumulatedFrictionImpulse - oldImpulse;

            glm::vec3 P = Pn * contact.normal + Pt * t;

            if (isnan(P.x) || isnan(P.y) || isnan(P.z)) {
                spdlog::warn("P is nan");
                continue;
            }

            glm::vec3 dvelA = P / getMass();
            glm::vec3 dvelB = P / other->getMass();
//            glm::vec3 deltaAngVelA = worldInverseInertiaA * glm::cross(r1, P);
            velocity_ -= dvelA;
            other->velocity_ += dvelB;
//            angularVelocity_ -= deltaAngVelA;
//            glm::vec3 newveldiff = - velocity_ - glm::cross(angularVelocity_, r1);
//            if (!glm::epsilonEqual(glm::dot(newveldiff, contact.normal), 0.0f, 0.001f)) {
//                spdlog::warn("veldiff is not 0.0f, {} -> {}, delta: {}", glm::dot(veldiff, contact.normal), glm::dot(newveldiff, contact.normal), glm::dot(veldiff, contact.normal) - glm::dot(newveldiff, contact.normal));
//                spdlog::info("P: {}", Pn);
//                spdlog::info("oldImpulse {}", oldImpulse);
//                spdlog::info("vbias: {}", v_bias);
//                spdlog::info("AccP: {}", contact.AccumulatedImpulse);
//                spdlog::info("r1: {}", glm::to_string(r1));
//                spdlog::info("r2: {}", glm::to_string(r2));
//                spdlog::info("contact.normal {}", glm::to_string(contact.normal));
//                spdlog::info("velocity_: {}", glm::to_string(velocity_));
//                spdlog::info("angularVelocity_: {}", glm::to_string(angularVelocity_));
//                spdlog::info("deltaAngVel: {}", glm::to_string(deltaAngVelA));
//                spdlog::info("r1 x P = {}", glm::to_string(glm::cross(r1, P)));
//                spdlog::info("deltaVel: {}", glm::to_string(dvelA));
//                spdlog::info("Original vel {}", glm::to_string(velA));
//                spdlog::info("Original velAng {}", glm::to_string(velAngA));
//                spdlog::info("Tangential vel A: {}", glm::to_string(velocity_ + glm::cross(angularVelocity_, r1)));
//                spdlog::info("inverseInertiaTensor_: {}", glm::to_string(inverseInertiaTensor_));
//                spdlog::info("InertiaTensor_: {}", glm::to_string(inertiaTensor_));
//                spdlog::info("i: {}", i);
//                spdlog::info("contact depth: {}", contact.depth);
//                //spdlog::info("in")
//
//                // assert(false);
//                // continue;
//            } else {
//              spdlog::info("OK");
//            }
            maxP = glm::max(maxP, glm::length(P));
            i++;
        }
//        spdlog::info("LAST i: {}", i);

        return maxP;

    }

    float RigidBody::resolveCollisionWithStatic(RigidBody *const other,
                                                collisions::PossibleCollision &possibleCollision,
                                                float dt) {
        // https://box2d.org/files/ErinCatto_SequentialImpulses_GDC2006.pdf

//        glm::mat4 &modelToWorld = *transform_;
//        glm::mat4 &otherModelToWorld = *other->transform_;
//        glm::mat4 worldToModel = glm::inverse(modelToWorld);
//        glm::mat4 otherModelToWorldInv = glm::inverse(otherModelToWorld);
//        glm::vec4 velA = modelToWorld * glm::vec4(velAModel, 0.0f);
//        glm::vec4 velB = otherModelToWorld * glm::vec4(velBModel, 0.0f);
//        glm::vec4 velAngA4 = modelToWorld * glm::vec4(velAngA, 0.0f);
//        glm::vec4 velAngB4 = otherModelToWorld * glm::vec4(velAngB, 0.0f);
        float maxP = 0;

        assert(possibleCollision.contacts.size() > 0);
        assert(this->isStatic_ == false);
        assert(other->isStatic_ == true);
        
        int i = 0;
        for (auto &contact: possibleCollision.contacts) {
            assert(contact.normal != glm::vec3(0.0f));
            assert(glm::epsilonEqual(glm::length(contact.normal), 1.0f, 0.001f));

            if (contact.normal == glm::vec3(0.0f))
              continue;

            glm::vec3 r1Local = contact.pointA;
            glm::vec3 r2InOther = contact.pointB;

            glm::vec3 r1 = r1Local;
            const glm::mat3 worldInverseInertiaA = getWorldInverseInertia();

            glm::vec3 veldiff = -getVelocityAtPoint(r1);


            float kn = (1 / getMass()) /*+
                       glm::dot(
                               glm::cross(worldInverseInertiaA * glm::cross(r1, contact.normal), r1),
                               contact.normal)*/;
            if (kn == 0.0f) {
                spdlog::warn("kn is 0.0f");
                continue;
            }
            if (kn < 0.0f) {
                spdlog::warn("kn is negative");
                continue;
            }


            float beta = 0.2f;
            float bias = beta / dt;
            float slop = 0.1f;

            float v_bias = bias * glm::max(0.0f, contact.depth - slop);

            float restitution = std::max(getRestitution(), other->getRestitution());

            float v = glm::dot(-(1 + restitution) * veldiff, contact.normal);
            float pv = (v + v_bias) / kn;

            auto oldImpulse = contact.AccumulatedImpulse;
            contact.AccumulatedImpulse += pv;
            contact.AccumulatedImpulse = glm::max(contact.AccumulatedImpulse, 0.f);
            float Pn = contact.AccumulatedImpulse - oldImpulse;

            glm::vec3 t = glm::normalize(glm::cross(contact.normal, glm::cross(veldiff, contact.normal)));

            if (glm::length(t) < 0.01f || glm::isnan(glm::length(t)))
                t = glm::cross(contact.normal, glm::vec3(1.0f, 0.0f, 0.0f));


            float vt = glm::dot(-veldiff, t);

            float kt = (1 / getMass()) /*+
                       glm::dot(glm::cross(getWorldInverseInertia() * glm::cross(r1, t), r1), t)*/;


            auto mi = 0.32f; // TODO: Calculate this based on the materials
            float Pt = vt / kt;

            oldImpulse = contact.AccumulatedFrictionImpulse;
            contact.AccumulatedFrictionImpulse = glm::clamp(contact.AccumulatedFrictionImpulse + Pt,
                                                            -mi * contact.AccumulatedImpulse,
                                                            mi * contact.AccumulatedImpulse);
            Pt = contact.AccumulatedFrictionImpulse - oldImpulse;

            glm::vec3 P = Pn * contact.normal + Pt * t;

            if (isnan(P.x) || isnan(P.y) || isnan(P.z)) {
                spdlog::warn("P is nan");
                continue;
            }

            glm::vec3 dvelA = P / getMass();
//            glm::vec3 deltaAngVelA = worldInverseInertiaA * glm::cross(r1, P);
            velocity_ -= dvelA;
//            angularVelocity_ -= deltaAngVelA;
//            glm::vec3 newveldiff = - velocity_ - glm::cross(angularVelocity_, r1);
//            if (!glm::epsilonEqual(glm::dot(newveldiff, contact.normal), 0.0f, 0.001f)) {
//                spdlog::warn("veldiff is not 0.0f, {} -> {}, delta: {}", glm::dot(veldiff, contact.normal), glm::dot(newveldiff, contact.normal), glm::dot(veldiff, contact.normal) - glm::dot(newveldiff, contact.normal));
//                spdlog::info("P: {}", Pn);
//                spdlog::info("oldImpulse {}", oldImpulse);
//                spdlog::info("vbias: {}", v_bias);
//                spdlog::info("AccP: {}", contact.AccumulatedImpulse);
//                spdlog::info("r1: {}", glm::to_string(r1));
//                spdlog::info("r2: {}", glm::to_string(r2));
//                spdlog::info("contact.normal {}", glm::to_string(contact.normal));
//                spdlog::info("velocity_: {}", glm::to_string(velocity_));
//                spdlog::info("angularVelocity_: {}", glm::to_string(angularVelocity_));
//                spdlog::info("deltaAngVel: {}", glm::to_string(deltaAngVelA));
//                spdlog::info("r1 x P = {}", glm::to_string(glm::cross(r1, P)));
//                spdlog::info("deltaVel: {}", glm::to_string(dvelA));
//                spdlog::info("Original vel {}", glm::to_string(velA));
//                spdlog::info("Original velAng {}", glm::to_string(velAngA));
//                spdlog::info("Tangential vel A: {}", glm::to_string(velocity_ + glm::cross(angularVelocity_, r1)));
//                spdlog::info("inverseInertiaTensor_: {}", glm::to_string(inverseInertiaTensor_));
//                spdlog::info("InertiaTensor_: {}", glm::to_string(inertiaTensor_));
//                spdlog::info("i: {}", i);
//                spdlog::info("contact depth: {}", contact.depth);
//                //spdlog::info("in")
//
//                // assert(false);
//                // continue;
//            } else {
//              spdlog::info("OK");
//            }
            maxP = glm::max(maxP, glm::length(P));
            i++;
        }
//        spdlog::info("LAST i: {}", i);

        return maxP;
    }

    glm::vec3 RigidBody::getVelocityAtPoint(const glm::vec3 &r1) const {
        return velocity_ + glm::cross(angularVelocity_, r1);
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

    glm::vec3 RigidBody::getVelocityAt(const glm::vec3 r) const {
        return velocity_ + glm::cross(angularVelocity_, r);
    }

    glm::mat4 *RigidBody::getTransform() const {
        return transform_;
    }

    float RigidBody::getDampingFactor() {
        return 0.96f;
    }

    glm::vec3 RigidBody::getAngularVelocity() const {
        return angularVelocity_;
    }

    const glm::vec3 &RigidBody::getForce() const {
        return force_;
    }

    void RigidBody::setIsStatic(bool isStatic) {
        isStatic_ = isStatic;
    }

    const glm::mat3 &RigidBody::getInertiaTensor() const {
        return inertiaTensor_;
    }

    void RigidBody::setInertiaTensor(const glm::mat3 &inertiaTensor) {
        inertiaTensor_ = inertiaTensor;
        inverseInertiaTensor_ = glm::inverse(inertiaTensor);
    }

    glm::mat3 RigidBody::getWorldInverseInertia() {
        glm::mat3 const worldRotation = glm::mat3(getWorldRotation());
        return glm::transpose(worldRotation) * inverseInertiaTensor_ * worldRotation;
    }

    glm::vec3 RigidBody::getWorldPosition() {
        return glm::vec3((*transform_)[3]);
    }

    glm::quat RigidBody::getWorldRotation() {
        return matrices::RotationFromMatrix(*transform_);
    }

    void RigidBody::applyForceOnLocalPoint(glm::vec3 force, glm::vec3 point) {
        force_ += force;
        torque_ += glm::cross(point, force);
    }
} // entre_portais
