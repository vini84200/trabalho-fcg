#include <list>
#include "entrePortaisEngine/physics/PhysicsEngine.hpp"
#include "entrePortaisEngine/physics/collisions.hpp"
#include "entrePortaisEngine/physics/RigidBody.hpp"
#include "spdlog/spdlog.h"

namespace entre_portais {
    PhysicsEngine::PhysicsEngine() : octreeBbc_(std::vector<BoundingBox>(), MAX_OCTREE_DEPTH, BoundingBox(
            glm::vec3(-MAX_OCTREE_SIZE, -MAX_OCTREE_SIZE, -MAX_OCTREE_SIZE),
            glm::vec3(MAX_OCTREE_SIZE, MAX_OCTREE_SIZE, MAX_OCTREE_SIZE)), MAX_OCTREE_OBJECTS, OCTREE_PARALLELISM) {


    }

    void PhysicsEngine::update(float deltaTime) {
        // Detecta colisões
        auto possible_collisions = octreeBbc_.CollisionDetection();

        std::vector<std::tuple<RigidBody *, RigidBody *, collisions::PossibleCollision>> collisions;
        for (auto const &collision: possible_collisions) {
            if (collision.first == collision.second) {
                continue;
            }
            auto const &rigidBody1 = rigidBodies_[collision.first];
            auto const &rigidBody2 = rigidBodies_[collision.second];
            // Check if both have colliders
            if (rigidBody1->getCollider() == nullptr || rigidBody2->getCollider() == nullptr) {
                continue;
            }
            auto const &collider1 = rigidBody1->getCollider().get();
            auto const &collider2 = rigidBody2->getCollider().get();

            auto collisionInfo = collider1->isColliding(collider2);
            if (collisionInfo.isColliding) {
                // COllision detected
                collisions.push_back(std::make_tuple(rigidBody1, rigidBody2, collisionInfo));
            }

//            spdlog::warn("Possivel colisão entre {} e  {}", rigidBody1->getID(), rigidBody2->getID());
        }

        // Resolve colisões

        for (auto const &[rigidBody1, rigidBody2, collision]: collisions) {
            bool cont = rigidBody1->onCollision(rigidBody2);
            cont |= rigidBody2->onCollision(rigidBody1);

            if (!cont) {
                continue;
            }


            // Resolve colisões
            if (rigidBody1->isStatic() && rigidBody2->isStatic()) {
                // Static objects don't have to be checked for possible_collisions
                // Unless they have a trigger collider //TODO: implement triggers
//                spdlog::warn("Colisão entre objetos estáticos");
                continue;
            }

            if (rigidBody1->isStatic()) {
                // Resolve colisão com objeto estático
                spdlog::warn("Colisão com objeto estático");
                auto const &reverseCollision = collision.reverse();
                rigidBody2->resolveCollisionWithStatic(rigidBody1, reverseCollision);
                continue;
            }
            if (rigidBody2->isStatic()) {
                // Resolve colisão com objeto estático
                spdlog::warn("Colisão com objeto estático");
                rigidBody1->resolveCollisionWithStatic(rigidBody2, collision);
                continue;
            }

            // Resolve colisão com objeto dinâmico
            spdlog::warn("Colisão com objeto dinâmico");
            auto const &reverseCollision = collision.reverse();
            glm::vec3 velDiff = rigidBody1->getVelocity() - rigidBody2->getVelocity();
            rigidBody1->resolveCollision(rigidBody2, collision, velDiff);
            rigidBody2->resolveCollision(rigidBody1, reverseCollision, -velDiff);


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