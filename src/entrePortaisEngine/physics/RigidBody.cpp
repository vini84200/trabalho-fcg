#include "entrePortaisEngine/physics/RigidBody.hpp"

namespace entre_portais {
    BoundingBox RigidBody::getBoundingBox() {
        return collisor_->getBoundingBox(*transform_);
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

    RigidBody::RigidBody(glm::mat4 *transform, std::unique_ptr<ICollider> collisor, PhysicsEngine &pysEngine) :
            transform_(transform), physicsEngine_(pysEngine) {
        collisor_ = std::move(collisor);
        pysEngine.addRigidBody(this);
    }

    void RigidBody::onChange() {
        physicsEngine_.updateRigidBody(this);
    }
} // entre_portais