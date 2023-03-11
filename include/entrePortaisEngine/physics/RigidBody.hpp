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
    public:
        void setIsStatic(bool isStatic);

    public:
        const glm::vec3 &getForce() const;

    private:
        glm::vec3 velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 force_ = glm::vec3(0.0f, 0.0f, 0.0f);
        // Rotation
        glm::vec3 angularVelocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 torque_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat3 inertiaTensor_ = glm::identity<glm::mat3>();

        glm::vec3 lastForce_ = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 gravity_ = glm::vec3(0, -9.8, 0); // in m/s^2

    public:


        const glm::vec3 &getVelocity() const;

        void updateDynamics(float deltaTime);

        void updateVelocity(float deltaTime);

        void updatePosition(float deltaTime);

        void updateGravity(float time);

    public:
        void applyForceOnCM(glm::vec3 force);

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

        void
        resolveCollision(RigidBody *const other, const collisions::PossibleCollision &possibleCollision,
                         glm::vec3 velA, glm::vec3 velB, glm::vec3 velAngA, glm::vec3 velAngB, float dt);

        void resolveCollisionWithStatic(RigidBody *const other, const collisions::PossibleCollision &possibleCollision,
                                        float dt);

    public:

    private:
        float mass_ = 1.0f;
        float restitution_ = 0.8f;
    public:
        float getMass() const;

        float getRestitution() const;

        float getFriction() const;

        glm::vec3 getAngularVelocity() const;

    private:
        float friction_ = 0.5f;

        float getDampingFactor();

        glm::vec3 getVelocityAt(const glm::vec3 r) const;
    };

} // entre_portais

#endif //ENTREPORTAIS_RIGIDBODY_HPP
