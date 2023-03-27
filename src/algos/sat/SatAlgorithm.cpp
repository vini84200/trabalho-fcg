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
#include "algos/sat/SatAlgorithm.hpp"
#include "glm/fwd.hpp"
#include "utils/utils.hpp"
#include "glm/gtx/norm.hpp"
#include "utils/matrices.h"
#include "algos/gjk/Contact.h"
#include "spdlog/spdlog.h"
//#include <reactphysics3d/constraint/ContactPoint.h>
//#include <reactphysics3d/collision/PolyhedronMesh.h>
//#include <reactphysics3d/collision/shapes/CapsuleShape.h>
//#include <reactphysics3d/collision/shapes/SphereShape.h>
//#include <reactphysics3d/engine/OverlappingPairs.h>
//#include <reactphysics3d/collision/narrowphase/NarrowPhaseInfoBatch.h>
//#include <reactphysics3d/collision/shapes/TriangleShape.h>
//#include <reactphysics3d/configuration.h>
#include <cassert>
#include <vector>
#include <array>

// We want to use the ReactPhysics3D namespace
using namespace reactphysics3d;

float clamp(float value, float lowerLimit, float upperLimit) {
    assert(lowerLimit <= upperLimit);
    return std::min(std::max(value, lowerLimit), upperLimit);
}


// Compute and return a point on segment from "segPointA" and "segPointB" that is closest to point "pointC"
glm::vec3
computeClosestPointOnSegment(const glm::vec3 &segPointA, const glm::vec3 &segPointB, const glm::vec3 &pointC) {

    const glm::vec3 ab = segPointB - segPointA;

    float abLengthSquare = glm::length2(ab);

    // If the segment has almost zero length
    if (abLengthSquare < MACHINE_EPSILON) {

        // Return one end-point of the segment as the closest point
        return segPointA;
    }

    // Project point C onto "AB" line
    float t = glm::dot(pointC - segPointA, ab) / abLengthSquare;
    // If projected point onto the line is outside the segment, clamp it to the segment
    if (t < float(0.0)) t = float(0.0);
    if (t > float(1.0)) t = float(1.0);

    // Return the closest point on the segment
    return segPointA + t * ab;
}

// Compute the closest points between two segments
// This method uses the technique described in the book Real-Time
// collision detection by Christer Ericson.
void computeClosestPointBetweenTwoSegments(const glm::vec3 &seg1PointA, const glm::vec3 &seg1PointB,
                                           const glm::vec3 &seg2PointA, const glm::vec3 &seg2PointB,
                                           glm::vec3 &closestPointSeg1, glm::vec3 &closestPointSeg2) {

    const glm::vec3 d1 = seg1PointB - seg1PointA;
    const glm::vec3 d2 = seg2PointB - seg2PointA;
    const glm::vec3 r = seg1PointA - seg2PointA;
    float a = glm::length2(d1);
    float e = glm::length2(d2);
    float f = glm::dot(d2, r);
    float s, t;

    // If both segments degenerate into points
    if (a <= MACHINE_EPSILON && e <= MACHINE_EPSILON) {

        closestPointSeg1 = seg1PointA;
        closestPointSeg2 = seg2PointA;
        return;
    }
    if (a <= MACHINE_EPSILON) {   // If first segment degenerates into a point

        s = float(0.0);

        // Compute the closest point on second segment
        t = clamp(f / e, float(0.0), float(1.0));
    } else {

        float c = glm::dot(d1, r);

        // If the second segment degenerates into a point
        if (e <= MACHINE_EPSILON) {

            t = float(0.0);
            s = clamp(-c / a, float(0.0), float(1.0));
        } else {

            float b = glm::dot(d1, d2);
            float denom = a * e - b * b;

            // If the segments are not parallel
            if (denom != float(0.0)) {

                // Compute the closest point on line 1 to line 2 and
                // clamp to first segment.
                s = clamp((b * f - c * e) / denom, float(0.0), float(1.0));
            } else {

                // Pick an arbitrary point on first segment
                s = float(0.0);
            }

            // Compute the point on line 2 closest to the closest point
            // we have just found
            t = (b * s + f) / e;

            // If this closest point is inside second segment (t in [0, 1]), we are done.
            // Otherwise, we clamp the point to the second segment and compute again the
            // closest point on segment 1
            if (t < float(0.0)) {
                t = float(0.0);
                s = clamp(-c / a, float(0.0), float(1.0));
            } else if (t > float(1.0)) {
                t = float(1.0);
                s = clamp((b - c) / a, float(0.0), float(1.0));
            }
        }
    }

    // Compute the closest points on both segments
    closestPointSeg1 = seg1PointA + d1 * s;
    closestPointSeg2 = seg2PointA + d2 * t;
}

// Let the plane define by the equation planeNormal.dot(X) = planeD with X a point on the plane and "planeNormal" the plane normal. This method
// computes the intersection P between the plane and the segment (segA, segB). The method returns the value "t" such
// that P = segA + t * (segB - segA). Note that it only returns a value in [0, 1] if there is an intersection. Otherwise,
// there is no intersection between the plane and the segment.
float computePlaneSegmentIntersection(const glm::vec3 &segA, const glm::vec3 &segB, const float planeD,
                                      const glm::vec3 &planeNormal) {

    const float parallelEpsilon = 0.0001F;
    float t = -1.F;

    const float nDotAB = glm::dot(planeNormal, segB - segA);

    // If the segment is not parallel to the plane
    if (std::abs(nDotAB) > parallelEpsilon) {
        t = (planeD - glm::dot(planeNormal, segA)) / nDotAB;
    }

    return t;
}


// Clip a segment against multiple planes and return the clipped segment vertices
// This method implements the Sutherland–Hodgman clipping algorithm
std::vector<glm::vec3>
clipSegmentWithPlanes(const glm::vec3 &segA, const glm::vec3 &segB, const std::vector<glm::vec3> &planesPoints,
                      const std::vector<glm::vec3> &planesNormals) {
    assert(planesPoints.size() == planesNormals.size());

    std::vector<glm::vec3> inputVertices;
    inputVertices.reserve(2);
    std::vector<glm::vec3> outputVertices;
    outputVertices.reserve(2);

    inputVertices[0] = segA;
    inputVertices[1] = segB;
    // For each clipping plane
    const uint32 nbPlanesPoints = static_cast<uint32>(planesPoints.size());
    for (uint32 p = 0; p < nbPlanesPoints; p++) {

        // If there is no more vertices, stop
        if (inputVertices.size() == 0) return inputVertices;

        assert(inputVertices.size() == 2);

        outputVertices.clear();

        glm::vec3 &v1 = inputVertices[0];
        glm::vec3 &v2 = inputVertices[1];

        float v1DotN = glm::dot(v1 - planesPoints[p], planesNormals[p]);
        float v2DotN = glm::dot(v2 - planesPoints[p], planesNormals[p]);

        // If the second vertex is in front of the clippling plane
        if (v2DotN >= 0.0F) {

            // If the first vertex is not in front of the clippling plane
            if (v1DotN < 0.0F) {

                // The second point we keep is the intersection between the segment v1, v2 and the clipping plane
                float t = computePlaneSegmentIntersection(v1, v2, glm::dot(planesNormals[p], planesPoints[p]),
                                                          planesNormals[p]);

                if (t >= float(0) && t <= float(1.0)) {
                    outputVertices.push_back(v1 + t * (v2 - v1));
                } else {
                    outputVertices.push_back(v2);
                }
            } else {
                outputVertices.push_back(v1);
            }

            // Add the second vertex
            outputVertices.push_back(v2);
        } else {  // If the second vertex is behind the clipping plane

            // If the first vertex is in front of the clippling plane
            if (v1DotN >= float(0.0)) {

                outputVertices.push_back(v1);

                // The first point we keep is the intersection between the segment v1, v2 and the clipping plane
                float t = computePlaneSegmentIntersection(v1, v2, glm::dot(-planesNormals[p], planesPoints[p]),
                                                          -planesNormals[p]);

                if (t >= float(0.0) && t <= float(1.0)) {
                    outputVertices.push_back(v1 + t * (v2 - v1));
                }
            }
        }

        inputVertices = outputVertices;
    }

    return outputVertices;
}

