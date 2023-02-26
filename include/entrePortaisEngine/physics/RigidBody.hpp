#ifndef ENTREPORTAIS_RIGIDBODY_HPP
#define ENTREPORTAIS_RIGIDBODY_HPP

#include "Boxes.hpp"
#include "entrePortaisEngine/Transform.hpp"
#include "PhysicsEngine.hpp"
#include "ICollider.hpp"

namespace entre_portais {

    class RigidBody {
    public:
        RigidBody(glm::mat4 *transform, std::unique_ptr<ICollider> collisor, PhysicsEngine &pysEngine,
                  Transform &transformToModify);

        void renderImGui();

        void onChange();

        PhysicsEngine &getPhysicsEngine() const;

        void setPhysicsEngine(PhysicsEngine &physicsEngine);

        void update(float deltaTime);

    private:
        // DYNAMICS
        bool isStatic_ = true;
        glm::vec3 velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 force_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 lastForce_ = glm::vec3(0.0f, 0.0f, 0.0f);
        float mass_ = 1.0f;
        glm::vec3 gravity_ = glm::vec3(0, -9.8, 0); // in m/s^2

        void updateDynamics(float deltaTime);

        void updateVelocity(float deltaTime);

        void updatePosition(float deltaTime);

        void updateGravity(float time);

    public:
        void applyForce(glm::vec3 force);

        bool isStatic() const;


    private:
        glm::mat4 *transform_;
        std::unique_ptr<ICollider> collider_;
    public:
        const std::unique_ptr<ICollider> &getCollider() const;

    private:
        Transform &transformToModify_;

        void onTransformChange();

        PhysicsEngine &physicsEngine_;
        int id_ = -1;


    public:
        BoundingBox getBoundingBox();

        void setId(int id);

        int getID() const;

    };

} // entre_portais

#endif //ENTREPORTAIS_RIGIDBODY_HPP
