#ifndef ENTREPORTAIS_PHYSXADAPTOR_HPP
#define ENTREPORTAIS_PHYSXADAPTOR_HPP

#include "entrePortaisEngine/render/Camera.hpp"
#include <vector>
#include <PxPhysicsAPI.h>

#define PVD_HOST "127.0.0.1"

namespace entre_portais {

    class MyErrorCallback : public physx::PxErrorCallback {
    public:
        void reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) override;
    };

/** @brief Interface para adaptadores de física
*/
    class PhysXAdaptor {
    public:
        static entre_portais::PhysXAdaptor &getInstance();

        void update(float deltaTime);

        // Delete move
        PhysXAdaptor(PhysXAdaptor &&) = delete;

        PhysXAdaptor &operator=(PhysXAdaptor &&) = delete;

        // Delete copy
        PhysXAdaptor(const PhysXAdaptor &) = delete;

        PhysXAdaptor &operator=(const PhysXAdaptor &) = delete;

        const physx::PxDefaultAllocator &getAllocator() const;

        const MyErrorCallback &getErrorCallback() const;

        physx::PxFoundation *getFoundation() const;

        physx::PxPhysics *getPhysics() const;

        physx::PxDefaultCpuDispatcher *getDispatcher() const;

        physx::PxPvd *getPvd() const;

        ~PhysXAdaptor();

    private:
        PhysXAdaptor();

        static PhysXAdaptor *instance;

        physx::PxDefaultAllocator allocator;
        MyErrorCallback errorCallback;
        physx::PxFoundation *foundation = nullptr;
        physx::PxPhysics *physics = nullptr;
        physx::PxDefaultCpuDispatcher *dispatcher = nullptr;
        physx::PxPvd *pvd = nullptr;
        physx::PxPvdTransport *transport;
    };
}

#endif //ENTREPORTAIS_PHYSXADAPTOR_HPP
