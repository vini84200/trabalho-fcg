#ifndef ENTREPORTAIS_ALGOSHAPE_HPP
#define ENTREPORTAIS_ALGOSHAPE_HPP

#include "glm/vec3.hpp"
#include "HalfEdgeStructure.hpp"

namespace algo::shapes
{
    typedef unsigned int uint32;
// A 3D Shape
// FONTE: (adaptado) https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/shapes/ConvexShape.h
    class Shape {
    public:
        glm::vec3 getLocalSupportPointWithMargin(const glm::vec3& direction) const;
        virtual glm::vec3 getLocalSupportPointWithoutMargin(const glm::vec3& direction) const = 0;
        virtual bool isPolyhedron() const = 0;

        Shape(float margin);
        virtual ~Shape() = default;

        float getMargin() const {
            return margin_;
        }

        void setMargin(float margin) {
            margin_ = margin;
        }

    private:
        float margin_;
    };


    class ConvexPolyhedronShape : public Shape {
    public:
        ConvexPolyhedronShape(float margin) : Shape(margin) {}
        virtual ~ConvexPolyhedronShape() = default;

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
        virtual bool isPolyhedron() const override;

        /// Return the centroid of the polyhedron
        virtual glm::vec3 getCentroid() const=0;

        /// Find and return the index of the polyhedron face with the most anti-parallel face
        /// normal given a direction vector
        uint32 findMostAntiParallelFace(const glm::vec3& direction) const;
    };
}

#endif  // ENTREPORTAIS_ALGOSHAPE_HPP
