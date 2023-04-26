#ifndef ENTREPORTAIS_PHYSICSSCENE_HPP
#define ENTREPORTAIS_PHYSICSSCENE_HPP

#include <vector>
#include "characterkinematic/PxControllerManager.h"
#include "entrePortaisEngine/physics/PhysXAdaptor.hpp"
#include "entrePortaisEngine/render/Camera.hpp"
#include "extensions/PxDefaultAllocator.h"
#include <PxPhysicsAPI.h>

namespace entre_portais {

    class PhysicsActor;

    class PhysicsScene {
    public:
        PhysicsScene();

        ~PhysicsScene();

        PhysicsScene(const PhysicsScene &other) = default;

        PhysicsScene &operator=(const PhysicsScene &other) = default;

        void update(float deltaTime);

        void initialize();

        bool addRigidBody(PhysicsActor *rigidBody);

        void removeRigidBody(PhysicsActor *rigidBody);

        void updateRigidBody(PhysicsActor *rigidBody);

        void renderImGui(Camera &camera);

        static physx::PxPhysics &getPhysics();

        physx::PxScene &getScene() const;

        physx::PxControllerManager &getControllerManager() const;

    private:
        static inline entre_portais::PhysXAdaptor &getPhysXAdaptorRef() {
            return entre_portais::PhysXAdaptor::getInstance();
        }

        physx::PxAllocatorCallback &getAllocatorCallback();

        physx::PxScene *scene = nullptr;

        physx::PxCpuDispatcher *getDispatcher();

        physx::PxPvdSceneClient *pvdClient;
        physx::PxControllerManager *controllerManager;
    };

} // entre_portais

#endif  // ENTREPORTAIS_PHYSICSSCENE_HPP
