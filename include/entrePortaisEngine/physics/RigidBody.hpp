#ifndef ENTREPORTAIS_RIGIDBODY_HPP
#define ENTREPORTAIS_RIGIDBODY_HPP

#include "Boxes.hpp"
#include "entrePortaisEngine/Transform.hpp"
#include "PhysicsEngine.hpp"
#include "ICollider.hpp"

namespace entre_portais {

    class RigidBody {
    public:
        RigidBody(glm::mat4 *transform, std::unique_ptr<ICollider> collisor, PhysicsEngine &pysEngine);

        void renderImGui();

        void onChange();


    private:
        glm::mat4 *transform_;
        std::unique_ptr<ICollider> collisor_;

        void onTransformChange();

        PhysicsEngine &physicsEngine_;
        int id_;
    public:
        PhysicsEngine &getPhysicsEngine() const;

        void setPhysicsEngine(PhysicsEngine &physicsEngine);


    public:
        BoundingBox getBoundingBox();

        void setId(int id);

        int getID() const;
    };

} // entre_portais

#endif //ENTREPORTAIS_RIGIDBODY_HPP
