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
    }

    void RigidBody::updateVelocity(float deltaTime) {
        velocity_ += force_ / mass_ * deltaTime;
        lastForce_ = force_;
        force_ = glm::vec3(0.0f);

        // Angular velocity

        angularVelocity_ += glm::inverse(inertiaTensor_) * torque_ * deltaTime;
        angularVelocity_ = angularVelocity_ * getDampingFactor();
        torque_ = glm::vec3(0.0f);
    }

    void RigidBody::updatePosition(float deltaTime) {
        if (isStatic_)
            return;
        transformToModify_.move(velocity_ * deltaTime);

        auto rotation = transformToModify_.getRotation();
        glm::quat w = glm::quat(0.0f, angularVelocity_.x, angularVelocity_.y, angularVelocity_.z);
        rotation = rotation + deltaTime / 2 * w * rotation;
//        rotation = glm::normalize(rotation);
        transformToModify_.setRotation(rotation);

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

    float RigidBody::resolveCollision(RigidBody *const other, const collisions::PossibleCollision &possibleCollision,
                                      glm::vec3 velA, glm::vec3 velB, glm::vec3 velAngA, glm::vec3 velAngB, float dt) {
        // https://box2d.org/files/ErinCatto_SequentialImpulses_GDC2006.pdf
        return 0.0; // TODO: Fix this

        float Pn = 0;
        float AccPn = 0;

        float Ptu = 0;
        float AccPtu = 0;
        float Ptv = 0;
        float AccPtv = 0;
        spdlog::info("velA: {}, velB: {}, velAngA: {}, velAngB: {}", glm::to_string(velA), glm::to_string(velB),
                     glm::to_string(velAngA), glm::to_string(velAngB));
        spdlog::info("Contacts: {}", possibleCollision.contacts.size());

        assert(possibleCollision.contacts.size() > 0);
        assert(this->isStatic_ == false);
        assert(other->isStatic_ == false);

        for (const auto &contact: possibleCollision.contacts) {
            spdlog::info("Contact normal: {} {} {}", contact.normal.x, contact.normal.y, contact.normal.z);
            spdlog::info("Contact pointA: {} {} {}", contact.pointA.x, contact.pointA.y, contact.pointA.z);
            spdlog::info("Contact pointB: {} {} {}", contact.pointB.x, contact.pointB.y, contact.pointB.z);
            spdlog::info("Contact depth: {}", contact.depth);

            if (contact.normal == glm::vec3(0.0f))
                continue;

            glm::vec3 r1Local = contact.pointA - glm::vec3(0.0f);
            glm::vec3 r2InOther = contact.pointB - glm::vec3(0);

            glm::vec3 r1 = r1Local;
            glm::vec3 r2 = r2InOther;
            spdlog::info("r1 in local: {} {} {}", r1.x, r1.y, r1.z);
            spdlog::info("r2 in other: {} {} {}", r2.x, r2.y, r2.z);


            glm::vec3 veldiff = velB - velA + glm::cross(velAngB, r2) - glm::cross(velAngA, r1);

            float kn = (1 / getMass()) + (1 / other->getMass()) +
                       glm::dot(
                               glm::inverse(inertiaTensor_) * glm::cross(glm::cross(r1, contact.normal), r1)
                               + glm::inverse(other->inertiaTensor_) * glm::cross(glm::cross(r2, contact.normal), r2),
                               contact.normal);
            if (kn == 0.0f) {
                spdlog::warn("kn is 0.0f");
                continue;
            }


            float beta = 0.2f;
            float bias = beta / dt;
            float slop = 0.01f;
            float v_bias = bias * glm::max(0.0f, contact.depth - slop);


            spdlog::info("veldiff: {} {} {}", veldiff.x, veldiff.y, veldiff.z);
            spdlog::info("kn: {}", kn);
            spdlog::info("v_bias: {} slop: {} beta: {} dt: {}", v_bias, slop, beta, dt);

            spdlog::info("AccPn: {}", AccPn);

            auto temp = AccPn;
            float v = glm::dot(-veldiff, contact.normal);
            spdlog::info("v: {}", v);
            float pv = (v + v_bias) / kn;
            AccPn = glm::max(AccPn + pv, 0.0f);
            spdlog::info("AccPn: {}", AccPn);
            Pn = AccPn - temp;
            spdlog::info("Pn: {}", Pn);
            spdlog::info("AccPn: {}", AccPn);

            glm::vec3 tu = glm::cross(contact.normal, glm::vec3(0.0f, 1.0f, 0.0f));
            if (glm::length(tu) < 0.01f)
                tu = glm::cross(contact.normal, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::vec3 tv = glm::cross(tu, contact.normal);

            float vtu = glm::dot(-veldiff, tu);
            float vtv = glm::dot(-veldiff, tv);

            float ktu = (1 / getMass()) + (1 / other->getMass()) +
                        glm::dot(glm::inverse(inertiaTensor_) * glm::cross(glm::cross(r1, tu), r1)
                                 + glm::inverse(other->inertiaTensor_) * glm::cross(glm::cross(r2, tu), r2), tu);


            float ktv = (1 / getMass()) + (1 / other->getMass()) +
                        glm::dot(glm::inverse(inertiaTensor_) * glm::cross(glm::cross(r1, tv), r1)
                                 + glm::inverse(other->inertiaTensor_) * glm::cross(glm::cross(r2, tv), r2), tv);


            auto mi = 0.3f;
            auto Ptu = vtu / ktu;
            auto Ptv = vtv / ktv;

            temp = AccPtu;
            AccPtu = glm::clamp(AccPtu + Ptu, -mi * AccPn, mi * AccPn);
            Ptu = AccPtu - temp;

            temp = AccPtv;
            AccPtv = glm::clamp(AccPtv + Ptv, -mi * AccPn, mi * AccPn);
            Ptv = AccPtv - temp;

            spdlog::info("Ptu: {} Ptv: {}", Ptu, Ptv);

            glm::vec3 P = Pn * contact.normal + Ptu * tu + Ptv * tv;
            spdlog::info("P: {} {} {}", P.x, P.y, P.z);
            spdlog::info("Ptu: {} Ptv: {}", Ptu, Ptv);
            spdlog::info("tu: {} {} {}", tu.x, tu.y, tu.z);
            spdlog::info("tv: {} {} {}", tv.x, tv.y, tv.z);

            if (isnan(P.x) || isnan(P.y) || isnan(P.z)) {
                spdlog::warn("P is nan");
                continue;
            }
            velocity_ -= P / getMass();
            angularVelocity_ -= glm::inverse(inertiaTensor_) * glm::cross(r1, P);
            other->velocity_ += P / other->getMass();
            other->angularVelocity_ += glm::inverse(other->inertiaTensor_) * glm::cross(r2, P);
        }

        spdlog::info("----------------------");

    }

    float RigidBody::resolveCollisionWithStatic(RigidBody *const other,
                                                collisions::PossibleCollision &possibleCollision,
                                                float dt) {
        // https://box2d.org/files/ErinCatto_SequentialImpulses_GDC2006.pdf

        glm::vec3 velA = velocity_;
        glm::vec3 velB = glm::vec3(0.0f);
        glm::vec3 velAngA = angularVelocity_;
        glm::vec3 velAngB = glm::vec3(0.0f);
//        glm::mat4 &modelToWorld = *transform_;
//        glm::mat4 &otherModelToWorld = *other->transform_;
//        glm::mat4 worldToModel = glm::inverse(modelToWorld);
//        glm::mat4 otherModelToWorldInv = glm::inverse(otherModelToWorld);
//        glm::vec4 velA = modelToWorld * glm::vec4(velAModel, 0.0f);
//        glm::vec4 velB = otherModelToWorld * glm::vec4(velBModel, 0.0f);
//        glm::vec4 velAngA4 = modelToWorld * glm::vec4(velAngA, 0.0f);
//        glm::vec4 velAngB4 = otherModelToWorld * glm::vec4(velAngB, 0.0f);

        float Pn = 0;

        float Ptu = 0;
        float Ptv = 0;

        float maxP = 0;

        assert(possibleCollision.contacts.size() > 0);
        assert(this->isStatic_ == false);
        assert(other->isStatic_ == true);

        for (auto &contact: possibleCollision.contacts) {
            assert(contact.normal != glm::vec3(0.0f));
            assert(glm::epsilonEqual(glm::length(contact.normal), 1.0f, 0.001f));
            if (contact.normal == glm::vec3(0.0f))
                continue;

            glm::vec3 r1Local = contact.pointA - glm::vec3(0.0f);
            glm::vec3 r2InOther = contact.pointB - glm::vec3(0);

            glm::mat4 &modelToWorld = *transform_;
            glm::mat4 &otherModelToWorld = *other->transform_;
            glm::mat4 otherModelToThisModel = glm::inverse(modelToWorld) * otherModelToWorld;


            glm::vec4 r1World = modelToWorld * glm::vec4(r1Local, 0.0f);
            glm::vec4 r2World = otherModelToWorld * glm::vec4(r2InOther, 0.0f);



            glm::vec3 r1 = r1World;
            glm::vec3 r2 = r2World;



            glm::vec3 veldiff = velB + glm::cross(velAngB, r2) - velA - glm::cross(velAngA, r1);


            float kn = (1 / getMass()) +
                       glm::dot(
                               inverseInertiaTensor_ * glm::cross(glm::cross(r1, contact.normal), r1),
//                               + other->inverseInertiaTensor_ * glm::cross(glm::cross(r2, contact.normal), r2),
                               contact.normal);
            if (kn == 0.0f) {
                spdlog::warn("kn is 0.0f");
                continue;
            }
            if (kn < 0.0f) {
                spdlog::warn("kn is negative");
                continue;
            }
            if (!glm::epsilonEqual(kn, 1.0f, 0.001f)) {
                spdlog::warn("kn is not 1.0f, kn={}", kn);
                continue;
            }


            float beta = 0.4f;
            float bias = beta / dt;
            float slop = 0.01f;

            float v_bias = bias * glm::max(0.0f, contact.depth - slop);


            float v = glm::dot(-veldiff, contact.normal);
            float pv = (v + v_bias) / kn;

            auto oldImpulse = contact.AccumulatedImpulse;
            contact.AccumulatedImpulse += pv;
            contact.AccumulatedImpulse = glm::max(contact.AccumulatedImpulse, 0.f);
            Pn = contact.AccumulatedImpulse - oldImpulse;

//            glm::vec3 tu = glm::cross(contact.normal, glm::vec3(0.0f, 1.0f, 0.0f));
//            if (glm::length(tu) < 0.01f)
//                tu = glm::cross(contact.normal, glm::vec3(1.0f, 0.0f, 0.0f));
//            glm::vec3 tv = glm::cross(tu, contact.normal);
//
//            float vtu = glm::dot(- veldiff, tu);
//            float vtv = glm::dot(- veldiff, tv);
//
//            float ktu = (1/getMass()) +
//                       glm::dot(glm::inverse(inertiaTensor_) * glm::cross(glm::cross(r1, tu), r1)
//                                + glm::inverse(other->inertiaTensor_) * glm::cross(glm::cross(r2, tu), r2), tu);
//
//
//            float ktv = (1/getMass()) +
//                       glm::dot(glm::inverse(inertiaTensor_) * glm::cross(glm::cross(r1, tv), r1)
//                                + glm::inverse(other->inertiaTensor_) * glm::cross(glm::cross(r2, tv), r2), tv);
//
//
//            auto mi = 0.9f;
//            Ptu = vtu / ktu;
//            Ptv = vtv / ktv;
//
//            oldImpulse = contact.AccumulatedFrictionImpulseU;
//            contact.AccumulatedFrictionImpulseU = glm::clamp(contact.AccumulatedFrictionImpulseU + Ptu, -mi * Pn, mi * Pn);
//            Ptu = contact.AccumulatedFrictionImpulseU - oldImpulse;
//
//            oldImpulse = contact.AccumulatedFrictionImpulseV;
//            contact.AccumulatedFrictionImpulseV = glm::clamp(contact.AccumulatedFrictionImpulseV + Ptv, -mi * Pn, mi * Pn);
//            Ptv = contact.AccumulatedFrictionImpulseV - oldImpulse;
//
//
            glm::vec3 P = Pn * contact.normal;//+ Ptu * tu + Ptv * tv;

            if (isnan(P.x) || isnan(P.y) || isnan(P.z)) {
                spdlog::warn("P is nan");
                continue;
            }

            velocity_ -= P / getMass();
            angularVelocity_ -= inverseInertiaTensor_ * glm::cross(r1, P);
            spdlog::info("P: {}", Pn);
            spdlog::info("r1: {}", glm::to_string(r1));
            spdlog::info("velocity_: {}", glm::to_string(velocity_));
            spdlog::info("angularVelocity_: {}", glm::to_string(angularVelocity_));
            spdlog::info("deltaAngVel: {}", glm::to_string(glm::cross(r1, P)));
            spdlog::info("normal: {}", glm::to_string(contact.normal));
            maxP = glm::max(maxP, glm::length(P));
        }

        return maxP;
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
        return 0.99f;
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
} // entre_portais