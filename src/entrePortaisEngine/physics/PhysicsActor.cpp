#include "entrePortaisEngine/physics/PhysicsActor.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"

namespace entre_portais {

    physx::PxVec3 toPxVec3(const glm::vec3 &vec) {
        return {vec.x, vec.y, vec.z};
    }

    physx::PxVec3 toPxVec3(const glm::vec4 &vec) {
        return {vec.x, vec.y, vec.z};
    }

    physx::PxQuat toPxQuat(const glm::quat &quat) {
        return {quat.x, quat.y, quat.z, quat.w};
    }

    physx::PxTransform toPxTransform(const entre_portais::Transform &transform) {
        return {
                toPxVec3(transform.getPosition()),
                toPxQuat(transform.getRotation())
        };

    }
    PhysicsScene &PhysicsActor::getPhysicsEngine() const {
//        actor_->userData
        // TODO: insert return statement here
    }

    physx::PxTransform toPxTransform(const Transform &transform);

    PhysicsActor::PhysicsActor(PhysicsScene &physicsScene, Transform &transform, const physx::PxGeometry &geometry,
                               const physx::PxMaterial &material, const physx::PxReal density, bool isDynamic) : localTransform_(transform) {
        shape_ = physicsScene.getPhysics().createShape(geometry, material, true);
        physx::PxTransform const pxTransform = entre_portais::toPxTransform(transform);
        if (isDynamic) {
            physx::PxRigidDynamic *rigidBody = physicsScene.getPhysics().createRigidDynamic(pxTransform);
            physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, density);
            body = rigidBody;
        }
        else {
            body = entre_portais::PhysicsScene::getPhysics().createRigidStatic(pxTransform);
        }
        body->attachShape(*shape_);
        body->userData = this;
        transform.onTransformChange([this]() { onChange(); });
        physicsScene.getScene().addActor(*body);
    }

    void PhysicsActor::postPhysics(float deltaTime) {
        // Update the transform of the rigid body
        const physx::PxTransform &transform = body->getGlobalPose();
        localTransform_.setPosition(glm::vec3(transform.p.x, transform.p.y, transform.p.z));
        localTransform_.setRotation(glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z));
    }

    void PhysicsActor::onChange() {

        // Update the actor transform in the physics engine to match the local transform
        const physx::PxTransform &transform = entre_portais::toPxTransform(localTransform_);
        body->setGlobalPose(transform);

    }

    void PhysicsActor::renderImGui(Camera *camera) {

    }

    void PhysicsActor::update(float deltaTime) {

    }

    void PhysicsActor::setTransform(Transform &transform) {

    }

    void PhysicsActor::setPhysicsEngine(PhysicsScene &physicsEngine) {

    }

} // entre_portais
