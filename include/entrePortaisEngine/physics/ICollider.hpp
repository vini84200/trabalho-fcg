#ifndef ENTREPORTAIS_ICOLLIDER_HPP
#define ENTREPORTAIS_ICOLLIDER_HPP

#include "Boxes.hpp"
#include "entrePortaisEngine/Transform.hpp"

namespace entre_portais {

    class ICollider {
    public:
        virtual ~ICollider() = default;

    public:
        virtual void Draw() = 0;

        virtual void renderImGui() = 0;

        virtual BoundingBox getBoundingBox(glm::mat4 modelMatrix) = 0;

        virtual BoundingBox getBoundingBox() = 0;
    };

} // entre_portais

#endif //ENTREPORTAIS_ICOLLIDER_HPP
