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

// Libraries
#include <map>
#include "algos/HalfEdgeStructure.hpp"

using namespace reactphysics3d;
// Initialize the structure (when all vertices and faces have been added)
// Add a vertex
/**
 * @param vertexPointIndex Index of the vertex in the vertex data array
 */

uint32 HalfEdgeStructure::addVertex(uint32 vertexPointIndex) {
    Vertex const vertex(vertexPointIndex);
    mVertices.push_back(vertex);
    return static_cast<uint32>(mVertices.size()) - 1;
}

// Add a face
/**
 * @param faceVertices Array of the vertices in a face (ordered in CCW order as seen from outside
 *                     the polyhedron
 */
void HalfEdgeStructure::addFace(std::vector<uint32> faceVertices) {

    // Create a new face
    Face face(faceVertices);
    mFaces.push_back(face);
}

// Return the number of faces
/**
 * @return The number of faces in the polyhedron
 */
uint32 HalfEdgeStructure::getNbFaces() const {
    return static_cast<uint32>(mFaces.size());
}

// Return the number of edges
/**
 * @return The number of edges in the polyhedron
 */
uint32 HalfEdgeStructure::getNbHalfEdges() const {
    return static_cast<uint32>(mEdges.size());
}

// Return the number of vertices
/**
 * @return The number of vertices in the polyhedron
 */
uint32 HalfEdgeStructure::getNbVertices() const {
    return static_cast<uint32>(mVertices.size());
}

// Return a given face
/**
 * @return A given face of the polyhedron
 */
const HalfEdgeStructure::Face &HalfEdgeStructure::getFace(uint32 index) const {
    assert(index < mFaces.size());
    return mFaces[index];
}

// Return a given edge
/**
 * @return A given edge of the polyhedron
 */
const HalfEdgeStructure::Edge &HalfEdgeStructure::getHalfEdge(uint32 index) const {
    assert(index < mEdges.size());
    return mEdges[index];
}

// Return a given vertex
/**
 * @return A given vertex of the polyhedron
 */
const HalfEdgeStructure::Vertex &HalfEdgeStructure::getVertex(uint32 index) const {
    assert(index < mVertices.size());
    return mVertices[index];
}


void HalfEdgeStructure::init() {

    std::map<VerticesPair, Edge> edges;
    std::map<VerticesPair, VerticesPair> nextEdges;
    std::map<VerticesPair, uint32> mapEdgeToStartVertex;
    std::map<VerticesPair, uint32> mapEdgeToIndex;
    std::map<uint32, VerticesPair> mapEdgeIndexToKey;
    std::map<uint32, VerticesPair> mapFaceIndexToEdgeKey;

    std::vector<VerticesPair> currentFaceEdges(mFaces[0].faceVertices.size());

    // For each face
    const uint32 nbFaces = static_cast<uint32>(mFaces.size());
    for (uint32 f = 0; f < nbFaces; f++) {

        Face &face = mFaces[f];

        VerticesPair firstEdgeKey(0, 0);

        // For each vertex of the face
        const uint32 nbFaceVertices = static_cast<uint32>(face.faceVertices.size());
        for (uint32 v = 0; v < nbFaceVertices; v++) {
            uint32 v1Index = face.faceVertices[v];
            uint32 v2Index = face.faceVertices[v == (face.faceVertices.size() - 1) ? 0 : v + 1];

            const VerticesPair pairV1V2 = VerticesPair(v1Index, v2Index);

            // Create a new half-edge
            Edge edge;
            edge.faceIndex = f;
            edge.vertexIndex = v1Index;
            if (v == 0) {
                firstEdgeKey = pairV1V2;
            } else if (v >= 1) {
                nextEdges.try_emplace(currentFaceEdges[currentFaceEdges.size() - 1], pairV1V2);
            }
            if (v == (face.faceVertices.size() - 1)) {
                nextEdges.try_emplace(pairV1V2, firstEdgeKey);
            }
            edges.try_emplace(pairV1V2, edge);

            const VerticesPair pairV2V1(v2Index, v1Index);

            mapEdgeToStartVertex.insert_or_assign(pairV1V2, v1Index);
            mapEdgeToStartVertex.insert_or_assign(pairV2V1, v2Index);
            mapFaceIndexToEdgeKey.insert_or_assign(f, pairV1V2);

            auto itEdge = edges.find(pairV2V1);
            if (itEdge != edges.end()) {

                const uint32 edgeIndex = static_cast<uint32>(mEdges.size());

                itEdge->second.twinEdgeIndex = edgeIndex + 1;
                edge.twinEdgeIndex = edgeIndex;

                mapEdgeIndexToKey.emplace(std::pair<uint32, VerticesPair>(edgeIndex, pairV2V1));
                mapEdgeIndexToKey.emplace(std::pair<uint32, VerticesPair>(edgeIndex + 1, pairV1V2));

                mVertices[v1Index].edgeIndex = edgeIndex + 1;
                mVertices[v2Index].edgeIndex = edgeIndex;

                mapEdgeToIndex.emplace(std::pair<VerticesPair, uint32>(pairV1V2, edgeIndex + 1));
                mapEdgeToIndex.emplace(std::pair<VerticesPair, uint32>(pairV2V1, edgeIndex));

                mEdges.emplace_back(itEdge->second);
                mEdges.emplace_back(edge);
            }

            currentFaceEdges.emplace_back(pairV1V2);
        }

        currentFaceEdges.clear();
    }

    // Set next edges
    const uint32 nbEdges = static_cast<uint32>(mEdges.size());
    for (uint32 i = 0; i < nbEdges; i++) {
        mEdges[i].nextEdgeIndex = mapEdgeToIndex[nextEdges[mapEdgeIndexToKey[i]]];
    }

    // Set face edge
    for (uint32 f = 0; f < nbFaces; f++) {
        mFaces[f].edgeIndex = mapEdgeToIndex[mapFaceIndexToEdgeKey[f]];
    }
}