// Clip a polygon against a single plane and return the clipped polygon vertices
// This method implements the Sutherland–Hodgman polygon clipping algorithm
void clipPolygonWithPlane(const std::vector<glm::vec3> &polygonVertices, const glm::vec3 &planePoint,
                          const glm::vec3 &planeNormal, std::vector<glm::vec3> &outClippedPolygonVertices) {

    uint32 nbInputVertices = static_cast<uint32>(polygonVertices.size());

    assert(outClippedPolygonVertices.size() == 0);

    uint32 vStartIndex = nbInputVertices - 1;

    const float planeNormalDotPlanePoint = glm::dot(planeNormal, planePoint);

    float vStartDotN = glm::dot(polygonVertices[vStartIndex] - planePoint, planeNormal);

    // For each edge of the polygon
    for (uint32 vEndIndex = 0; vEndIndex < nbInputVertices; vEndIndex++) {

        const glm::vec3 &vStart = polygonVertices[vStartIndex];
        const glm::vec3 &vEnd = polygonVertices[vEndIndex];

        const float vEndDotN = glm::dot(vEnd - planePoint, planeNormal);

        // If the second vertex is in front of the clippling plane
        if (vEndDotN >= float(0.0)) {

            // If the first vertex is not in front of the clippling plane
            if (vStartDotN < float(0.0)) {

                // The second point we keep is the intersection between the segment v1, v2 and the clipping plane
                const float t = computePlaneSegmentIntersection(vStart, vEnd, planeNormalDotPlanePoint, planeNormal);

                if (t >= float(0) && t <= float(1.0)) {
                    outClippedPolygonVertices.push_back(vStart + t * (vEnd - vStart));
                } else {
                    outClippedPolygonVertices.push_back(vEnd);
                }
            }

            // Add the second vertex
            outClippedPolygonVertices.push_back(vEnd);
        } else {  // If the second vertex is behind the clipping plane

            // If the first vertex is in front of the clippling plane
            if (vStartDotN >= float(0.0)) {

                // The first point we keep is the intersection between the segment v1, v2 and the clipping plane
                const float t = computePlaneSegmentIntersection(vStart, vEnd, -planeNormalDotPlanePoint, -planeNormal);

                if (t >= float(0.0) && t <= float(1.0)) {
                    outClippedPolygonVertices.push_back(vStart + t * (vEnd - vStart));
                } else {
                    outClippedPolygonVertices.push_back(vStart);
                }
            }
        }

        vStartIndex = vEndIndex;
        vStartDotN = vEndDotN;
    }
}

// Project a point onto a plane that is given by a point and its unit length normal
glm::vec3 projectPointOntoPlane(const glm::vec3 &point, const glm::vec3 &unitPlaneNormal, const glm::vec3 &planePoint) {
    return point - glm::dot(unitPlaneNormal, point - planePoint) * unitPlaneNormal;
}

// Return true if two vectors are parallel
bool areParallelVectors(const glm::vec3 &vector1, const glm::vec3 &vector2) {
    return glm::length2(glm::cross(vector1, vector2)) < float(0.00001);
}

// Static variables initialization
const float SATAlgorithm::SEPARATING_AXIS_RELATIVE_TOLERANCE = float(1.002);
const float SATAlgorithm::SEPARATING_AXIS_ABSOLUTE_TOLERANCE = float(0.0005);

// Constructor
SATAlgorithm::SATAlgorithm(bool clipWithPreviousAxisIfStillColliding)
        : mClipWithPreviousAxisIfStillColliding(clipWithPreviousAxisIfStillColliding) {

}

// Test collision between a sphere and a convex mesh
bool SATAlgorithm::testCollisionSphereVsConvexPolyhedron(algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                                         algo::shapes::Shape &shapeB, const glm::mat4 &transB,
                                                         std::vector<Contact> &contacts) const {

    bool isCollisionFound = false;

    bool const isSphereShape1 = shapeA.getType() == algo::shapes::Shape::ShapeType::SPHERE;

    assert(shapeA.getType() == algo::shapes::Shape::ShapeType::SPHERE ||
           shapeB.getType() == algo::shapes::Shape::ShapeType::SPHERE);
    assert(shapeA.getType() == algo::shapes::Shape::ShapeType::CONVEX_POLYHEDRON ||
           shapeB.getType() == algo::shapes::Shape::ShapeType::CONVEX_POLYHEDRON);

    // Get the capsule collision shapes
    const auto *sphere = dynamic_cast<const algo::shapes::SphereShape *>(isSphereShape1 ? &shapeA : &shapeB);
    const auto *polyhedron = dynamic_cast<const algo::shapes::ConvexPolyhedronShape *>(isSphereShape1 ? &shapeB
                                                                                                      : &shapeA);

    const glm::mat4 &sphereToWorldTransform = isSphereShape1 ? transA : transB;
    const glm::mat4 &polyhedronToWorldTransform = isSphereShape1 ? transB : transA;

    // Get the transform from sphere local-space to polyhedron local-space
    const glm::mat4 worldToPolyhedronTransform = glm::inverse(polyhedronToWorldTransform);
    const glm::mat4 sphereToPolyhedronSpaceTransform = worldToPolyhedronTransform * sphereToWorldTransform;

    // Transform the center of the sphere into the local-space of the convex polyhedron
    const glm::vec3 sphereCenter = matrices::PositionFromMatrix(sphereToPolyhedronSpaceTransform);

    // Minimum penetration depth
    float minPenetrationDepth = std::numeric_limits<float>::max();
    uint32 minFaceIndex = 0;
    bool noContact = false;

    // For each face of the convex mesh
    for (uint32 f = 0; f < polyhedron->getNbFaces(); f++) {

        // Compute the penetration depth of the shapes along the face normal direction
        float penetrationDepth = computePolyhedronFaceVsSpherePenetrationDepth(f, polyhedron, sphere, sphereCenter);

        // If the penetration depth is negative, we have found a separating axis
        if (penetrationDepth <= float(0.0)) {

            noContact = true;
            break;
        }

        // Check if we have found a new minimum penetration axis
        if (penetrationDepth < minPenetrationDepth) {
            minPenetrationDepth = penetrationDepth;
            minFaceIndex = f;
        }
    }

    if (noContact) {
        return isCollisionFound;
    }

    // If we need to report contacts
    if (true) {

        const glm::vec3 minFaceNormal = polyhedron->getFaceNormal(minFaceIndex);
        glm::vec3 minFaceNormalWorld = matrices::RotationFromMatrix(polyhedronToWorldTransform) * minFaceNormal;
//        glm::vec3 contactPointSphereLocal = sphereToWorldTransform.getInverse().getOrientation() * (-minFaceNormalWorld * sphere->getRadius());
        glm::vec3 contactPointSphereLocal = matrices::RotationFromMatrix(glm::inverse(sphereToWorldTransform)) *
                                            (-minFaceNormalWorld * sphere->getRadius());
        glm::vec3 contactPointPolyhedronLocal =
                sphereCenter + minFaceNormal * (minPenetrationDepth - sphere->getRadius());

        glm::vec3 normalWorld = isSphereShape1 ? -minFaceNormalWorld : minFaceNormalWorld;

        // Compute smooth triangle mesh contact if one of the two collision shapes is a triangle

        // Create the contact info object
//        narrowPhaseInfoBatch.addContactPoint(batchIndex, normalWorld, minPenetrationDepth,
//                                             isSphereShape1 ? contactPointSphereLocal : contactPointPolyhedronLocal,
//                                             isSphereShape1 ? contactPointPolyhedronLocal : contactPointSphereLocal);
        contacts.emplace_back(normalWorld, minPenetrationDepth,
                              isSphereShape1 ? contactPointSphereLocal : contactPointPolyhedronLocal,
                              isSphereShape1 ? contactPointPolyhedronLocal : contactPointSphereLocal);
    }

//    narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].isColliding = true;
    isCollisionFound = true;

    return isCollisionFound;
}

