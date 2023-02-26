#ifndef ENTREPORTAIS_PHYSICSENGINE_HPP
#define ENTREPORTAIS_PHYSICSENGINE_HPP

#include <vector>
#include "OctreeAdaptor.hpp"

#define MAX_OCTREE_DEPTH 10
#define MAX_OCTREE_OBJECTS 10

#define MAX_OCTREE_SIZE 10000
#define OCTREE_PARALLELISM true

namespace entre_portais {
    class RigidBody;

    class PhysicsEngine {
    public:

        void update(float deltaTime);

        void initialize();

        bool addRigidBody(RigidBody *rigidBody);

        void removeRigidBody(RigidBody *rigidBody);

        void updateRigidBody(RigidBody *rigidBody);


        PhysicsEngine();

    private:
        std::vector<RigidBody *> rigidBodies_;
        OctreeBBC octreeBbc_;


    };

} // entre_portais

#endif //ENTREPORTAIS_PHYSICSENGINE_HPP
