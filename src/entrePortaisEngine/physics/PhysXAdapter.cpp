#include "entrePortaisEngine/physics/PhysXAdaptor.hpp"
#include "pvd/PxPvdTransport.h"


namespace entre_portais {

    entre_portais::PhysXAdaptor *PhysXAdaptor::instance{nullptr};


    PhysXAdaptor::PhysXAdaptor() {
        spdlog::info("PhysXAdaptor::PhysXAdaptor() with PhysX");

        foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
        if (!foundation) {
            spdlog::error("PxCreateFoundation failed!");
        } else {
            spdlog::info("PxCreateFoundation success!");
        }
        pvd = PxCreatePvd(*foundation);
        transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
        pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pvd);
        if (physics != nullptr) {
            spdlog::info("PxCreatePhysics success!");
        } else {
            spdlog::error("PxCreatePhysics failed!");
        }

        dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // TODO: Find out how many threads to use
        // TODO: Integrate with own TaskManager



    }

    entre_portais::PhysXAdaptor &PhysXAdaptor::getInstance() {
        if (instance == nullptr) {
            instance = new PhysXAdaptor();
        }
        return *instance;
    }

    const physx::PxDefaultAllocator &PhysXAdaptor::getAllocator() const {
        return allocator;
    }

    const MyErrorCallback &PhysXAdaptor::getErrorCallback() const {
        return errorCallback;
    }

    physx::PxFoundation *PhysXAdaptor::getFoundation() const {
        return foundation;
    }

    physx::PxPhysics *PhysXAdaptor::getPhysics() const {
        return physics;
    }

    physx::PxDefaultCpuDispatcher *PhysXAdaptor::getDispatcher() const {
        return dispatcher;
    }

    physx::PxPvd *PhysXAdaptor::getPvd() const {
        return pvd;
    }

    PhysXAdaptor::~PhysXAdaptor() {
        if (physics != nullptr) {
            physics->release();
        }
        if (pvd != nullptr) {
            pvd->release();
        }
        if (transport != nullptr) {
            transport->release();
        }
        if (dispatcher != nullptr) {
            dispatcher->release();
        }
        if (foundation != nullptr) {
            foundation->release();
        }

    }

    void MyErrorCallback::reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) {
        spdlog::error("PhysX Error: {} in {} at line {}", message, file, line);
    }
}