// Compute the penetration depth between a face of the polyhedron and a sphere along the polyhedron face normal direction
float SATAlgorithm::computePolyhedronFaceVsSpherePenetrationDepth(uint32 faceIndex,
                                                                  const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                                  const algo::shapes::SphereShape *sphere,
                                                                  const glm::vec3 &sphereCenter) const {

    // Get the face
    const HalfEdgeStructure::Face &face = polyhedron->getFace(faceIndex);

    // Get the face normal
    const glm::vec3 faceNormal = polyhedron->getFaceNormal(faceIndex);

    glm::vec3 sphereCenterToFacePoint = polyhedron->getVertexPosition(face.faceVertices[0]) - sphereCenter;
    float penetrationDepth = glm::dot(sphereCenterToFacePoint, faceNormal) + sphere->getRadius();

    return penetrationDepth;
}

// Test collision between a capsule and a convex mesh
bool SATAlgorithm::testCollisionCapsuleVsConvexPolyhedron(algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                                          algo::shapes::Shape &shapeB, const glm::mat4 &transB,
                                                          std::vector<Contact> &contacts) const {

    bool isCapsuleShape1 = shapeA.getType() == algo::shapes::Shape::ShapeType::CAPSULE;

    assert(shapeA.getType() == algo::shapes::Shape::ShapeType::CAPSULE ||
           shapeB.getType() == algo::shapes::Shape::ShapeType::CAPSULE);
    assert(shapeA.getType() == algo::shapes::Shape::ShapeType::CONVEX_POLYHEDRON ||
           shapeB.getType() == algo::shapes::Shape::ShapeType::CONVEX_POLYHEDRON);

    // Get the collision shapes
    auto capsuleShape = static_cast<const algo::shapes::CapsuleShape *>(isCapsuleShape1 ? &shapeA : &shapeB);
    const algo::shapes::ConvexPolyhedronShape *polyhedron = static_cast<const algo::shapes::ConvexPolyhedronShape *>(isCapsuleShape1
                                                                                                                     ? &shapeB
                                                                                                                     : &shapeA);

    const glm::mat4 capsuleToWorld = isCapsuleShape1 ? transA : transB;
    const glm::mat4 polyhedronToWorld = isCapsuleShape1 ? transB : transA;

    const glm::mat4 polyhedronToCapsuleTransform = glm::inverse(capsuleToWorld) * polyhedronToWorld;


    // Compute the end-points of the inner segment of the capsule
    const glm::vec3 capsuleSegA(0, -capsuleShape->getHeight() * float(0.5), 0);
    const glm::vec3 capsuleSegB(0, capsuleShape->getHeight() * float(0.5), 0);
    const glm::vec3 capsuleSegmentAxis = capsuleSegB - capsuleSegA;

    // Minimum penetration depth
    float minPenetrationDepth = std::numeric_limits<float>::max();
    uint32 minFaceIndex = 0;
    bool isMinPenetrationFaceNormal = false;
    glm::vec3 separatingAxisCapsuleSpace;
    glm::vec3 separatingPolyhedronEdgeVertex1;
    glm::vec3 separatingPolyhedronEdgeVertex2;

    // For each face of the convex mesh
    for (uint32 f = 0; f < polyhedron->getNbFaces(); f++) {

        glm::vec3 outFaceNormalCapsuleSpace;

        // Compute the penetration depth
        const float penetrationDepth = computePolyhedronFaceVsCapsulePenetrationDepth(f, polyhedron, capsuleShape,
                                                                                      polyhedronToCapsuleTransform,
                                                                                      outFaceNormalCapsuleSpace);

        // If the penetration depth is negative, we have found a separating axis
        if (penetrationDepth <= float(0.0)) {

            return false;
        }

        // Check if we have found a new minimum penetration axis
        if (penetrationDepth < minPenetrationDepth) {
            minPenetrationDepth = penetrationDepth;
            minFaceIndex = f;
            isMinPenetrationFaceNormal = true;
            separatingAxisCapsuleSpace = outFaceNormalCapsuleSpace;
        }
    }

    // For each direction that is the cross product of the capsule inner segment and an edge of the polyhedron
    for (uint32 e = 0; e < polyhedron->getNbHalfEdges(); e += 2) {

        // Get an edge from the polyhedron (convert it into the capsule local-space)
        const HalfEdgeStructure::Edge &edge = polyhedron->getHalfEdge(e);
        const glm::vec3 edgeVertex1 = polyhedron->getVertexPosition(edge.vertexIndex);
        const glm::vec3 edgeVertex2 = polyhedron->getVertexPosition(
                polyhedron->getHalfEdge(edge.nextEdgeIndex).vertexIndex);
        const glm::vec3 edgeDirectionCapsuleSpace =
                matrices::RotationFromMatrix(polyhedronToCapsuleTransform) * (edgeVertex2 - edgeVertex1);

        const HalfEdgeStructure::Edge &twinEdge = polyhedron->getHalfEdge(edge.twinEdgeIndex);
        const glm::vec3 adjacentFace1Normal =
                matrices::RotationFromMatrix(polyhedronToCapsuleTransform) * polyhedron->getFaceNormal(edge.faceIndex);
        const glm::vec3 adjacentFace2Normal = matrices::RotationFromMatrix(polyhedronToCapsuleTransform) *
                                              polyhedron->getFaceNormal(twinEdge.faceIndex);

        // Check using the Gauss Map if this edge cross product can be as separating axis
        if (isMinkowskiFaceCapsuleVsEdge(capsuleSegmentAxis, adjacentFace1Normal, adjacentFace2Normal)) {

            glm::vec3 outAxis;

            // Compute the penetration depth
            const float penetrationDepth = computeEdgeVsCapsuleInnerSegmentPenetrationDepth(polyhedron, capsuleShape,
                                                                                            capsuleSegmentAxis,
                                                                                            edgeVertex1,
                                                                                            edgeDirectionCapsuleSpace,
                                                                                            polyhedronToCapsuleTransform,
                                                                                            outAxis);

            // If the penetration depth is negative, we have found a separating axis
            if (penetrationDepth <= float(0.0)) {

                return false;
            }

            // Check if we have found a new minimum penetration axis
            if (penetrationDepth < minPenetrationDepth) {
                minPenetrationDepth = penetrationDepth;
                isMinPenetrationFaceNormal = false;
                separatingAxisCapsuleSpace = outAxis;
                separatingPolyhedronEdgeVertex1 = edgeVertex1;
                separatingPolyhedronEdgeVertex2 = edgeVertex2;
            }
        }
    }

    // Convert the inner capsule segment points into the polyhedron local-space
    const glm::mat4 capsuleToPolyhedronTransform = glm::inverse(polyhedronToCapsuleTransform);
    const glm::vec3 capsuleSegAPolyhedronSpace = capsuleToPolyhedronTransform * glm::vec4(capsuleSegA, 1.0f);
    const glm::vec3 capsuleSegBPolyhedronSpace = capsuleToPolyhedronTransform * glm::vec4(capsuleSegB, 1.0f);

    glm::vec3 normalWorld = matrices::RotationFromMatrix(capsuleToWorld) * separatingAxisCapsuleSpace;
    if (isCapsuleShape1) {
        normalWorld = -normalWorld;
    }
    const float capsuleRadius = capsuleShape->getRadius();

    // If the separating axis is a face normal
    // We need to clip the inner capsule segment with the adjacent faces of the separating face
    if (isMinPenetrationFaceNormal) {

        // If we need to report contacts
        if (true) {

            return computeCapsulePolyhedronFaceContactPoints(minFaceIndex, capsuleRadius, polyhedron,
                                                             minPenetrationDepth, polyhedronToCapsuleTransform,
                                                             normalWorld, separatingAxisCapsuleSpace,
                                                             capsuleSegAPolyhedronSpace, capsuleSegBPolyhedronSpace,
                                                             isCapsuleShape1, contacts);
        }
    } else {   // The separating axis is the cross product of a polyhedron edge and the inner capsule segment

        // If we need to report contacts
        if (true) {

            // Compute the closest points between the inner capsule segment and the
            // edge of the polyhedron in polyhedron local-space
            glm::vec3 closestPointPolyhedronEdge, closestPointCapsuleInnerSegment;
            computeClosestPointBetweenTwoSegments(capsuleSegAPolyhedronSpace, capsuleSegBPolyhedronSpace,
                                                  separatingPolyhedronEdgeVertex1, separatingPolyhedronEdgeVertex2,
                                                  closestPointCapsuleInnerSegment, closestPointPolyhedronEdge);

            // Project closest capsule inner segment point into the capsule bounds
            glm::vec3 contactPointCapsule =
                    glm::vec3(polyhedronToCapsuleTransform * glm::vec4(closestPointCapsuleInnerSegment, 1)) -
                    separatingAxisCapsuleSpace * capsuleRadius;

            // Compute smooth triangle mesh contact if one of the two collision shapes is a triangle
//            TriangleShape::computeSmoothTriangleMeshContact(narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape1, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape2,
//                                                            isCapsuleShape1 ? contactPointCapsule : closestPointPolyhedronEdge,
//                                                            isCapsuleShape1 ? closestPointPolyhedronEdge : contactPointCapsule,
//                                                            narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape1ToWorldTransform, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape2ToWorldTransform,
//                                                            minPenetrationDepth, normalWorld);

            // Create the contact point
//            narrowPhaseInfoBatch.addContactPoint(batchIndex, normalWorld, minPenetrationDepth,
//                                                 isCapsuleShape1 ? contactPointCapsule : closestPointPolyhedronEdge,
//                                                 isCapsuleShape1 ? closestPointPolyhedronEdge : contactPointCapsule);
            contacts.emplace_back(normalWorld, minPenetrationDepth,
                                  isCapsuleShape1 ? contactPointCapsule : closestPointPolyhedronEdge,
                                  isCapsuleShape1 ? closestPointPolyhedronEdge : contactPointCapsule);
        }
    }

    return true;
}

