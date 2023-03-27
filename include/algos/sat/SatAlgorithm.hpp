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

#ifndef REACTPHYSICS3D_SAT_ALGORITHM_H
#define REACTPHYSICS3D_SAT_ALGORITHM_H

// Libraries
#include <vector>
#include "algos/HalfEdgeStructure.hpp"
#include "glm/vec3.hpp"
#include "algos/algoShape.hpp"
#include "glm/mat4x4.hpp"
#include "algos/gjk/Contact.h"

/// ReactPhysics3D namespace
namespace reactphysics3d {


// Class SATAlgorithm
/**
 * This class implements the Separating Axis Theorem algorithm (SAT).
 * This algorithm is used to find the axis of minimum penetration between two convex polyhedra.
 * If none is found, the objects are separated. Otherwise, the two objects are
 * in contact and we use clipping to get the contact points.
 */
    class SATAlgorithm {

    private :

        // -------------------- Attributes -------------------- //

        /// Relative and absolute bias used to make sure the SAT algorithm returns the same penetration axis between frames
        /// when there are multiple separating axis with almost the same penetration depth. The goal is to
        /// make sure the contact manifold does not change too much between frames for better stability.
        static const float SEPARATING_AXIS_RELATIVE_TOLERANCE;
        static const float SEPARATING_AXIS_ABSOLUTE_TOLERANCE;

        /// True means that if two shapes were colliding last time (previous frame) and are still colliding
        /// we use the previous (minimum penetration depth) axis to clip the colliding features and we don't
        /// recompute a new (minimum penetration depth) axis. This value must be true for a dynamic simulation
        /// because it uses temporal coherence and clip the colliding features with the previous
        /// axis (this is good for stability). However, when we use the testCollision() methods, the penetration
        /// depths might be very large and we always want the current true axis with minimum penetration depth.
        /// In this case, this value must be set to false. Consider the following situation. Two shapes start overlaping
        /// with "x" being the axis of minimum penetration depth. Then, if the shapes move but are still penetrating,
        /// it is possible that the axis of minimum penetration depth changes for axis "y" for instance. If this value
        /// is true, we will always use the axis of the previous collision test and therefore always report that the
        /// penetrating axis is "x" even if it has changed to axis "y" during the collision. This is not what we want
        /// when we call the testCollision() methods.
        bool mClipWithPreviousAxisIfStillColliding;

        // -------------------- Methods -------------------- //

        /// Return true if two edges of two polyhedrons build a minkowski face (and can therefore be a separating axis)
        bool testEdgesBuildMinkowskiFace(const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                         const HalfEdgeStructure::Edge &edge1,
                                         const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                         const HalfEdgeStructure::Edge &edge2,
                                         const glm::mat4 &polyhedron1ToPolyhedron2) const;

        /// Return true if the arcs AB and CD on the Gauss Map intersect
        bool testGaussMapArcsIntersect(const glm::vec3& a, const glm::vec3& b,
                                       const glm::vec3& c, const glm::vec3& d,
                                       const glm::vec3& bCrossA, const glm::vec3& dCrossC) const;

        /// Compute and return the distance between the two edges in the direction of the candidate separating axis
        float computeDistanceBetweenEdges(const glm::vec3& edge1A, const glm::vec3& edge2A,
                                            const glm::vec3& polyhedron1Centroid, const glm::vec3& polyhedron2Centroid,
                                            const glm::vec3& edge1Direction, const glm::vec3& edge2Direction,
                                            bool isShape1Triangle, glm::vec3& outSeparatingAxis) const;

        /// Return the penetration depth between two polyhedra along a face normal axis of the first polyhedron
        float testSingleFaceDirectionPolyhedronVsPolyhedron(const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                                            const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                                            const glm::mat4 &polyhedron1ToPolyhedron2,
                                                            uint32 faceIndex) const;


        /// Test all the normals of a polyhedron for separating axis in the polyhedron vs polyhedron case
        float testFacesDirectionPolyhedronVsPolyhedron(const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                                       const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                                       const glm::mat4 &polyhedron1ToPolyhedron2,
                                                       uint32 &minFaceIndex) const;

