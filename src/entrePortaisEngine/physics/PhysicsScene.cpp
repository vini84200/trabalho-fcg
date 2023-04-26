#include <list>

#include "entrePortaisEngine/physics/PhysicsScene.hpp"
#include "entrePortaisEngine/physics/PhysicsActor.hpp"
#include "PxPhysicsAPI.h"
#include "spdlog/spdlog.h"

namespace entre_portais {
    PhysicsScene::PhysicsScene() {
        spdlog::info("PhysicsScene::PhysicsScene()");
        physx::PxSceneDesc sceneDesc(getPhysXAdaptorRef().getPhysics()->getTolerancesScale());
        sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = getDispatcher();
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
        scene = getPhysXAdaptorRef().getPhysics()->createScene(sceneDesc);

        pvdClient = scene->getScenePvdClient();
        if (pvdClient != nullptr) {
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
        else {
            spdlog::info("No PVD client found");
        }

        controllerManager = PxCreateControllerManager(*scene);



    }

    void PhysicsScene::update(float deltaTime) {
        scene->simulate(deltaTime);
        scene->fetchResults(true); // TODO: Find a way to not block the thread here

        unsigned int const numActors = scene->getNbActors(
                physx::PxActorTypeFlag::eRIGID_DYNAMIC);
        if (numActors > 0) {
            std::vector<physx::PxActor*> actors(numActors);
            scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC,
                             actors.data(), numActors);
            for (const auto *actor : actors) {
                if (actor->userData != nullptr) {
                    auto *rigidBody = static_cast<PhysicsActor *>(actor->userData);
                    rigidBody->postPhysics(deltaTime);
                }
                else {
//                    spdlog::warn("Actor without user data");
//                  FIXME: Check if this is a problem
                }
            }
        }
    }

    void PhysicsScene::initialize() {
        // TODO: implementar

    }

    bool PhysicsScene::addRigidBody(PhysicsActor *rigidBody) {
        // TODO: implementar
        return false;
    }

    void PhysicsScene::removeRigidBody(PhysicsActor *rigidBody) {
        // TODO: implementar
    }

    void PhysicsScene::updateRigidBody(PhysicsActor *rigidBody) {
        // TODO: implementar
    }

    void PhysicsScene::renderImGui(Camera &camera) {
        // TODO: implementar
    }

    physx::PxAllocatorCallback & PhysicsScene::getAllocatorCallback() {
        return (physx::PxAllocatorCallback &) getPhysXAdaptorRef().getAllocator();
    }

    physx::PxCpuDispatcher *PhysicsScene::getDispatcher() {
        return getPhysXAdaptorRef().getDispatcher();
    }

    PhysicsScene::~PhysicsScene() {
        spdlog::info("PhysicsScene::~PhysicsScene()");
        if (scene != nullptr) {
            scene->release();
            scene = nullptr;
        }

    }

    physx::PxPhysics &PhysicsScene::getPhysics() {
       return *getPhysXAdaptorRef().getPhysics();
    }

    physx::PxScene &PhysicsScene::getScene() const {
        return *scene;
    }

    physx::PxControllerManager &PhysicsScene::getControllerManager() const {
        return *controllerManager;
    }
} // entre_portais