// Compute the penetration depth when the separating axis is the cross product of polyhedron edge and capsule inner segment
float
SATAlgorithm::computeEdgeVsCapsuleInnerSegmentPenetrationDepth(const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                               const algo::shapes::CapsuleShape *capsule,
                                                               const glm::vec3 &capsuleSegmentAxis,
                                                               const glm::vec3 &edgeVertex1,
                                                               const glm::vec3 &edgeDirectionCapsuleSpace,
                                                               const glm::mat4 &polyhedronToCapsuleTransform,
                                                               glm::vec3 &outAxis) const {

    float penetrationDepth = std::numeric_limits<float>::max();

    // Compute the axis to test (cross product between capsule inner segment and polyhedron edge)
    outAxis = glm::cross(capsuleSegmentAxis, edgeDirectionCapsuleSpace);

    // Skip separating axis test if polyhedron edge is parallel to the capsule inner segment
    if (glm::length2(outAxis) >= float(0.00001)) {

        const glm::vec3 polyhedronCentroid = polyhedronToCapsuleTransform * glm::vec4(polyhedron->getCentroid(), 1);
        const glm::vec3 pointOnPolyhedronEdge = polyhedronToCapsuleTransform * glm::vec4(edgeVertex1,
                                                                                         1);//TODO: is this really a point and not a direction?

        // Swap axis direction if necessary such that it points out of the polyhedron
        if (glm::dot(outAxis, pointOnPolyhedronEdge - polyhedronCentroid) < 0) {
            outAxis = -outAxis;
        }

        outAxis = glm::normalize(outAxis);

        // Compute the penetration depth
        const glm::vec3 capsuleSupportPoint = capsule->getLocalSupportPointWithMargin(-outAxis);
        const glm::vec3 capsuleSupportPointToEdgePoint = pointOnPolyhedronEdge - capsuleSupportPoint;
        penetrationDepth = glm::dot(capsuleSupportPointToEdgePoint, outAxis);
    }

    return penetrationDepth;
}

// Compute the penetration depth between the face of a polyhedron and a capsule along the polyhedron face normal direction
float SATAlgorithm::computePolyhedronFaceVsCapsulePenetrationDepth(uint32 polyhedronFaceIndex,
                                                                   const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                                   const algo::shapes::CapsuleShape *capsule,
                                                                   const glm::mat4 &polyhedronToCapsuleTransform,
                                                                   glm::vec3 &outFaceNormalCapsuleSpace) const {

    // Get the face
    const HalfEdgeStructure::Face &face = polyhedron->getFace(polyhedronFaceIndex);

    // Get the face normal
    const glm::vec3 faceNormal = polyhedron->getFaceNormal(polyhedronFaceIndex);

    // Compute the penetration depth (using the capsule support in the direction opposite to the face normal)
    outFaceNormalCapsuleSpace = matrices::RotationFromMatrix(polyhedronToCapsuleTransform) * faceNormal;
    const glm::vec3 capsuleSupportPoint = capsule->getLocalSupportPointWithMargin(-outFaceNormalCapsuleSpace);
    const glm::vec3 pointOnPolyhedronFace =
            polyhedronToCapsuleTransform * glm::vec4(polyhedron->getVertexPosition(face.faceVertices[0]), 1);
    const glm::vec3 capsuleSupportPointToFacePoint = pointOnPolyhedronFace - capsuleSupportPoint;
    const float penetrationDepth = glm::dot(capsuleSupportPointToFacePoint, outFaceNormalCapsuleSpace);

    return penetrationDepth;
}

// Compute the two contact points between a polyhedron and a capsule when the separating
// axis is a face normal of the polyhedron
bool SATAlgorithm::computeCapsulePolyhedronFaceContactPoints(uint32_t referenceFaceIndex, float capsuleRadius,
                                                             const algo::shapes::ConvexPolyhedronShape *polyhedron,
                                                             float penetrationDepth,
                                                             const glm::mat4 &polyhedronToCapsuleTransform,
                                                             glm::vec3 &normalWorld,
                                                             const glm::vec3 &separatingAxisCapsuleSpace,
                                                             const glm::vec3 &capsuleSegAPolyhedronSpace,
                                                             const glm::vec3 &capsuleSegBPolyhedronSpace,
                                                             bool isCapsuleShape1,
                                                             std::vector<Contact> &contacts) const {

    const HalfEdgeStructure::Face &face = polyhedron->getFace(referenceFaceIndex);

    // Get the face normal
    glm::vec3 faceNormal = polyhedron->getFaceNormal(referenceFaceIndex);

    uint32 firstEdgeIndex = face.edgeIndex;
    uint32 edgeIndex = firstEdgeIndex;

    std::vector<glm::vec3> planesPoints;
    std::vector<glm::vec3> planesNormals;
    planesPoints.reserve(2);
    planesNormals.reserve(2);

    // For each adjacent edge of the separating face of the polyhedron
    do {

        const HalfEdgeStructure::Edge &edge = polyhedron->getHalfEdge(edgeIndex);
        const HalfEdgeStructure::Edge &twinEdge = polyhedron->getHalfEdge(edge.twinEdgeIndex);

        // Compute the edge vertices and edge direction
        glm::vec3 edgeV1 = polyhedron->getVertexPosition(edge.vertexIndex);
        glm::vec3 edgeV2 = polyhedron->getVertexPosition(twinEdge.vertexIndex);
        glm::vec3 edgeDirection = edgeV2 - edgeV1;

        // Compute the normal of the clipping plane for this edge
        // The clipping plane is perpendicular to the edge direction and the reference face normal
        glm::vec3 clipPlaneNormal = glm::cross(faceNormal, edgeDirection);

        // Construct a clipping plane for each adjacent edge of the separating face of the polyhedron
        planesPoints.emplace_back(polyhedron->getVertexPosition(edge.vertexIndex));
        planesNormals.emplace_back(clipPlaneNormal);

        edgeIndex = edge.nextEdgeIndex;

    } while (edgeIndex != firstEdgeIndex);

    // First we clip the inner segment of the capsule with the four planes of the adjacent faces
    std::vector<glm::vec3> clipSegment = clipSegmentWithPlanes(capsuleSegAPolyhedronSpace, capsuleSegBPolyhedronSpace,
                                                               planesPoints, planesNormals);

    // Project the two clipped points into the polyhedron face
    const glm::vec3 delta = faceNormal * (penetrationDepth - capsuleRadius);

    bool contactFound = false;

    // For each of the two clipped points
    const uint32 nbClipSegments = static_cast<uint32>(clipSegment.size());
    for (uint32 i = 0; i < nbClipSegments; i++) {

        // Compute the penetration depth of the two clipped points (to filter out the points that does not correspond to the penetration depth)
        const float clipPointPenDepth = glm::dot(planesPoints[0] - clipSegment[i], faceNormal);

        // If the clipped point is one that produce this penetration depth, we keep it
        if (clipPointPenDepth > penetrationDepth - capsuleRadius - float(0.001)) {

            contactFound = true;

            glm::vec3 contactPointPolyhedron = clipSegment[i] + delta;

            // Project the clipped point into the capsule bounds
            glm::vec3 contactPointCapsule = glm::vec3(polyhedronToCapsuleTransform * glm::vec4(clipSegment[i], 1)) -
                                            separatingAxisCapsuleSpace * capsuleRadius;



            // Compute smooth triangle mesh contact if one of the two collision shapes is a triangle
//            TriangleShape::computeSmoothTriangleMeshContact(narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape1, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape2,
//                                                            isCapsuleShape1 ? contactPointCapsule : contactPointPolyhedron,
//                                                            isCapsuleShape1 ? contactPointPolyhedron : contactPointCapsule,
//                                                            narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape1ToWorldTransform, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape2ToWorldTransform,
//                                                            penetrationDepth, normalWorld);


            // Create the contact point
//            narrowPhaseInfoBatch.addContactPoint(batchIndex, normalWorld, penetrationDepth,
//                                                 isCapsuleShape1 ? contactPointCapsule : contactPointPolyhedron,
//                                                 isCapsuleShape1 ? contactPointPolyhedron : contactPointCapsule);
            contacts.emplace_back(normalWorld, penetrationDepth,
                                  isCapsuleShape1 ? contactPointCapsule : contactPointPolyhedron,
                                  isCapsuleShape1 ? contactPointPolyhedron : contactPointCapsule);

        }
    }

    return contactFound;
}

