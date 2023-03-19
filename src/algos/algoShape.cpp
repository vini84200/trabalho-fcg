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

    bool ConvexPolyhedronShape::isPolyhedron() const {
        return true;
    }

    glm::vec3 BoxShape::getLocalSupportPointWithoutMargin(const glm::vec3 &direction) const {
        return glm::vec3(direction.x < 0.0F ? -halfExtents_.x : halfExtents_.x,
                         direction.y < 0.0F ? -halfExtents_.y : halfExtents_.y,
                         direction.z < 0.0F ? -halfExtents_.z : halfExtents_.z);
    }

    Shape::ShapeType BoxShape::getType() const {
        return CONVEX_POLYHEDRON;
    }

    uint32 BoxShape::getNbFaces() const {
        return 6;
    }

    const reactphysics3d::HalfEdgeStructure::Face &BoxShape::getFace(uint32 faceIndex) const {
        assert(faceIndex < getNbFaces());
        return reactphysics3d::HalfEdgeStructureManager::getInstance().getBoxShapeHalfEdgeStructure().getFace(
                faceIndex);
    }

    uint32 BoxShape::getNbVertices() const {
        return 8;
    }

    const reactphysics3d::HalfEdgeStructure::Vertex &BoxShape::getVertex(uint32 vertexIndex) const {
        assert(vertexIndex < getNbVertices());
        return reactphysics3d::HalfEdgeStructureManager::getInstance().getBoxShapeHalfEdgeStructure().getVertex(
                vertexIndex);
    }

    glm::vec3 BoxShape::getVertexPosition(uint32 vertexIndex) const {
        assert(vertexIndex < getNbVertices());

        switch (vertexIndex) {
            case 0:
                return glm::vec3(-halfExtents_.x, -halfExtents_.y, halfExtents_.z);
            case 1:
                return glm::vec3(halfExtents_.x, -halfExtents_.y, halfExtents_.z);
            case 2:
                return glm::vec3(halfExtents_.x, halfExtents_.y, halfExtents_.z);
            case 3:
                return glm::vec3(-halfExtents_.x, halfExtents_.y, halfExtents_.z);
            case 4:
                return glm::vec3(-halfExtents_.x, -halfExtents_.y, -halfExtents_.z);
            case 5:
                return glm::vec3(halfExtents_.x, -halfExtents_.y, -halfExtents_.z);
            case 6:
                return glm::vec3(halfExtents_.x, halfExtents_.y, -halfExtents_.z);
            case 7:
                return glm::vec3(-halfExtents_.x, halfExtents_.y, -halfExtents_.z);
        }

        assert(false);
        return glm::vec3(0);
    }

    glm::vec3 BoxShape::getFaceNormal(uint32 faceIndex) const {
        assert(faceIndex < getNbFaces());

        switch (faceIndex) {
            case 0:
                return glm::vec3(0, 0, 1);
            case 1:
                return glm::vec3(1, 0, 0);
            case 2:
                return glm::vec3(0, 0, -1);
            case 3:
                return glm::vec3(-1, 0, 0);
            case 4:
                return glm::vec3(0, -1, 0);
            case 5:
                return glm::vec3(0, 1, 0);
        }

        assert(false);
        return glm::vec3(0);
    }

    const reactphysics3d::HalfEdgeStructure::Edge &BoxShape::getHalfEdge(uint32 edgeIndex) const {
        assert(edgeIndex < getNbHalfEdges());
        return reactphysics3d::HalfEdgeStructureManager::getInstance().getBoxShapeHalfEdgeStructure().getHalfEdge(
                edgeIndex);
    }

    bool BoxShape::isPolyhedron() const {
        return true;
    }

    glm::vec3 BoxShape::getCentroid() const {
        return glm::vec3(0);
    }
}