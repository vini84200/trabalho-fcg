#include "algos/algoShape.hpp"
#include "utils/utils.hpp"

namespace algo::shapes {
    // FONTE: (adaptado) https://github.com/DanielChappuis/reactphysics3d/tree/master/src/collision/shapes

    glm::vec3 Shape::getLocalSupportPointWithMargin(const glm::vec3 &direction) const {
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


    bool SphereShape::isPolyhedron() const {
        return false;
    }

    glm::vec3 SphereShape::getLocalSupportPointWithoutMargin(const glm::vec3 &direction) const {
        return glm::vec3(0.0, 0.0, 0.0);
    }

    glm::vec3 CapsuleShape::getLocalSupportPointWithoutMargin(const glm::vec3 &direction) const {
        float dotPrdouctTop = halfHeight_ * direction.y;
        float dotPrdouctBottom = -halfHeight_ * direction.y;

        if (dotPrdouctTop > dotPrdouctBottom) {
            return glm::vec3(0.0, halfHeight_, 0.0);
        } else {
            return glm::vec3(0.0, -halfHeight_, 0.0);
        }
    }

    bool CapsuleShape::isPolyhedron() const {
        return false;
    }

    uint32 ConvexPolyhedronShape::findMostAntiParallelFace(const glm::vec3 &direction) const {
        float minDotProduct = std::numeric_limits<float>::max();
        uint32 mostAntiParallelFace = 0;

        // For each face of the polyhedron
        const uint32 nbFaces = getNbFaces();
        for (uint32 i = 0; i < nbFaces; i++) {

            // Get the face normal
            const float dotProduct = glm::dot(getFaceNormal(i), direction);
            if (dotProduct < minDotProduct) {
                minDotProduct = dotProduct;
                mostAntiParallelFace = i;
            }
        }

        return mostAntiParallelFace;
    }
}