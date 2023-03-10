#ifndef ENTREPORTAIS_RIGIDBODY_HPP
#define ENTREPORTAIS_RIGIDBODY_HPP

#include "Boxes.hpp"
#include "entrePortaisEngine/Transform.hpp"
#include "PhysicsEngine.hpp"
#include "ICollider.hpp"
#include "entrePortaisEngine/render/Camera.hpp"

namespace entre_portais {

    class RigidBody {
    public:
        RigidBody(glm::mat4 *transform, std::unique_ptr<ICollider> collisor, PhysicsEngine &pysEngine,
                  Transform &transformToModify);

        void renderImGui(Camera *camera);

        void onChange();

        PhysicsEngine &getPhysicsEngine() const;

        void setPhysicsEngine(PhysicsEngine &physicsEngine);

        void update(float deltaTime);

    private:
        // DYNAMICS
        bool isStatic_ = true;
        glm::vec3 velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
    public:
        const glm::vec3 &getVelocity() const;

    private:
        glm::vec3 force_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 lastForce_ = glm::vec3(0.0f, 0.0f, 0.0f);
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
    public:
        glm::mat4 *getTransform() const;

    private:
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

        bool onCollision(RigidBody *const other);

        void resolveCollision(RigidBody *const other, const collisions::PossibleCollision &possibleCollision,
                              glm::vec3 velDiff);

        void resolveCollisionWithStatic(RigidBody *const other, const collisions::PossibleCollision &possibleCollision);

    public:

    private:
        float mass_ = 1.0f;
        float restitution_ = 0.8f;
    public:
        float getMass() const;

        float getRestitution() const;

        float getFriction() const;

    private:
        float friction_ = 0.5f;

    };

} // entre_portais

#endif //ENTREPORTAIS_RIGIDBODY_HPP
