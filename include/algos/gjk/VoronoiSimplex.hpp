// FONTE: (extra) https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/narrowphase/GJK/VoronoiSimplex.h

// THIS FILE HAS BEEN ADAPTED FROM THE BELOW SOURCE:
// IT HAS SOME CHENGES
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

#ifndef REACTPHYSICS3D_VORONOI_SIMPLEX_H
#define    REACTPHYSICS3D_VORONOI_SIMPLEX_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

/// ReactPhysics3D namespace
namespace reactphysics3d {

// Type definitions
    typedef unsigned int Bits;

// Class VoronoiSimplex
/**
 * This class represents a simplex which is a set of 3D points. This
 * class is used in the GJK algorithm. This implementation is based in the book
 * "Real-Time Collision Detection" by Christer Ericson. This simple is used to
 * replace theJohnson's algorithm for computing the point of a simplex that
 * is closest to the origin and also the smallest simplex needed to represent
 * that closest point.
 */
    class VoronoiSimplex {

    private:

        // -------------------- Attributes -------------------- //

        /// Current points
        glm::vec3 mPoints[4];

        /// Number of vertices in the simplex
        int mNbPoints;

        /// Barycentric coordinates of the closest point using simplex vertices
        float mBarycentricCoords[4];

        /// pointsLengthSquare[i] = (points[i].length)^2
        float mPointsLengthSquare[4];

        /// Support points of object A in local coordinates
        glm::vec3 mSuppPointsA[4];

        /// Support points of object B in local coordinates
        glm::vec3 mSuppPointsB[4];

        /// True if the closest point has to be recomputed (because the simplex has changed)
        bool mRecomputeClosestPoint;

        /// Current point that is closest to the origin
        glm::vec3 mClosestPoint;

        /// Current closest point on object A
        glm::vec3 mClosestSuppPointA;

        /// Current closest point on object B
        glm::vec3 mClosestSuppPointB;

        /// True if the last computed closest point is valid
        bool mIsClosestPointValid;

        /// Epsilon value
        static float epsilon;

        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        VoronoiSimplex(const VoronoiSimplex &simplex);

        /// Private assignment operator
        VoronoiSimplex &operator=(const VoronoiSimplex &simplex);

        /// Set the barycentric coordinates of the closest point
        void setBarycentricCoords(float a, float b, float c, float d);

        /// Recompute the closest point if the simplex has been modified
        bool recomputeClosestPoint();

        /// Return true if the
        bool checkClosestPointValid() const;

        /// Compute point of a line segment that is closest to the origin
        void computeClosestPointOnSegment(const glm::vec3 &a, const glm::vec3 &b, int &bitUsedVertices,
                                          float &t) const;

        /// Compute point of a triangle that is closest to the origin
        void computeClosestPointOnTriangle(const glm::vec3 &a, const glm::vec3 &b,
                                           const glm::vec3 &c, int &bitsUsedPoints, glm::vec3 &baryCoordsABC) const;

        /// Compute point of a tetrahedron that is closest to the origin
        bool computeClosestPointOnTetrahedron(const glm::vec3 &a, const glm::vec3 &b,
                                              const glm::vec3 &c, const glm::vec3 &d,
                                              int &bitsUsedPoints, glm::vec2 &baryCoordsAB, glm::vec2 &baryCoordsCD,
                                              bool &isDegenerate) const;

        /// Test if a point is outside of the plane given by the points of the triangle (a, b, c)
        int
        testOriginOutsideOfPlane(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec3 &d) const;

        /// Remap the used vertices bits of a triangle to the corresponding used vertices of a tetrahedron
        int mapTriangleUsedVerticesToTetrahedron(int triangleUsedVertices, int first, int second, int third) const;

    public:

        // -------------------- Methods -------------------- //

        /// Constructor
        VoronoiSimplex();

        /// Destructor
        ~VoronoiSimplex();

        /// Return true if the simplex contains 4 points
        bool isFull() const;

        /// Return true if the simplex is empty
        bool isEmpty() const;

        /// Return the points of the simplex
        int getSimplex(glm::vec3 *mSuppPointsA, glm::vec3 *mSuppPointsB, glm::vec3 *mPoints) const;

        /// Return the maximum squared length of a point
        float getMaxLengthSquareOfAPoint() const;

        /// Add a new support point of (A-B) into the simplex
        void addPoint(const glm::vec3 &point, const glm::vec3 &suppPointA, const glm::vec3 &suppPointB);

        /// Remove a point from the simplex
        void removePoint(int index);

        /// Reduce the simplex (only keep vertices that participate to the point closest to the origin)
        void reduceSimplex(int bitsUsedPoints);

        /// Return true if the point is in the simplex
        bool isPointInSimplex(const glm::vec3 &point) const;

        /// Return true if the set is affinely dependent
        bool isAffinelyDependent() const;

        /// Backup the closest point
        void backupClosestPointInSimplex(glm::vec3 &point);

        /// Compute the closest points "pA" and "pB" of object A and B.
        void computeClosestPointsOfAandB(glm::vec3 &pA, glm::vec3 &pB) const;

        /// Compute the closest point to the origin of the current simplex.
        bool computeClosestPoint(glm::vec3 &v);
    };

// Return true if the simplex contains 4 points
    inline bool VoronoiSimplex::isFull() const {
        return mNbPoints == 4;
    }

// Return true if the simple is empty
    inline bool VoronoiSimplex::isEmpty() const {
        return mNbPoints == 0;
    }

// Set the barycentric coordinates of the closest point
    inline void VoronoiSimplex::setBarycentricCoords(float a, float b, float c, float d) {
        mBarycentricCoords[0] = a;
        mBarycentricCoords[1] = b;
        mBarycentricCoords[2] = c;
        mBarycentricCoords[3] = d;
    }

// Compute the closest point "v" to the origin of the current simplex.
    inline bool VoronoiSimplex::computeClosestPoint(glm::vec3 &v) {

        bool isValid = recomputeClosestPoint();
        v = mClosestPoint;
        return isValid;
    }

// Return true if the
    inline bool VoronoiSimplex::checkClosestPointValid() const {
        return mBarycentricCoords[0] >= float(0.0) && mBarycentricCoords[1] >= float(0.0) &&
               mBarycentricCoords[2] >= float(0.0) && mBarycentricCoords[3] >= float(0.0);
    }

}

#endif