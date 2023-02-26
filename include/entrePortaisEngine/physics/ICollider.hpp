#ifndef ENTREPORTAIS_ICOLLIDER_HPP
#define ENTREPORTAIS_ICOLLIDER_HPP

#include "Boxes.hpp"
#include "entrePortaisEngine/Transform.hpp"
#include "collisions.hpp"

namespace entre_portais {
    class BoxCollider;

    class SphereCollider;

    class ICollider {
    public:
        ICollider(glm::mat4 &modelMatrix) : modelMatrix_(modelMatrix) {}

        virtual ~ICollider() = default;


    public: // Collisions
        virtual collisions::PossibleCollision isColliding(ICollider *other) = 0;

        virtual collisions::PossibleCollision isColliding(glm::vec4 point) = 0;

        virtual collisions::PossibleCollision isColliding(BoxCollider box) = 0;

        virtual collisions::PossibleCollision isColliding(SphereCollider sphere) = 0;


    public:
        virtual void Draw() = 0;

        virtual void renderImGui() = 0;

        virtual BoundingBox getBoundingBox(glm::mat4 modelMatrix) = 0;

        virtual BoundingBox getBoundingBox() = 0;

    protected:
        glm::mat4 &modelMatrix_;
    public:
        glm::mat4 &getModelMatrix() const {
            return modelMatrix_;
        }
    };

} // entre_portais

#endif //ENTREPORTAIS_ICOLLIDER_HPP
