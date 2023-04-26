#ifndef ENTREPORTAIS_PHYSICSACTOR_HPP
#define ENTREPORTAIS_PHYSICSACTOR_HPP

#include "PhysicsScene.hpp"
#include "PxActor.h"
#include "PxRigidActor.h"
#include "PxRigidBody.h"
#include "entrePortaisEngine/Transform.hpp"
#include "entrePortaisEngine/render/Camera.hpp"

namespace entre_portais {

    class PhysicsActor {
    public:
        PhysicsActor(PhysicsScene &physicsEngine, Transform &transform, const physx::PxGeometry &geometry,
                     const physx::PxMaterial &material, const physx::PxReal density, bool isDynamic);

        void renderImGui(Camera *camera);

        void onChange();

        void update(float deltaTime);

        void postPhysics(float deltaTime);

        void setTransform(Transform &transform);

        PhysicsScene &getPhysicsEngine() const;

        void setPhysicsEngine(PhysicsScene &physicsEngine);

    private:
        physx::PxActor *actor_ = nullptr;
        physx::PxRigidActor *body = nullptr;
        physx::PxShape *shape_;
        Transform &localTransform_;
    };

} // entre_portais

#endif //ENTREPORTAIS_PHYSICSACTOR_HPP
