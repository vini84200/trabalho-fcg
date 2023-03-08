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
#include "algos/gjk/gjk.h"

#include <cassert>

#include "algos/gjk/VoronoiSimplex.hpp"
#include "glm/glm.hpp"
#include "utils/utils.hpp"
#include "glm/detail/type_quat.hpp"
#include "utils/matrices.h"
#include "glm/gtx/norm.hpp"
#include "Contact.h"

// We want to use the ReactPhysics3D namespace
using namespace reactphysics3d;

// Compute a contact info if the two collision shapes collide.
/// This method implements the Hybrid Technique for computing the penetration depth by
/// running the GJK algorithm on original objects (without margin). If the shapes intersect
/// only in the margins, the method compute the penetration depth and contact points
/// (of enlarged objects). If the original objects (without margin) intersect, we
/// call the computePenetrationDepthForEnlargedObjects() method that run the GJK
/// algorithm on the enlarged object to obtain a simplex polytope that contains the
/// origin, they we give that simplex polytope to the EPA algorithm which will compute
/// the correct penetration depth and contact points between the enlarged objects.
void GJKAlgorithm::testCollision(const algo::shapes::Shape &shapeA, const glm::mat4 &transA,
                                 const algo::shapes::Shape &shapeB,
                                 const glm::mat4 &transB, GJKResult &gjkResult, Contact &contact) {

    glm::vec3 suppA;             // Support point of object A
    glm::vec3 suppB;             // Support point of object B
    glm::vec3 w;                 // Support point of Minkowski difference A-B
    glm::vec3 pA;                // Closest point of object A
    glm::vec3 pB;                // Closest point of object B
    float vDotw;
    float prevDistSquare;
    bool contactFound = false;

//    assert(narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape1->isConvex());
//    assert(narrowPhaseInfoBatch.narrowPhaseInfos[batchIndex].collisionShape2->isConvex());

    const algo::shapes::Shape *shape1 = &shapeA;
    const algo::shapes::Shape *shape2 = &shapeB;

    // Get the local-space to world-space transforms
    const glm::mat4 &transform1 = transA;
    const glm::mat4 &transform2 = transB;

    // Transform a point from local space of body 2 to local
    // space of body 1 (the GJK algorithm is done in local space of body 1)
    glm::mat4 const transform1Inverse = glm::inverse(transform1);
    glm::mat4 const body2Tobody1 = transform1Inverse * transform2;

    // Quaternion that transform a direction from local
    // space of body 1 into local space of body 2
//    glm::quat rotateToBody2 = transform2.getOrientation().getInverse() * transform1.getOrientation();
    glm::quat const rotateToBody2 =
            glm::inverse(matrices::RotationFromMatrix(transform1)) * matrices::RotationFromMatrix(transform2);

    // Initialize the margin (sum of margins of both objects)
    float const margin = shape1->getMargin() + shape2->getMargin();
    float const marginSquare = margin * margin;
    assert(margin > float(0.0));

    // Create a simplex set
    VoronoiSimplex simplex;

    // Get the last collision frame info

    // Get the previous point V (last cached separating axis)
    glm::vec3 v;
    v = glm::vec3(0, 1, 0);

    // Initialize the upper bound for the square distance
    float distSquare = std::numeric_limits<float>::max();

    bool noIntersection = false;

    do {

        // Compute the support points for original objects (without margins) A and B
        suppA = shape1->getLocalSupportPointWithoutMargin(-v);
        suppB = body2Tobody1 * glm::vec4(shape2->getLocalSupportPointWithoutMargin(rotateToBody2 * v), 1.0);

        // Compute the support point for the Minkowski difference A-B
        w = suppA - suppB;

        vDotw = glm::dot(v, w);

        // If to enlarge objects (with margins) do not intersect
        if (vDotw > float(0.0) && vDotw * vDotw > distSquare * marginSquare) {

            // Cache the current separating axis for frame coherence
            // No intersection, we return
            gjkResult = GJKResult::SEPARATED;
            noIntersection = true;
            break;
        }

        // If the objects intersect only in the margins
        if (simplex.isPointInSimplex(w) || distSquare - vDotw <= distSquare * REL_ERROR_SQUARE) {

            // Contact point has been found
            contactFound = true;
            break;
        }

        // Add the new support point to the simplex
        simplex.addPoint(w, suppA, suppB);

        // If the simplex is affinely dependent
        if (simplex.isAffinelyDependent()) {

            // Contact point has been found
            contactFound = true;
            break;
        }

        // Compute the point of the simplex closest to the origin
        // If the computation of the closest point fails
        if (!simplex.computeClosestPoint(v)) {

            // Contact point has been found
            contactFound = true;
            break;
        }

        // Store and update the squared distance of the closest point
        prevDistSquare = distSquare;
        distSquare = glm::length2(v);

        // If the distance to the closest point doesn't improve a lot
        if (prevDistSquare - distSquare <= MACHINE_EPSILON * prevDistSquare) {

            simplex.backupClosestPointInSimplex(v);

            // Get the new squared distance
            distSquare = glm::length2(v);

            // Contact point has been found
            contactFound = true;
            break;
        }

    } while (!simplex.isFull() && distSquare > MACHINE_EPSILON * simplex.getMaxLengthSquareOfAPoint());

    if (noIntersection) {
        return;
    }

    if (contactFound && distSquare > MACHINE_EPSILON) {

        // Compute the closet points of both objects (without the margins)
        simplex.computeClosestPointsOfAandB(pA, pB);

        // Project those two points on the margins to have the closest points of both
        // object with the margins
        float dist = std::sqrt(distSquare);
        assert(dist > float(0.0));
        pA = (pA - (shape1->getMargin() / dist) * v);
        pB = glm::inverse(body2Tobody1) * glm::vec4(pB + (shape2->getMargin() / dist) * v, 1.0);

        // Compute the contact info
        glm::vec3 normal = glm::normalize(matrices::RotationFromMatrix(transform1) * (-v));
        float penetrationDepth = margin - dist;

        // If the penetration depth is negative (due too numerical errors), there is no contact
        if (penetrationDepth <= float(0.0)) {
            gjkResult = GJKResult::SEPARATED;
            return;
        }

        // Do not generate a contact point with zero normal length
        if (glm::length2(normal) < MACHINE_EPSILON) {
            gjkResult = GJKResult::SEPARATED;
            return;
        }


        // Add a new contact point
        contact = Contact(normal, penetrationDepth, pA, pB);

        // If the objects intersect only in the margins
        gjkResult = GJKResult::COLLIDE_IN_MARGIN;

        return;
    }

    gjkResult = GJKResult::INTERPENETRATE;

}