// This method returns true if an edge of a polyhedron and a capsule forms a
// face of the Minkowski Difference. This test is used to know if two edges
// (one edge of the polyhedron vs the inner segment of the capsule in this case)
// have to be test as a possible separating axis
bool
SATAlgorithm::isMinkowskiFaceCapsuleVsEdge(const glm::vec3 &capsuleSegment, const glm::vec3 &edgeAdjacentFace1Normal,
                                           const glm::vec3 &edgeAdjacentFace2Normal) const {

    // Return true if the arc on the Gauss Map corresponding to the polyhedron edge
    // intersect the unit circle plane corresponding to capsule Gauss Map
    return glm::dot(capsuleSegment, edgeAdjacentFace1Normal) * glm::dot(capsuleSegment, edgeAdjacentFace2Normal) <
           float(0.0);
}

// Test collision between two convex polyhedrons
bool SATAlgorithm::testCollisionConvexPolyhedronVsConvexPolyhedron(algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                                                   algo::shapes::Shape &shapeB, const glm::mat4 &transB,
                                                                   std::vector<Contact> &contacts) const {

    bool isCollisionFound = false;

    const algo::shapes::ConvexPolyhedronShape *polyhedron1 = static_cast<const algo::shapes::ConvexPolyhedronShape *>(&shapeA);
    const algo::shapes::ConvexPolyhedronShape *polyhedron2 = static_cast<const algo::shapes::ConvexPolyhedronShape *>(&shapeB);

    const glm::mat4 polyhedron1ToPolyhedron2 = glm::inverse(transB) * transA;
    const glm::mat4 polyhedron2ToPolyhedron1 = glm::inverse(polyhedron1ToPolyhedron2);

    float minPenetrationDepth = std::numeric_limits<float>::max();
    uint32 minFaceIndex = 0;
    bool isMinPenetrationFaceNormal = false;
    bool isMinPenetrationFaceNormalPolyhedron1 = false;
    uint32 minSeparatingEdge1Index = 0;
    uint32 minSeparatingEdge2Index = 0;
    glm::vec3 separatingEdge1A, separatingEdge1B;
    glm::vec3 separatingEdge2A, separatingEdge2B;
    glm::vec3 minEdgeVsEdgeSeparatingAxisPolyhedron2Space;
    const bool isShape1Triangle = false;

    minPenetrationDepth = std::numeric_limits<float>::max();
    isMinPenetrationFaceNormal = false;

    // Test all the face normals of the polyhedron 1 for separating axis
    uint32 faceIndex1;
    float penetrationDepth1 = testFacesDirectionPolyhedronVsPolyhedron(polyhedron1, polyhedron2,
                                                                       polyhedron1ToPolyhedron2, faceIndex1);
    if (penetrationDepth1 <= float(0.0)) {

//            lastFrameCollisionInfo->satIsAxisFacePolyhedron1 = true;
//            lastFrameCollisionInfo->satIsAxisFacePolyhedron2 = false;
//            lastFrameCollisionInfo->satMinAxisFaceIndex = faceIndex1;

        // We have found a separating axis
        return false;
    }

    // Test all the face normals of the polyhedron 2 for separating axis
    uint32 faceIndex2;
    float penetrationDepth2 = testFacesDirectionPolyhedronVsPolyhedron(polyhedron2, polyhedron1,
                                                                       polyhedron2ToPolyhedron1, faceIndex2);
    if (penetrationDepth2 <= float(0.0)) {

//            lastFrameCollisionInfo->satIsAxisFacePolyhedron1 = false;
//            lastFrameCollisionInfo->satIsAxisFacePolyhedron2 = true;
//            lastFrameCollisionInfo->satMinAxisFaceIndex = faceIndex2;

        // We have found a separating axis
        return false;
    }

    // Here we know that we have found penetration along both axis of a face of polyhedron1 and a face of
    // polyhedron2. If the two penetration depths are almost the same, we need to make sure we always prefer
    // one axis to the other for consistency between frames. This is to prevent the contact manifolds to switch
    // from one reference axis to the other for a face to face resting contact for instance. This is better for
    // stability. To do this, we use a relative and absolute bias to move penetrationDepth2 a little bit to the right.
    // Now if:
    //  penetrationDepth1 < penetrationDepth2: Nothing happens and we use axis of polygon 1
    //  penetrationDepth1 ~ penetrationDepth2: Until penetrationDepth1 becomes significantly less than penetrationDepth2 we still use axis of polygon 1
    //  penetrationDepth1 >> penetrationDepth2: penetrationDepth2 is now significantly less than penetrationDepth1 and we use polygon 2 axis
    if (penetrationDepth1 <
        penetrationDepth2 * SEPARATING_AXIS_RELATIVE_TOLERANCE + SEPARATING_AXIS_ABSOLUTE_TOLERANCE) {

        // We use penetration axis of polygon 1
        isMinPenetrationFaceNormal = true;
        minPenetrationDepth = std::min(penetrationDepth1, penetrationDepth2);
        minFaceIndex = faceIndex1;
        isMinPenetrationFaceNormalPolyhedron1 = true;
    } else {

        // We use penetration axis of polygon 2
        isMinPenetrationFaceNormal = true;
        minPenetrationDepth = std::min(penetrationDepth1, penetrationDepth2);
        minFaceIndex = faceIndex2;
        isMinPenetrationFaceNormalPolyhedron1 = false;
    }

    bool separatingAxisFound = false;

    // Test the cross products of edges of polyhedron 1 with edges of polyhedron 2 for separating axis
    for (uint32 i = 0; i < polyhedron1->getNbHalfEdges(); i += 2) {

        // Get an edge of polyhedron 1
        const HalfEdgeStructure::Edge &edge1 = polyhedron1->getHalfEdge(i);

        const glm::vec3 edge1A =
                polyhedron1ToPolyhedron2
                * glm::vec4(polyhedron1->getVertexPosition(edge1.vertexIndex), 1);
        const glm::vec3 edge1B = polyhedron1ToPolyhedron2 * glm::vec4(
                polyhedron1->getVertexPosition(
                        polyhedron1->getHalfEdge(edge1.nextEdgeIndex).vertexIndex), 1);
        const glm::vec3 edge1Direction = edge1B - edge1A;

        for (uint32 j = 0; j < polyhedron2->getNbHalfEdges(); j += 2) {

            // Get an edge of polyhedron 2
            const HalfEdgeStructure::Edge &edge2 = polyhedron2->getHalfEdge(j);

            const glm::vec3 edge2A = polyhedron2->getVertexPosition(edge2.vertexIndex);
            const glm::vec3 edge2B = polyhedron2->getVertexPosition(
                    polyhedron2->getHalfEdge(edge2.nextEdgeIndex).vertexIndex);
            const glm::vec3 edge2Direction = edge2B - edge2A;

            // If the two edges build a minkowski face (and the cross product is
            // therefore a candidate for separating axis
            if (testEdgesBuildMinkowskiFace(polyhedron1, edge1, polyhedron2, edge2, polyhedron1ToPolyhedron2)) {

                glm::vec3 separatingAxisPolyhedron2Space;

                // Compute the penetration depth
                const glm::vec3 polyhedron1Centroid =
                        polyhedron1ToPolyhedron2 * glm::vec4(polyhedron1->getCentroid(), 1);
                float penetrationDepth = computeDistanceBetweenEdges(edge1A, edge2A, polyhedron1Centroid,
                                                                     polyhedron2->getCentroid(), edge1Direction,
                                                                     edge2Direction, isShape1Triangle,
                                                                     separatingAxisPolyhedron2Space);

                if (penetrationDepth <= float(0.0)) {

//                        lastFrameCollisionInfo->satIsAxisFacePolyhedron1 = false;
//                        lastFrameCollisionInfo->satIsAxisFacePolyhedron2 = false;
//                        lastFrameCollisionInfo->satMinEdge1Index = i;
//                        lastFrameCollisionInfo->satMinEdge2Index = j;

                    // We have found a separating axis
                    separatingAxisFound = true;
                    break;
                }

                // If the current minimum penetration depth is along a face normal axis (isMinPenetrationFaceNormal=true) and we have found a new
                // smaller pentration depth along an edge-edge cross-product axis we want to favor the face normal axis because contact manifolds between
                // faces have more contact points and therefore more stable than the single contact point of an edge-edge collision. It means that if the new minimum
                // penetration depth from the edge-edge contact is only a little bit smaller than the current minPenetrationDepth (from a face contact), we favor
                // the face contact and do not generate an edge-edge contact. However, if the new penetration depth from the edge-edge contact is really smaller than
                // the current one, we generate an edge-edge contact.
                // To do this, we use a relative and absolute bias to increase a little bit the new penetration depth from the edge-edge contact during the comparison test
                if ((isMinPenetrationFaceNormal &&
                     penetrationDepth1 * SEPARATING_AXIS_RELATIVE_TOLERANCE + SEPARATING_AXIS_ABSOLUTE_TOLERANCE <
                     minPenetrationDepth) || (!isMinPenetrationFaceNormal && penetrationDepth < minPenetrationDepth)) {

                    minPenetrationDepth = penetrationDepth;
                    isMinPenetrationFaceNormalPolyhedron1 = false;
                    isMinPenetrationFaceNormal = false;
                    minSeparatingEdge1Index = i;
                    minSeparatingEdge2Index = j;
                    separatingEdge1A = edge1A;
                    separatingEdge1B = edge1B;
                    separatingEdge2A = edge2A;
                    separatingEdge2B = edge2B;
                    minEdgeVsEdgeSeparatingAxisPolyhedron2Space = separatingAxisPolyhedron2Space;
                }
            }
        }

        if (separatingAxisFound) {
            break;
        }
    }

    if (separatingAxisFound) {
        return false;
    }

    // Here we know the shapes are overlapping on a given minimum separating axis.
    // Now, we will clip the shapes along this axis to find the contact points

    assert(minPenetrationDepth > float(0.0));

    // If the minimum separating axis is a face normal
    if (isMinPenetrationFaceNormal) {

        // Compute the contact points between two faces of two convex polyhedra.
        bool contactsFound = computePolyhedronVsPolyhedronFaceContactPoints(isMinPenetrationFaceNormalPolyhedron1,
                                                                            polyhedron1, polyhedron2,
                                                                            polyhedron1ToPolyhedron2,
                                                                            polyhedron2ToPolyhedron1, transA, transB,
                                                                            minFaceIndex, contacts);

        // There should be clipping points here. If it is not the case, it might be
        // because of a numerical issue
        if (!contactsFound) {
            spdlog::warn("No contact points found between two polyhedra even though the shapes are overlapping");

//                lastFrameCollisionInfo->satIsAxisFacePolyhedron1 = isMinPenetrationFaceNormalPolyhedron1;
//                lastFrameCollisionInfo->satIsAxisFacePolyhedron2 = !isMinPenetrationFaceNormalPolyhedron1;
//                lastFrameCollisionInfo->satMinAxisFaceIndex = minFaceIndex;

            // Return no collision
            return false;
        }

//            lastFrameCollisionInfo->satIsAxisFacePolyhedron1 = isMinPenetrationFaceNormalPolyhedron1;
//            lastFrameCollisionInfo->satIsAxisFacePolyhedron2 = !isMinPenetrationFaceNormalPolyhedron1;
//            lastFrameCollisionInfo->satMinAxisFaceIndex = minFaceIndex;
    } else {    // If we have an edge vs edge contact

        // If we need to report contacts
        if (true) {

            // Compute the closest points between the two edges (in the local-space of poylhedron 2)
            glm::vec3 closestPointPolyhedron1Edge, closestPointPolyhedron2Edge;
            computeClosestPointBetweenTwoSegments(separatingEdge1A, separatingEdge1B, separatingEdge2A,
                                                  separatingEdge2B, closestPointPolyhedron1Edge,
                                                  closestPointPolyhedron2Edge);

            // Compute the contact point on polyhedron 1 edge in the local-space of polyhedron 1
            glm::vec3 closestPointPolyhedron1EdgeLocalSpace =
                    polyhedron2ToPolyhedron1 * glm::vec4(closestPointPolyhedron1Edge, 1.0f);

            // Compute the world normal
            glm::vec3 normalWorld = matrices::RotationFromMatrix(transB) * minEdgeVsEdgeSeparatingAxisPolyhedron2Space;

            // Compute smooth triangle mesh contact if one of the two collision shapes is a triangle
//                TriangleShape::computeSmoothTriangleMeshContact(narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape1, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape2,
//                                                                closestPointPolyhedron1EdgeLocalSpace, closestPointPolyhedron2Edge,
//                                                                narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape1ToWorldTransform, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape2ToWorldTransform,
//                                                                minPenetrationDepth, normalWorld);

            // Create the contact point
//                narrowPhaseInfoBatch.addContactPoint(batchIndex, normalWorld, minPenetrationDepth,
//                                                     closestPointPolyhedron1EdgeLocalSpace, closestPointPolyhedron2Edge);
            contacts.emplace_back(normalWorld, minPenetrationDepth, closestPointPolyhedron1EdgeLocalSpace,
                                  closestPointPolyhedron2Edge);
        }

//            lastFrameCollisionInfo->satIsAxisFacePolyhedron1 = false;
//            lastFrameCollisionInfo->satIsAxisFacePolyhedron2 = false;
//            lastFrameCollisionInfo->satMinEdge1Index = minSeparatingEdge1Index;
//            lastFrameCollisionInfo->satMinEdge2Index = minSeparatingEdge2Index;
    }

//        narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].isColliding = true;
    isCollisionFound = true;

    return isCollisionFound;
}

