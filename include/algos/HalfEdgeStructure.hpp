/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2022 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_HALF_EDGE_STRUCTURE_MESH_H
#define REACTPHYSICS3D_HALF_EDGE_STRUCTURE_MESH_H

// Libraries

#include <utility>
#include <vector>
#include <cassert>

namespace reactphysics3d {
    typedef unsigned int uint32;

// Class HalfEdgeStructure
/**
 * This class describes a polyhedron mesh made of faces and vertices.
 * The faces do not have to be triangle. Note that the half-edge structure
 * is only valid if the mesh is closed (each edge has two adjacent faces).
 */
    class HalfEdgeStructure {

    public:

        using VerticesPair = std::pair<uint32, uint32>;

        /// Edge
        struct Edge {
            uint32 vertexIndex;       // Index of the vertex at the beginning of the edge
            uint32 twinEdgeIndex;     // Index of the twin edge
            uint32 faceIndex;         // Adjacent face index of the edge
            uint32 nextEdgeIndex;     // Index of the next edge
        };

        /// Face
        struct Face {
            uint32 edgeIndex;             // Index of an half-edge of the face
            std::vector<uint32> faceVertices;    // Index of the vertices of the face

            /// Constructor
            Face() : edgeIndex(0), faceVertices() {}

            /// Constructor
            Face(std::vector<uint32> vertices) : edgeIndex(0), faceVertices(vertices) {}

//            explicit Face(const std::vector<uint32> &faceVertices) : faceVertices(faceVertices) {}
        };

        /// Vertex
        struct Vertex {
            uint32 vertexPointIndex;  // Index of the vertex point in the origin vertex array
            uint32 edgeIndex;         // Index of one edge emanting from this vertex

            /// Constructor
            Vertex(uint32 vertexCoordsIndex) : vertexPointIndex(vertexCoordsIndex), edgeIndex(0) {}

            Vertex() = default;
        };

    private:

        /// All the faces
        std::vector<Face> mFaces;

        /// All the vertices
        std::vector<Vertex> mVertices;

        /// All the half-edges
        std::vector<Edge> mEdges;

    public:

        /// Constructor
        HalfEdgeStructure(uint32 facesCapacity, uint32 verticesCapacity, uint32 edgesCapacity) : mFaces(),
                                                                                                 mVertices(),
                                                                                                 mEdges() {
            mFaces.reserve(facesCapacity);
            mVertices.reserve(verticesCapacity);
            mEdges.reserve(edgesCapacity);
        }

        /// Destructor
        ~HalfEdgeStructure() = default;

        /// Initialize the structure (when all vertices and faces have been added)
        void init();

        /// Add a vertex
        uint32 addVertex(uint32 vertexPointIndex);

        /// Add a face
        void addFace(std::vector<uint32> faceVertices);

        /// Return the number of faces
        uint32 getNbFaces() const;

        /// Return the number of half-edges
        uint32 getNbHalfEdges() const;

        /// Return the number of vertices
        uint32 getNbVertices() const;

        /// Return a given face
        const Face &getFace(uint32 index) const;

        /// Return a given edge
        const Edge &getHalfEdge(uint32 index) const;

        /// Return a given vertex
        const Vertex &getVertex(uint32 index) const;

    };

    class HalfEdgeStructureManager {
    private:
        HalfEdgeStructureManager();

        void init();

    public:

        void initBoxShapeHalfEdgeStructure();

        static HalfEdgeStructureManager &getInstance();

        HalfEdgeStructure &getBoxShapeHalfEdgeStructure();


        static HalfEdgeStructureManager *mInstance;
        HalfEdgeStructure mBoxShapeHalfEdgeStructure;
    };
}
#endif
