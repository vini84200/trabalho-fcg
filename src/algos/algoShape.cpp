#include "algos/algoShape.hpp"
#include "glm/gtx/norm.hpp"
#include "utils/utils.hpp"

namespace algo::shapes
{
    // FONTE: (adaptado) https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/shapes

    glm::vec3 Shape::getLocalSupportPointWithMargin(const glm::vec3& direction) const
    {
        // Get the support point without margin
        glm::vec3 supportPoint = getLocalSupportPointWithoutMargin(direction);

        if (margin_ != 0.0F) {

            // Add the margin to the support point
            glm::vec3 unitVec(0.0, -1.0, 0.0);
            if (glm::length2(direction) > MACHINE_EPSILON * MACHINE_EPSILON) {
                unitVec = glm::normalize(direction);
            }
            supportPoint += unitVec * margin_;
        }

        return supportPoint;
    }

    Shape::Shape(float margin) : margin_(margin) {
    }


}