//FONTE: (extra) https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/narrowphase/GJK/GJKAlgorithm.h
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

#ifndef REACTPHYSICS3D_GJK_ALGORITHM_H
#define REACTPHYSICS3D_GJK_ALGORITHM_H

#include "algos/algoShape.hpp"
#include "algos/gjk/Contact.h"
#include "glm/ext/matrix_float4x4.hpp"

/// ReactPhysics3D namespace
namespace reactphysics3d {

  // Declarations
  struct ContactManifoldInfo;
  struct NarrowPhaseInfoBatch;
  class ConvexShape;
  class VoronoiSimplex;
  template<typename T> class Array;

  // Constants
  constexpr float REL_ERROR = 1.0e-3F;
  constexpr float REL_ERROR_SQUARE = REL_ERROR * REL_ERROR;
  constexpr int MAX_ITERATIONS_GJK_RAYCAST = 32;

  typedef unsigned int uint32;

  // Class GJKAlgorithm
  /**
 * This class implements a narrow-phase collision detection algorithm. This
 * algorithm uses the ISA-GJK algorithm. This
 * implementation is based on the implementation discussed in the book
 * "Collision Detection in Interactive 3D Environments" by Gino van den Bergen.
 * This method implements the Hybrid Technique for calculating the
 * penetration depth. The two objects are enlarged with a small margin. If
 * the object intersects in their margins, the penetration depth is quickly
 * computed using the GJK algorithm on the original objects (without margin).
 * If the original objects (without margin) intersect, we exit GJK and run
 * the SAT algorithm to get contacts and collision data.
   */
  class GJKAlgorithm {

   private :

    // -------------------- Attributes -------------------- //


    // -------------------- Methods -------------------- //

   public :

    enum class GJKResult {
      SEPARATED,              // The two shapes are separated outside the margin
      COLLIDE_IN_MARGIN,      // The two shapes overlap only in the margin (shallow penetration)
      INTERPENETRATE          // The two shapes overlap event without the margin (deep penetration)
    };

    // -------------------- Methods -------------------- //

    /// Constructor
    GJKAlgorithm() = default;

    /// Destructor
    ~GJKAlgorithm() = default;

    /// Deleted copy-constructor
    GJKAlgorithm(const GJKAlgorithm& algorithm) = delete;

    /// Deleted assignment operator
    GJKAlgorithm& operator=(const GJKAlgorithm& algorithm) = delete;

    /// Compute a contact info if the two bounding volumes collide.
    void testCollision(const algo::shapes::Shape &shapeA, const glm::mat4 &transA, const algo::shapes::Shape &shapeB,
                       const glm::mat4 &transB, GJKResult &gjkResult, Contact& contact);


  };

}

#endif