#ifndef ENTREPORTAIS_PHYSICSENGINE_HPP
#define ENTREPORTAIS_PHYSICSENGINE_HPP

#include <vector>
#include "OctreeAdaptor.hpp"
#include "entrePortaisEngine/render/Camera.hpp"
#include "collisions.hpp"

#define MAX_OCTREE_DEPTH 10
#define MAX_OCTREE_OBJECTS 10

#define MAX_OCTREE_SIZE 100
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

        void renderImGui(Camera &camera);


        PhysicsEngine();

    private:
        std::vector<std::tuple<RigidBody *, RigidBody *, collisions::PossibleCollision>> collisions;
        std::vector<RigidBody *> rigidBodies_;
        OctreeBBC octreeBbc_;


    };

} // entre_portais

#endif //ENTREPORTAIS_PHYSICSENGINE_HPP
