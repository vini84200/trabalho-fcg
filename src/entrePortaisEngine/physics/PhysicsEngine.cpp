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
        collisions.clear();

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
                collisions.emplace_back(rigidBody1, rigidBody2, collisionInfo);
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
                spdlog::info("Colisão com objeto estático");
                auto const &reverseCollision = collision.reverse();
                rigidBody2->resolveCollisionWithStatic(rigidBody1, reverseCollision);
                continue;
            }
            if (rigidBody2->isStatic()) {
                // Resolve colisão com objeto estático
                spdlog::info("Colisão com objeto estático");
                rigidBody1->resolveCollisionWithStatic(rigidBody2, collision);
                continue;
            }

            // Resolve colisão com objeto dinâmico
            spdlog::info("Colisão com objeto dinâmico");
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

    void PhysicsEngine::renderImGui(Camera &camera) {
        ImGui::Begin("Physics Engine");
        // Show octree
        // TODO: implementar octree render

        // Show rigid bodies
        // Draw rigid bodies bounding boxes
        bool drawBoundingBoxes = ImGui::GetStateStorage()->GetBool(ImGui::GetID("Draw bounding boxes"), false);
        if (ImGui::Checkbox("Draw bounding boxes", &drawBoundingBoxes)) {
            ImGui::GetStateStorage()->SetBool(ImGui::GetID("Draw bounding boxes"), drawBoundingBoxes);
        }
        if (drawBoundingBoxes) {
            for (auto const &rigidBody: rigidBodies_) {
                auto const &bb = rigidBody->getBoundingBox();
                auto const &min = bb.min;
                auto const &max = bb.max;
                glm::vec3 const center = (min + max) / 2.0f;
                glm::vec3 const size = max - min;
                auto const color = rigidBody->isStatic() ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
                camera.debugDrawCube(center, size, color);
            }
        }

        // Show collisions

        bool drawCollisions = ImGui::GetStateStorage()->GetBool(ImGui::GetID("Draw collisions"), false);
        if (ImGui::Checkbox("Draw collisions", &drawCollisions)) {
            ImGui::GetStateStorage()->SetBool(ImGui::GetID("Draw collisions"), drawCollisions);
        }
        if (drawCollisions) {
            for (auto const &[rigidBody1, rigidBody2, collision]: collisions) {
                auto const &bb1 = rigidBody1->getBoundingBox();
                auto const &bb2 = rigidBody2->getBoundingBox();
                auto const &min1 = bb1.min;
                auto const &max1 = bb1.max;
                auto const &min2 = bb2.min;
                auto const &max2 = bb2.max;
                glm::vec3 const center1 = (min1 + max1) / 2.0f;
                glm::vec3 const center2 = (min2 + max2) / 2.0f;
                glm::vec3 const size1 = max1 - min1;
                glm::vec3 const size2 = max2 - min2;
                auto const color1 = rigidBody1->isStatic() ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
                auto const color2 = rigidBody2->isStatic() ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
                camera.debugDrawCube(center1, size1, color1);
                camera.debugDrawCube(center2, size2, color2);

                // Draw collision normal
                glm::vec3 const normal = collision.normal;
                camera.debugDrawLine(center1, center1 + normal, IM_COL32(0, 0, 255, 255), 1.f);
                camera.debugDrawLine(center2, center2 - normal, IM_COL32(0, 0, 255, 255), 1.f);

                // Draw collision manifold

            }
        }
        ImGui::End();

    }
} // entre_portais