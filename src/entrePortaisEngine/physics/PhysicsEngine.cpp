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
            if (collision.first == collision.second) {
                continue;
            }
            auto const &rigidBody1 = rigidBodies_[collision.first];
            auto const &rigidBody2 = rigidBodies_[collision.second];
            // Check if both have colliders
            if (rigidBody1->getCollider() == nullptr || rigidBody2->getCollider() == nullptr) {
                continue;
            }
            // Check if both are static
            if (rigidBody1->isStatic() && rigidBody2->isStatic()) {
                // Static objects don't have to be checked for collisions
                // Unless they have a trigger collider //TODO: implement triggers
                continue;
            }
            auto const &collider1 = rigidBody1->getCollider().get();
            auto const &collider2 = rigidBody2->getCollider().get();

            auto collisionInfo = collider1->isColliding(collider2);
            if (collisionInfo.isColliding) {
                // COllision detected
                spdlog::warn("Colisão entre {} e  {}", rigidBody1->getID(), rigidBody2->getID());
            }

//            spdlog::warn("Possivel colisão entre {} e  {}", rigidBody1->getID(), rigidBody2->getID());
        }

        // Atualiza dinâmica
        for (auto const &rigidBody: rigidBodies_) {
            rigidBody->update(deltaTime);
        }
    }

    void PhysicsEngine::initialize() {

    }

    bool PhysicsEngine::addRigidBody(RigidBody *rigidBody) {
        auto bb = rigidBody->getBoundingBox();
        bool ok = octreeBbc_.Add(bb);
        if (ok) {
            rigidBodies_.push_back(rigidBody);
            int const id = rigidBodies_.size() - 1;
            rigidBody->setPhysicsEngine(*this);
            rigidBody->setId(id);
        } else {
            spdlog::error("Não foi possível adicionar RigidBody ao PhysicsEngine");
        }
        return ok;
    }

    void PhysicsEngine::removeRigidBody(RigidBody *rigidBody) {
        if (rigidBody->getID() < 0 || rigidBody->getID() >= rigidBodies_.size()) {
            spdlog::error("RigidBody {} não está no PhysicsEngine", rigidBody->getID());
            return;
        }
        octreeBbc_.Erase(rigidBody->getID());
        rigidBodies_.erase(std::find(rigidBodies_.begin(), rigidBodies_.end(), rigidBody));

    }

    void PhysicsEngine::updateRigidBody(RigidBody *rigidBody) {
        bool ok = octreeBbc_.Update(rigidBody->getID(), rigidBody->getBoundingBox());
        if (!ok) {
            spdlog::error("Não foi possível atualizar RigidBody {} no PhysicsEngine", rigidBody->getID());
        }

    }
} // entre_portais