// Compute the contact points between two faces of two convex polyhedra.
/// The method returns true if contact points have been found
bool SATAlgorithm::computePolyhedronVsPolyhedronFaceContactPoints(bool isMinPenetrationFaceNormalPolyhedron1,
                                                                  const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                                                  const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                                                  const glm::mat4 &polyhedron1ToPolyhedron2,
                                                                  const glm::mat4 &polyhedron2ToPolyhedron1,
                                                                  const glm::mat4 &transA, const glm::mat4 &transB,
                                                                  uint32 minFaceIndex,
                                                                  std::vector<Contact> &contacts) const {

    const algo::shapes::ConvexPolyhedronShape *referencePolyhedron;
    const algo::shapes::ConvexPolyhedronShape *incidentPolyhedron;
    const glm::mat4 &referenceToIncidentTransform = isMinPenetrationFaceNormalPolyhedron1 ? polyhedron1ToPolyhedron2
                                                                                          : polyhedron2ToPolyhedron1;
    const glm::mat4 &incidentToReferenceTransform = isMinPenetrationFaceNormalPolyhedron1 ? polyhedron2ToPolyhedron1
                                                                                          : polyhedron1ToPolyhedron2;

    if (isMinPenetrationFaceNormalPolyhedron1) {
        referencePolyhedron = polyhedron1;
        incidentPolyhedron = polyhedron2;
    } else {
        referencePolyhedron = polyhedron2;
        incidentPolyhedron = polyhedron1;
    }

    const glm::vec3 axisReferenceSpace = referencePolyhedron->getFaceNormal(minFaceIndex);
    const glm::vec3 axisIncidentSpace = matrices::RotationFromMatrix(referenceToIncidentTransform) * axisReferenceSpace;

    // Compute the world normal
    const glm::vec3 normalWorld = isMinPenetrationFaceNormalPolyhedron1 ? matrices::RotationFromMatrix(transA) *
                                                                          axisReferenceSpace : -(
            matrices::RotationFromMatrix(transB) * axisReferenceSpace);

    // Get the reference face
    const HalfEdgeStructure::Face &referenceFace = referencePolyhedron->getFace(minFaceIndex);

    // Find the incident face on the other polyhedron (most anti-parallel face)
    uint32 incidentFaceIndex = incidentPolyhedron->findMostAntiParallelFace(axisIncidentSpace);

    // Get the incident face
    const HalfEdgeStructure::Face &incidentFace = incidentPolyhedron->getFace(incidentFaceIndex);

    const uint32 nbIncidentFaceVertices = static_cast<uint32>(incidentFace.faceVertices.size());
    const uint32 nbMaxElements = nbIncidentFaceVertices * 2 * static_cast<uint32>(referenceFace.faceVertices.size());
    std::vector<glm::vec3> verticesTemp1;
    verticesTemp1.reserve(nbMaxElements);
    std::vector<glm::vec3> verticesTemp2;
    verticesTemp2.reserve(nbMaxElements);

    // Get all the vertices of the incident face (in the reference local-space)
    for (uint32 i = 0; i < nbIncidentFaceVertices; i++) {
        const glm::vec3 faceVertexIncidentSpace = incidentPolyhedron->getVertexPosition(incidentFace.faceVertices[i]);
        verticesTemp1.emplace_back(incidentToReferenceTransform * glm::vec4(faceVertexIncidentSpace, 1.0f));
    }

    // For each edge of the reference we use it to clip the incident face polygon using Sutherland-Hodgman algorithm
    uint32 firstEdgeIndex = referenceFace.edgeIndex;
    bool areVertices1Input = false;
    uint32 nbOutputVertices;
    uint32 currentEdgeIndex;

    // Get the adjacent edge
    const HalfEdgeStructure::Edge *currentEdge = &(referencePolyhedron->getHalfEdge(firstEdgeIndex));
    glm::vec3 edgeV1 = referencePolyhedron->getVertexPosition(currentEdge->vertexIndex);

    do {

        // Switch the input/output arrays of vertices
        areVertices1Input = !areVertices1Input;

        // Get the adjacent edge
        const HalfEdgeStructure::Edge *nextEdge = &(referencePolyhedron->getHalfEdge(currentEdge->nextEdgeIndex));

        // Compute the edge vertices and edge direction
        const glm::vec3 edgeV2 = referencePolyhedron->getVertexPosition(nextEdge->vertexIndex);
        const glm::vec3 edgeDirection = edgeV2 - edgeV1;

        // Compute the normal of the clipping plane for this edge
        // The clipping plane is perpendicular to the edge direction and the reference face normal
        const glm::vec3 planeNormal = glm::cross(axisReferenceSpace, edgeDirection);

        assert((areVertices1Input && verticesTemp1.size() > 0) || !areVertices1Input);
        assert((!areVertices1Input && verticesTemp2.size() > 0) || areVertices1Input);

        // Clip the incident face with one adjacent plane (corresponding to one edge) of the reference face
        clipPolygonWithPlane(areVertices1Input ? verticesTemp1 : verticesTemp2, edgeV1, planeNormal,
                             areVertices1Input ? verticesTemp2 : verticesTemp1);

        currentEdgeIndex = currentEdge->nextEdgeIndex;

        // Go to the next adjacent edge of the reference face
        currentEdge = nextEdge;
        edgeV1 = edgeV2;

        // Clear the input array of vertices before the next loop
        if (areVertices1Input) {
            verticesTemp1.clear();
            nbOutputVertices = static_cast<uint32>(verticesTemp2.size());
        } else {
            verticesTemp2.clear();
            nbOutputVertices = static_cast<uint32>(verticesTemp1.size());
        }

    } while (currentEdgeIndex != firstEdgeIndex && nbOutputVertices > 0);

    // Reference to the output clipped polygon vertices
    std::vector<glm::vec3> &clippedPolygonVertices = areVertices1Input ? verticesTemp2 : verticesTemp1;

    // We only keep the clipped points that are below the reference face
    const glm::vec3 referenceFaceVertex = referencePolyhedron->getVertexPosition(
            referencePolyhedron->getHalfEdge(firstEdgeIndex).vertexIndex);
    bool contactPointsFound = false;
    const uint32 nbClipPolygonVertices = static_cast<uint32>(clippedPolygonVertices.size());
    for (uint32 i = 0; i < nbClipPolygonVertices; i++) {

        // Compute the penetration depth of this contact point (can be different from the minPenetration depth which is
        // the maximal penetration depth of any contact point for this separating axis
        float penetrationDepth = glm::dot(referenceFaceVertex - clippedPolygonVertices[i], axisReferenceSpace);

        // If the clip point is below the reference face
        if (penetrationDepth > float(0.0)) {

            contactPointsFound = true;

            // If we need to report contacts
            if (true) {

                glm::vec3 outWorldNormal = normalWorld;

                // Convert the clip incident polyhedron vertex into the incident polyhedron local-space
                glm::vec3 contactPointIncidentPolyhedron =
                        referenceToIncidentTransform * glm::vec4(clippedPolygonVertices[i], 1);

                // Project the contact point onto the reference face
                glm::vec3 contactPointReferencePolyhedron = projectPointOntoPlane(clippedPolygonVertices[i],
                                                                                  axisReferenceSpace,
                                                                                  referenceFaceVertex);

                // Compute smooth triangle mesh contact if one of the two collision shapes is a triangle
//                TriangleShape::computeSmoothTriangleMeshContact(narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape1, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape2,
//                                                                isMinPenetrationFaceNormalPolyhedron1 ? contactPointReferencePolyhedron : contactPointIncidentPolyhedron,
//                                                                isMinPenetrationFaceNormalPolyhedron1 ? contactPointIncidentPolyhedron : contactPointReferencePolyhedron,
//                                                                narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape1ToWorldTransform, narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].shape2ToWorldTransform,
//                                                                penetrationDepth, outWorldNormal);

                // Create a new contact point
//                narrowPhaseInfoBatch.addContactPoint(batchIndex, outWorldNormal, penetrationDepth,
//                                                     isMinPenetrationFaceNormalPolyhedron1 ? contactPointReferencePolyhedron : contactPointIncidentPolyhedron,
//                                                     isMinPenetrationFaceNormalPolyhedron1 ? contactPointIncidentPolyhedron : contactPointReferencePolyhedron);
                contacts.emplace_back(outWorldNormal, penetrationDepth,
                                      isMinPenetrationFaceNormalPolyhedron1 ? contactPointReferencePolyhedron
                                                                            : contactPointIncidentPolyhedron,
                                      isMinPenetrationFaceNormalPolyhedron1 ? contactPointIncidentPolyhedron
                                                                            : contactPointReferencePolyhedron);
            }
        }
    }

    return contactPointsFound;
}