        /// Compute the penetration depth between a face of the polyhedron and a sphere along the polyhedron face normal direction
        float computePolyhedronFaceVsSpherePenetrationDepth(uint32 faceIndex,
                                                            const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                            const algo::shapes::SphereShape *sphere,
                                                            const glm::vec3 &sphereCenter) const;

        /// Compute the penetration depth between the face of a polyhedron and a capsule along the polyhedron face normal direction
        float computePolyhedronFaceVsCapsulePenetrationDepth(uint32 polyhedronFaceIndex,
                                                             const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                             const algo::shapes::CapsuleShape *capsule,
                                                             const glm::mat4 &polyhedronToCapsuleTransform,
                                                             glm::vec3 &outFaceNormalCapsuleSpace) const;

        /// Compute the penetration depth when the separating axis is the cross product of polyhedron edge and capsule inner segment
        float computeEdgeVsCapsuleInnerSegmentPenetrationDepth(const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                               const algo::shapes::CapsuleShape *capsule,
                                                               const glm::vec3 &capsuleSegmentAxis,
                                                               const glm::vec3 &edgeVertex1,
                                                               const glm::vec3 &edgeDirectionCapsuleSpace,
                                                               const glm::mat4 &polyhedronToCapsuleTransform,
                                                               glm::vec3 &outAxis) const;

        /// Compute the contact points between two faces of two convex polyhedra.
        bool computePolyhedronVsPolyhedronFaceContactPoints(bool isMinPenetrationFaceNormalPolyhedron1,
                                                            const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                                            const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                                            const glm::mat4 &polyhedron1ToPolyhedron2,
                                                            const glm::mat4 &polyhedron2ToPolyhedron1,
                                                            const glm::mat4 &transA, const glm::mat4 &transB,
                                                            uint32 minFaceIndex, std::vector<Contact> &contacts) const;


    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        SATAlgorithm(bool clipWithPreviousAxisIfStillColliding);

        /// Destructor
        ~SATAlgorithm() = default;

        /// Deleted copy-constructor
        SATAlgorithm(const SATAlgorithm& algorithm) = delete;

        /// Deleted assignment operator
        SATAlgorithm& operator=(const SATAlgorithm& algorithm) = delete;

        /// Test collision between a sphere and a convex mesh
        bool testCollisionSphereVsConvexPolyhedron(algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                                   algo::shapes::Shape &shapeB,
                                                   const glm::mat4 &transB, std::vector<Contact> &contacts) const;

        /// Test collision between a capsule and a convex mesh
        bool testCollisionCapsuleVsConvexPolyhedron(algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                                    algo::shapes::Shape &shapeB, const glm::mat4 &transB,
                                                    std::vector<Contact> &contacts) const;

        /// Compute the two contact points between a polyhedron and a capsule when the separating axis is a face normal of the polyhedron
        bool computeCapsulePolyhedronFaceContactPoints(uint32_t referenceFaceIndex, float capsuleRadius,
                                                       const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                       float penetrationDepth,
                                                       const glm::mat4 &polyhedronToCapsuleTransform,
                                                       glm::vec3 &normalWorld,
                                                       const glm::vec3 &separatingAxisCapsuleSpace,
                                                       const glm::vec3 &capsuleSegAPolyhedronSpace,
                                                       const glm::vec3 &capsuleSegBPolyhedronSpace,
                                                       bool isCapsuleShape1,
                                                       std::vector<Contact> &contacts) const;

        // This method returns true if an edge of a polyhedron and a capsule forms a face of the Minkowski Difference
        bool isMinkowskiFaceCapsuleVsEdge(const glm::vec3& capsuleSegment, const glm::vec3& edgeAdjacentFace1Normal,
                                          const glm::vec3& edgeAdjacentFace2Normal) const;

        /// Test collision between two convex meshes
        bool testCollisionConvexPolyhedronVsConvexPolyhedron(algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                                             algo::shapes::Shape &shapeB, const glm::mat4 &transB,
                                                             std::vector<Contact> &contacts) const;


    };
}

#endif