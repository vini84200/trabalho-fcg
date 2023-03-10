#ifndef ENTREPORTAIS_ALGOSHAPE_HPP
#define ENTREPORTAIS_ALGOSHAPE_HPP

#include "glm/vec3.hpp"
#include "glm/gtx/norm.hpp"
#include "HalfEdgeStructure.hpp"

namespace algo::shapes
{
    using uint32 = unsigned int;
// A 3D Shape
// FONTE: (adaptado) https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/shapes/ConvexShape.h
    class Shape {
    public:
        glm::vec3 getLocalSupportPointWithMargin(const glm::vec3& direction) const;
        virtual glm::vec3 getLocalSupportPointWithoutMargin(const glm::vec3& direction) const = 0;
        virtual bool isPolyhedron() const = 0;

        explicit Shape(float margin);

        virtual ~Shape() = default;

        float getMargin() const {
            return margin_;
        }

        void setMargin(float margin) {
            margin_ = margin;
        }

        enum ShapeType {
            SPHERE,
            BOX,
            CONVEX_POLYHEDRON,
            CAPSULE,
            CONVEX_MESH,
        };

        virtual ShapeType getType() const = 0;

    private:
        float margin_;
    };


    class ConvexPolyhedronShape : public Shape {
    public:
        ConvexPolyhedronShape(float margin) : Shape(margin) {}

        ~ConvexPolyhedronShape() override = default;

        /// Return the number of faces of the polyhedron
        virtual uint32 getNbFaces() const=0;

        /// Return a given face of the polyhedron
        virtual const reactphysics3d::HalfEdgeStructure::Face& getFace(uint32 faceIndex) const=0;

        /// Return the number of vertices of the polyhedron
        virtual uint32 getNbVertices() const=0;

        /// Return a given vertex of the polyhedron
        virtual const reactphysics3d::HalfEdgeStructure::Vertex& getVertex(uint32 vertexIndex) const=0;

        /// Return the position of a given vertex
        virtual glm::vec3 getVertexPosition(uint32 vertexIndex) const=0;

        /// Return the normal vector of a given face of the polyhedron
        virtual glm::vec3 getFaceNormal(uint32 faceIndex) const=0;

        /// Return the number of half-edges of the polyhedron
        virtual uint32 getNbHalfEdges() const=0;

        /// Return a given half-edge of the polyhedron
        virtual const reactphysics3d::HalfEdgeStructure::Edge& getHalfEdge(uint32 edgeIndex) const=0;

        /// Return true if the collision shape is a polyhedron
        bool isPolyhedron() const override;

        /// Return the centroid of the polyhedron
        virtual glm::vec3 getCentroid() const = 0;

        /// Find and return the index of the polyhedron face with the most anti-parallel face
        /// normal given a direction vector
        uint32 findMostAntiParallelFace(const glm::vec3 &direction) const;
    };


    class SphereShape : public Shape {
    public:
        SphereShape(float radius) : Shape(radius) {};

        virtual ~SphereShape() = default;

        float getRadius() const {
            return getMargin();
        }

        void setRadius(float radius) {
            setMargin(radius);
        }

        glm::vec3 getLocalSupportPointWithoutMargin(const glm::vec3 &direction) const override;

        virtual bool isPolyhedron() const override;

        virtual ShapeType getType() const override {
            return SPHERE;
        }
    };

    class CapsuleShape : public Shape {
    private:
        float halfHeight_;
    public:
        CapsuleShape(float radius, float height) : Shape(radius), halfHeight_(height / 2.0F) {};

        glm::vec3 getLocalSupportPointWithoutMargin(const glm::vec3 &direction) const override;

        float getRadius() const {
            return getMargin();
        }

        void setRadius(float radius) {
            setMargin(radius);
        }

        float getHeight() const {
            return halfHeight_ * 2.0F;
        }

        void setHeight(float height) {
            halfHeight_ = height / 2.0F;
        }

        bool isPolyhedron() const override;

        virtual ShapeType getType() const override {
            return CAPSULE;
        }
    };

    class BoxShape : public ConvexPolyhedronShape {
    private:
        glm::vec3 halfExtents_;
    public:
        BoxShape(const glm::vec3 &halfExtents) : ConvexPolyhedronShape(0), halfExtents_(halfExtents) {};


        const glm::vec3 &getHalfExtents() const {
            return halfExtents_;
        }

        void setHalfExtents(const glm::vec3 &halfExtents) {
            halfExtents_ = halfExtents;
        }


        glm::vec3 getLocalSupportPointWithoutMargin(const glm::vec3 &direction) const override;

        ~BoxShape() override = default;

        ShapeType getType() const override;

        uint32 getNbFaces() const override;

        const reactphysics3d::HalfEdgeStructure::Face &getFace(uint32 faceIndex) const override;

        uint32 getNbVertices() const override;

        const reactphysics3d::HalfEdgeStructure::Vertex &getVertex(uint32 vertexIndex) const override;

        glm::vec3 getVertexPosition(uint32 vertexIndex) const override;

        glm::vec3 getFaceNormal(uint32 faceIndex) const override;

        uint32 getNbHalfEdges() const override {
            return 24;
        }

        const reactphysics3d::HalfEdgeStructure::Edge &getHalfEdge(uint32 edgeIndex) const override;

        bool isPolyhedron() const override;

        glm::vec3 getCentroid() const override;
    };
}

#endif  // ENTREPORTAIS_ALGOSHAPE_HPP