// Compute and return the distance between the two edges in the direction of the candidate separating axis
float SATAlgorithm::computeDistanceBetweenEdges(const glm::vec3 &edge1A, const glm::vec3 &edge2A,
                                                const glm::vec3 &polyhedron1Centroid,
                                                const glm::vec3 &polyhedron2Centroid, const glm::vec3 &edge1Direction,
                                                const glm::vec3 &edge2Direction, bool isShape1Triangle,
                                                glm::vec3 &outSeparatingAxisPolyhedron2Space) const {

    // If the two edges are parallel
    if (areParallelVectors(edge1Direction, edge2Direction)) {

        // Return a large penetration depth to skip those edges
        return std::numeric_limits<float>::max();
    }

    // Compute the candidate separating axis (cross product between two polyhedrons edges)
    glm::vec3 axis = glm::normalize(glm::cross(edge1Direction, edge2Direction));

    // Make sure the axis direction is going from first to second polyhedron
    float dotProd;
    if (isShape1Triangle) {

        // The shape 1 is a triangle. It is safer to use a vector from
        // centroid to edge of the shape 2 because for a triangle, we
        // can have a vector that is orthogonal to the axis

        dotProd = glm::dot(axis, edge2A - polyhedron2Centroid);
    } else {

        // The shape 2 might be a triangle. It is safer to use a vector from
        // centroid to edge of the shape 2 because for a triangle, we
        // can have a vector that is orthogonal to the axis

        dotProd = glm::dot(axis, polyhedron1Centroid - edge1A);
    }
    if (dotProd > float(0.0)) {
        axis = -axis;
    }

    outSeparatingAxisPolyhedron2Space = axis;

    // Compute and return the distance between the edges
    return glm::dot(-axis, edge2A - edge1A);
}


