#include "entrePortaisEngine/physics/PhysicsEngine.hpp"
#include "entrePortaisEngine/physics/RigidBody.hpp"
#include "spdlog/spdlog.h"

namespace entre_portais {
    PhysicsEngine::PhysicsEngine()
            : octreeBbc_(std::vector<BoundingBox>(),
                         MAX_OCTREE_DEPTH,
                         BoundingBox(glm::vec3(-MAX_OCTREE_SIZE, -MAX_OCTREE_SIZE, -MAX_OCTREE_SIZE),
                                     glm::vec3(MAX_OCTREE_SIZE, MAX_OCTREE_SIZE, MAX_OCTREE_SIZE)),
                         MAX_OCTREE_OBJECTS,
                         OCTREE_PARALLELISM) {


    }

    void PhysicsEngine::update(float deltaTime) {
        // Detecta colisões
        auto collisions = octreeBbc_.CollisionDetection();
        for (auto const &collision: collisions) {
            auto const &rigidBody1 = rigidBodies_[collision.first];
            auto const &rigidBody2 = rigidBodies_[collision.second];
            spdlog::warn("Possivel colisão entre {} e  {}", rigidBody1->getID(), rigidBody2->getID());
        }
    }

    void PhysicsEngine::initialize() {

    }

    bool PhysicsEngine::addRigidBody(RigidBody *rigidBody) {
        bool ok = octreeBbc_.Add(rigidBody->getBoundingBox());
        if (ok) {
            rigidBodies_.push_back(rigidBody);
            int const id = rigidBodies_.size() - 1;
            rigidBody->setPhysicsEngine(*this);
            rigidBody->setId(id);
        }
        return ok;
    }

    void PhysicsEngine::removeRigidBody(RigidBody *rigidBody) {
        octreeBbc_.Erase(rigidBody->getID());
        rigidBodies_.erase(std::find(rigidBodies_.begin(), rigidBodies_.end(), rigidBody));

    }

    void PhysicsEngine::updateRigidBody(RigidBody *rigidBody) {
        octreeBbc_.Update(rigidBody->getID(), rigidBody->getBoundingBox());

    }
} // entre_portais