// Return the penetration depth between two polyhedra along a face normal axis of the first polyhedron
float
SATAlgorithm::testSingleFaceDirectionPolyhedronVsPolyhedron(const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                                            const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                                            const glm::mat4 &polyhedron1ToPolyhedron2,
                                                            uint32 faceIndex) const {

    const HalfEdgeStructure::Face &face = polyhedron1->getFace(faceIndex);

    // Get the face normal
    const glm::vec3 faceNormal = polyhedron1->getFaceNormal(faceIndex);

    // Convert the face normal into the local-space of polyhedron 2
    const glm::vec3 faceNormalPolyhedron2Space = matrices::RotationFromMatrix(polyhedron1ToPolyhedron2) * faceNormal;

    // Get the support point of polyhedron 2 in the inverse direction of face normal
    const glm::vec3 supportPoint = polyhedron2->getLocalSupportPointWithoutMargin(-faceNormalPolyhedron2Space);

    // Compute the penetration depth
    const glm::vec3 faceVertex =
            polyhedron1ToPolyhedron2 * glm::vec4(polyhedron1->getVertexPosition(face.faceVertices[0]), 1);
    float penetrationDepth = glm::dot(faceVertex - supportPoint, faceNormalPolyhedron2Space);

    return penetrationDepth;
}

// Test all the normals of a polyhedron for separating axis in the polyhedron vs polyhedron case
float SATAlgorithm::testFacesDirectionPolyhedronVsPolyhedron(const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                                             const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                                             const glm::mat4 &polyhedron1ToPolyhedron2,
                                                             uint32 &minFaceIndex) const {

    float minPenetrationDepth = std::numeric_limits<float>::max();

    // For each face of the first polyhedron
    for (uint32 f = 0; f < polyhedron1->getNbFaces(); f++) {

        float penetrationDepth = testSingleFaceDirectionPolyhedronVsPolyhedron(polyhedron1, polyhedron2,
                                                                               polyhedron1ToPolyhedron2, f);

        // If the penetration depth is negative, we have found a separating axis
        if (penetrationDepth <= float(0.0)) {
            minFaceIndex = f;
            return penetrationDepth;
        }

        // Check if we have found a new minimum penetration axis
        if (penetrationDepth < minPenetrationDepth) {
            minPenetrationDepth = penetrationDepth;
            minFaceIndex = f;
        }
    }

    return minPenetrationDepth;
}


// Return true if two edges of two polyhedrons build a minkowski face (and can therefore be a separating axis)
bool SATAlgorithm::testEdgesBuildMinkowskiFace(const algo::shapes::ConvexPolyhedronShape *polyhedron1,
                                               const HalfEdgeStructure::Edge &edge1,
                                               const algo::shapes::ConvexPolyhedronShape *polyhedron2,
                                               const HalfEdgeStructure::Edge &edge2,
                                               const glm::mat4 &polyhedron1ToPolyhedron2) const {

    const glm::vec3 a =
            matrices::RotationFromMatrix(polyhedron1ToPolyhedron2) * polyhedron1->getFaceNormal(edge1.faceIndex);
    const glm::vec3 b = matrices::RotationFromMatrix(polyhedron1ToPolyhedron2) *
                        polyhedron1->getFaceNormal(polyhedron1->getHalfEdge(edge1.twinEdgeIndex).faceIndex);

    const glm::vec3 c = polyhedron2->getFaceNormal(edge2.faceIndex);
    const glm::vec3 d = polyhedron2->getFaceNormal(polyhedron2->getHalfEdge(edge2.twinEdgeIndex).faceIndex);

    // Compute b.cross(a) using the edge direction
    const glm::vec3 edge1Vertex1 = polyhedron1->getVertexPosition(edge1.vertexIndex);
    const glm::vec3 edge1Vertex2 = polyhedron1->getVertexPosition(
            polyhedron1->getHalfEdge(edge1.twinEdgeIndex).vertexIndex);
    const glm::vec3 bCrossA = matrices::RotationFromMatrix(polyhedron1ToPolyhedron2) * (edge1Vertex1 - edge1Vertex2);

    // Compute d.cross(c) using the edge direction
    const glm::vec3 edge2Vertex1 = polyhedron2->getVertexPosition(edge2.vertexIndex);
    const glm::vec3 edge2Vertex2 = polyhedron2->getVertexPosition(
            polyhedron2->getHalfEdge(edge2.twinEdgeIndex).vertexIndex);
    const glm::vec3 dCrossC = edge2Vertex1 - edge2Vertex2;

    // Test if the two arcs of the Gauss Map intersect (therefore forming a minkowski face)
    // Note that we negate the normals of the second polyhedron because we are looking at the
    // Gauss map of the minkowski difference of the polyhedrons
    return testGaussMapArcsIntersect(a, b, -c, -d, bCrossA, dCrossC);
}


// Return true if the arcs AB and CD on the Gauss Map (unit sphere) intersect
/// This is used to know if the edge between faces with normal A and B on first polyhedron
/// and edge between faces with normal C and D on second polygon create a face on the Minkowski
/// sum of both polygons. If this is the case, it means that the cross product of both edges
/// might be a separating axis.
bool
SATAlgorithm::testGaussMapArcsIntersect(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec3 &d,
                                        const glm::vec3 &bCrossA, const glm::vec3 &dCrossC) const {

    const float cba = glm::dot(c, bCrossA);
    const float dba = glm::dot(d, bCrossA);
    const float adc = glm::dot(a, dCrossC);
    const float bdc = glm::dot(b, dCrossC);

    return cba * dba < float(0.0) && adc * bdc < float(0.0) && cba * bdc > float(0.0);
}